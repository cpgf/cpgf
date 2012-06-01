import math

def doMain() :
	# Defines MyPai
	MyPai = 3.14159

	# Create the window of the application
	videoMode = sfml.VideoMode(800, 600, 32)
	title = "SFML Pong -- cpgf SFML binding demo -- Python"
	App = sfml.RenderWindow(videoMode, title)

	# Load the sounds used in the game
	BallSoundBuffer = sfml.SoundBuffer()
	
	if not BallSoundBuffer.LoadFromFile("datas/pong/ball.wav") :
		return sfml.EXIT_FAILURE
	
	BallSound = sfml.Sound(BallSoundBuffer)

	# Load the images used in the game
	BackgroundImage = sfml.Image()
	LeftPaddleImage = sfml.Image()
	RightPaddleImage = sfml.Image()
	BallImage = sfml.Image()

	if (not BackgroundImage.LoadFromFile("datas/pong/background.jpg")) \
		or (not LeftPaddleImage.LoadFromFile("datas/pong/paddle_left.png")) \
		or (not RightPaddleImage.LoadFromFile("datas/pong/paddle_right.png")) \
		or (not BallImage.LoadFromFile("datas/pong/ball.png")) :
		return EXIT_FAILURE

	# Load the text font
	Cheeseburger = sfml.Font()
	if not Cheeseburger.LoadFromFile("datas/post-fx/cheeseburger.ttf") :
		return EXIT_FAILURE

	# Initialize the end text
	End = sfml.String()
	End.SetFont(Cheeseburger)
	End.SetSize(60.0)
	End.Move(150.0, 200.0)
	End.SetColor(sfml.Color(50, 50, 250))

	# Create the sprites of the background, the paddles and the ball
	Background = sfml.Sprite(BackgroundImage)
	LeftPaddle = sfml.Sprite(LeftPaddleImage)
	RightPaddle = sfml.Sprite(RightPaddleImage)
	Ball = sfml.Sprite(BallImage)

	LeftPaddle.Move(10, (App.GetView().GetRect().GetHeight() - LeftPaddle.GetSize().y) / 2)
	RightPaddle.Move(App.GetView().GetRect().GetWidth() - RightPaddle.GetSize().x - 10, (App.GetView().GetRect().GetHeight() - RightPaddle.GetSize().y) / 2)
	Ball.Move((App.GetView().GetRect().GetWidth() - Ball.GetSize().x) / 2, (App.GetView().GetRect().GetHeight() - Ball.GetSize().y) / 2)

	# Define the paddles properties
	AITimer = sfml.Clock()
	AITime     = 0.1
	LeftPaddleSpeed  = 400.0
	RightPaddleSpeed = 400.0

	# Define the ball properties
	BallSpeed = 400.0
	
	BallAngle = 0.5

	while 1 :
		# Make sure the ball initial angle is not too much vertical
		BallAngle = sfml.Randomizer.Random(0.0, 2 * MyPai)
		if math.fabs(math.cos(BallAngle)) >= 0.7 : break

	IsPlaying = 1
	while App.IsOpened() :
		# Handle events
		Event = sfml.Event()
		while App.GetEvent(Event) :
			# Window closed or escape key pressed : exit
			if Event.Type == sfml.Event.Closed \
			   or ((Event.Type == sfml.Event.KeyPressed) and (Event.Key.Code == sfml.Escape)) :
				App.Close()
				break
		if IsPlaying :
			# Move the player's paddle
			if App.GetInput().IsKeyDown(sfml.Up) and (LeftPaddle.GetPosition().y > 5.0) :
				LeftPaddle.Move(0.0, -LeftPaddleSpeed * App.GetFrameTime())
			if App.GetInput().IsKeyDown(sfml.Down) and (LeftPaddle.GetPosition().y < App.GetView().GetRect().GetHeight() - LeftPaddle.GetSize().y - 5.0) :
				LeftPaddle.Move(0.0, LeftPaddleSpeed * App.GetFrameTime())

			# Move the computer's paddle
			if ((RightPaddleSpeed < 0.0) and (RightPaddle.GetPosition().y > 5.0)) or \
				((RightPaddleSpeed > 0.0) and (RightPaddle.GetPosition().y < App.GetView().GetRect().GetHeight() - RightPaddle.GetSize().y - 5.0)) :
				RightPaddle.Move(0.0, RightPaddleSpeed * App.GetFrameTime())

			# Update the computer's paddle direction according to the ball position
			if AITimer.GetElapsedTime() > AITime :
				AITimer.Reset()
				if (RightPaddleSpeed < 0) and (Ball.GetPosition().y + Ball.GetSize().y > RightPaddle.GetPosition().y + RightPaddle.GetSize().y) :
					RightPaddleSpeed = -RightPaddleSpeed
				if (RightPaddleSpeed > 0) and (Ball.GetPosition().y < RightPaddle.GetPosition().y) :
					RightPaddleSpeed = -RightPaddleSpeed

			# Move the ball
			Factor = BallSpeed * App.GetFrameTime()
			Ball.Move(math.cos(BallAngle) * Factor, math.sin(BallAngle) * Factor)

			# Check collisions between the ball and the screen
			if Ball.GetPosition().x < 0.0 :
				IsPlaying = 0
				End.SetText("You lost !\n(press escape to exit)")
				
			if Ball.GetPosition().x + Ball.GetSize().x > App.GetView().GetRect().GetWidth() :
				IsPlaying = False
				End.SetText("You won !\n(press escape to exit)")
				
			if Ball.GetPosition().y < 0.0 :
				BallSound.Play()
				BallAngle = -BallAngle
				Ball.SetY(0.1)

			if Ball.GetPosition().y + Ball.GetSize().y > App.GetView().GetRect().GetHeight() :
				BallSound.Play()
				BallAngle = -BallAngle
				Ball.SetY(App.GetView().GetRect().GetHeight() - Ball.GetSize().y - 0.1)

			# Check the collisions between the ball and the paddles
			# Left Paddle
			if Ball.GetPosition().x < LeftPaddle.GetPosition().x + LeftPaddle.GetSize().x and \
				Ball.GetPosition().x > LeftPaddle.GetPosition().x + (LeftPaddle.GetSize().x / 2.0) and \
				Ball.GetPosition().y + Ball.GetSize().y >= LeftPaddle.GetPosition().y and \
				Ball.GetPosition().y <= LeftPaddle.GetPosition().y + LeftPaddle.GetSize().y :
				BallSound.Play()
				BallAngle = MyPai - BallAngle
				Ball.SetX(LeftPaddle.GetPosition().x + LeftPaddle.GetSize().x + 0.1)

			# Right Paddle
			if Ball.GetPosition().x + Ball.GetSize().x > RightPaddle.GetPosition().x and \
				Ball.GetPosition().x + Ball.GetSize().x < RightPaddle.GetPosition().x + (RightPaddle.GetSize().x / 2.0) and \
				Ball.GetPosition().y + Ball.GetSize().y >= RightPaddle.GetPosition().y and \
				Ball.GetPosition().y <= RightPaddle.GetPosition().y + RightPaddle.GetSize().y :
				BallSound.Play()
				BallAngle = MyPai - BallAngle
				Ball.SetX(RightPaddle.GetPosition().x - Ball.GetSize().x - 0.1)

		
		# Clear the window
		App.Clear()

		# Draw the background, paddles and ball sprites
		App.Draw(Background)
		App.Draw(LeftPaddle)
		App.Draw(RightPaddle)
		App.Draw(Ball)

		# If the game is over, display the end message
		if not IsPlaying :
			App.Draw(End)

		# Display things on screen
		App.Display()

doMain()
