doMain();

function doMain()
{

	// Defines MyPai
	var MyPai = 3.14159;

	// Create the window of the application
	var App = new sfml.RenderWindow(new sfml.VideoMode(800, 600, 32), "SFML Pong -- cpgf SFML binding demo -- V8 Javascript");

	// Load the sounds used in the game
	var BallSoundBuffer = new sfml.SoundBuffer();
	
	if(!BallSoundBuffer.LoadFromFile("datas/pong/ball.wav"))
	{
		return sfml.EXIT_FAILURE;
	}
	var BallSound = new sfml.Sound(BallSoundBuffer);

	// Load the images used in the game
	var BackgroundImage = new sfml.Image();
	var LeftPaddleImage = new sfml.Image;
	var RightPaddleImage = new sfml.Image();
	var BallImage = new sfml.Image();
	if (!BackgroundImage.LoadFromFile("datas/pong/background.jpg")    ||
		!LeftPaddleImage.LoadFromFile("datas/pong/paddle_left.png")   ||
		!RightPaddleImage.LoadFromFile("datas/pong/paddle_right.png") ||
		!BallImage.LoadFromFile("datas/pong/ball.png"))
	{
		return sfml.EXIT_FAILURE;
	}

	// Load the text font
	var Cheeseburger = new sfml.Font();
	if (!Cheeseburger.LoadFromFile("datas/post-fx/cheeseburger.ttf"))
		return sfml.EXIT_FAILURE;

	// Initialize the end text
	var End = new sfml.String();
	End.SetFont(Cheeseburger);
	End.SetSize(60.0);
	End.Move(150.0, 200.0);
	End.SetColor(new sfml.Color(50, 50, 250));

	// Create the sprites of the background, the paddles and the ball
	var Background = new sfml.Sprite(BackgroundImage);
	var LeftPaddle = new sfml.Sprite(LeftPaddleImage);
	var RightPaddle = new sfml.Sprite(RightPaddleImage);
	var Ball = new sfml.Sprite(BallImage);

	LeftPaddle.Move(10, (App.GetView().GetRect().GetHeight() - LeftPaddle.GetSize().y) / 2);
	RightPaddle.Move(App.GetView().GetRect().GetWidth() - RightPaddle.GetSize().x - 10, (App.GetView().GetRect().GetHeight() - RightPaddle.GetSize().y) / 2);
	Ball.Move((App.GetView().GetRect().GetWidth() - Ball.GetSize().x) / 2, (App.GetView().GetRect().GetHeight() - Ball.GetSize().y) / 2);

	// Define the paddles properties
	var AITimer = new sfml.Clock();
	var AITime     = 0.1;
	var LeftPaddleSpeed  = 400.0;
	var RightPaddleSpeed = 400.0;


	// Define the ball properties
	var BallSpeed = 400.0;
	var BallAngle;
	do
	{
		// Make sure the ball initial angle is not too much vertical
		BallAngle = sfml.Randomizer.Random(0.0, 2 * MyPai);
	} while(Math.abs(Math.cos(BallAngle)) < 0.7);
	
	var IsPlaying = true;
	while(App.IsOpened())
	{
		// Handle events
		var Event = new sfml.Event();
		while (App.GetEvent(Event))
		{
			// Window closed or escape key pressed : exit
			if ((Event.Type == sfml.Event.Closed) || 
			   ((Event.Type == sfml.Event.KeyPressed) && (Event.Key.Code == sfml.Escape)))
			{
				App.Close();
				break;
			}
		}

		if (IsPlaying)
		{
			// Move the player's paddle
			if (App.GetInput().IsKeyDown(sfml.Up) && (LeftPaddle.GetPosition().y > 5.0))
				LeftPaddle.Move(0.0, -LeftPaddleSpeed * App.GetFrameTime());
			if (App.GetInput().IsKeyDown(sfml.Down) && (LeftPaddle.GetPosition().y < App.GetView().GetRect().GetHeight() - LeftPaddle.GetSize().y - 5.0))
				LeftPaddle.Move(0.0, LeftPaddleSpeed * App.GetFrameTime());

			// Move the computer's paddle
			if (((RightPaddleSpeed < 0.0) && (RightPaddle.GetPosition().y > 5.0)) ||
				((RightPaddleSpeed > 0.0) && (RightPaddle.GetPosition().y < App.GetView().GetRect().GetHeight() - RightPaddle.GetSize().y - 5.0)))
			{
				RightPaddle.Move(0.0, RightPaddleSpeed * App.GetFrameTime());
			}

			// Update the computer's paddle direction according to the ball position
			if (AITimer.GetElapsedTime() > AITime)
			{
				AITimer.Reset();
				if ((RightPaddleSpeed < 0) && (Ball.GetPosition().y + Ball.GetSize().y > RightPaddle.GetPosition().y + RightPaddle.GetSize().y))
					RightPaddleSpeed = -RightPaddleSpeed;
				if ((RightPaddleSpeed > 0) && (Ball.GetPosition().y < RightPaddle.GetPosition().y))
					RightPaddleSpeed = -RightPaddleSpeed;
			}

			// Move the ball
			var Factor = BallSpeed * App.GetFrameTime();
			Ball.Move(Math.cos(BallAngle) * Factor, Math.sin(BallAngle) * Factor);

			// Check collisions between the ball and the screen
			if (Ball.GetPosition().x < 0.0)
			{
				IsPlaying = false;
				End.SetText("You lost !\n(press escape to exit)");
			}
			if (Ball.GetPosition().x + Ball.GetSize().x > App.GetView().GetRect().GetWidth())
			{
				IsPlaying = false;
				End.SetText("You won !\n(press escape to exit)");
			}
			if (Ball.GetPosition().y < 0.0)
			{
				BallSound.Play();
				BallAngle = -BallAngle;
				Ball.SetY(0.1);
			}

			if (Ball.GetPosition().y + Ball.GetSize().y > App.GetView().GetRect().GetHeight())
			{
				BallSound.Play();
				BallAngle = -BallAngle;
				Ball.SetY(App.GetView().GetRect().GetHeight() - Ball.GetSize().y - 0.1);
			}

			// Check the collisions between the ball and the paddles
			// Left Paddle
			if (Ball.GetPosition().x < LeftPaddle.GetPosition().x + LeftPaddle.GetSize().x && 
				Ball.GetPosition().x > LeftPaddle.GetPosition().x + (LeftPaddle.GetSize().x / 2.0) &&
				Ball.GetPosition().y + Ball.GetSize().y >= LeftPaddle.GetPosition().y &&
				Ball.GetPosition().y <= LeftPaddle.GetPosition().y + LeftPaddle.GetSize().y)
			{
				BallSound.Play();
				BallAngle = MyPai - BallAngle;
				Ball.SetX(LeftPaddle.GetPosition().x + LeftPaddle.GetSize().x + 0.1);
			}

			// Right Paddle
			if (Ball.GetPosition().x + Ball.GetSize().x > RightPaddle.GetPosition().x &&
				Ball.GetPosition().x + Ball.GetSize().x < RightPaddle.GetPosition().x + (RightPaddle.GetSize().x / 2.0) &&
				Ball.GetPosition().y + Ball.GetSize().y >= RightPaddle.GetPosition().y &&
				Ball.GetPosition().y <= RightPaddle.GetPosition().y + RightPaddle.GetSize().y)
			{
				BallSound.Play();
				BallAngle = MyPai - BallAngle;
				Ball.SetX(RightPaddle.GetPosition().x - Ball.GetSize().x - 0.1);
			}
		
		}

		// Clear the window
		App.Clear();

		// Draw the background, paddles and ball sprites
		App.Draw(Background);
		App.Draw(LeftPaddle);
		App.Draw(RightPaddle);
		App.Draw(Ball);

		// If the game is over, display the end message
		if (!IsPlaying)
			App.Draw(End);

		// Display things on screen
		App.Display();
	}

}

