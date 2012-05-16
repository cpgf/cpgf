function doMain()

	-- Defines MyPai
	local MyPai = 3.14159

	-- Create the window of the application
	local App = sfml.RenderWindow(sfml.VideoMode(800, 600, 32), "SFML Pong -- cpgf SFML binding demo -- Lua")

	-- Load the sounds used in the game
	local BallSoundBuffer = sfml.SoundBuffer()
	
	if not BallSoundBuffer.LoadFromFile("datas/pong/ball.wav") then
		return sfml.EXIT_FAILURE
	end
	
	local BallSound = sfml.Sound(BallSoundBuffer)

	-- Load the images used in the game
	local BackgroundImage = sfml.Image()
	local LeftPaddleImage = sfml.Image()
	local RightPaddleImage = sfml.Image()
	local BallImage = sfml.Image()

	if (not BackgroundImage.LoadFromFile("datas/pong/background.jpg"))
		or (not LeftPaddleImage.LoadFromFile("datas/pong/paddle_left.png"))
		or (not RightPaddleImage.LoadFromFile("datas/pong/paddle_right.png"))
		or (not BallImage.LoadFromFile("datas/pong/ball.png"))
	 then
		return EXIT_FAILURE
	end

	-- Load the text font
	local Cheeseburger = sfml.Font()
	if not Cheeseburger.LoadFromFile("datas/post-fx/cheeseburger.ttf") then
		return EXIT_FAILURE
	end

	-- Initialize the end text
	local End = sfml.String()
	End.SetFont(Cheeseburger)
	End.SetSize(60.0)
	End.Move(150.0, 200.0)
	End.SetColor(sfml.Color(50, 50, 250))

	-- Create the sprites of the background, the paddles and the ball
	local Background = sfml.Sprite(BackgroundImage)
	local LeftPaddle = sfml.Sprite(LeftPaddleImage)
	local RightPaddle = sfml.Sprite(RightPaddleImage)
	local Ball = sfml.Sprite(BallImage)

	LeftPaddle.Move(10, (App.GetView().GetRect().GetHeight() - LeftPaddle.GetSize().y) / 2)
	RightPaddle.Move(App.GetView().GetRect().GetWidth() - RightPaddle.GetSize().x - 10, (App.GetView().GetRect().GetHeight() - RightPaddle.GetSize().y) / 2)
	Ball.Move((App.GetView().GetRect().GetWidth() - Ball.GetSize().x) / 2, (App.GetView().GetRect().GetHeight() - Ball.GetSize().y) / 2)

	-- Define the paddles properties
	local AITimer = sfml.Clock()
	local AITime     = 0.1
	local LeftPaddleSpeed  = 400.0
	local RightPaddleSpeed = 400.0

	-- Define the ball properties
	local BallSpeed = 400.0
	local BallAngle

	repeat
		-- Make sure the ball initial angle is not too much vertical
		BallAngle = sfml.Randomizer.Random(0.0, 2 * MyPai)
	until math.abs(math.cos(BallAngle)) >= 0.7

	local IsPlaying = true
	while App.IsOpened() do
		-- Handle events
		local Event = sfml.Event()
		while App.GetEvent(Event) do
			-- Window closed or escape key pressed : exit
			if Event.Type == sfml.Event.Closed 
			   or ((Event.Type == sfml.Event.KeyPressed) and (Event.Key.Code == sfml.Escape)) then
				App.Close()
				break
			end
		end

		if IsPlaying then
			-- Move the player's paddle
			if App.GetInput().IsKeyDown(sfml.Up) and (LeftPaddle.GetPosition().y > 5.0) then
				LeftPaddle.Move(0.0, -LeftPaddleSpeed * App.GetFrameTime())
			end
			if App.GetInput().IsKeyDown(sfml.Down) and (LeftPaddle.GetPosition().y < App.GetView().GetRect().GetHeight() - LeftPaddle.GetSize().y - 5.0) then
				LeftPaddle.Move(0.0, LeftPaddleSpeed * App.GetFrameTime())
			end

			-- Move the computer's paddle
			if ((RightPaddleSpeed < 0.0) and (RightPaddle.GetPosition().y > 5.0)) or
				((RightPaddleSpeed > 0.0) and (RightPaddle.GetPosition().y < App.GetView().GetRect().GetHeight() - RightPaddle.GetSize().y - 5.0)) then
				RightPaddle.Move(0.0, RightPaddleSpeed * App.GetFrameTime())
			end

			-- Update the computer's paddle direction according to the ball position
			if AITimer.GetElapsedTime() > AITime then
				AITimer.Reset()
				if (RightPaddleSpeed < 0) and (Ball.GetPosition().y + Ball.GetSize().y > RightPaddle.GetPosition().y + RightPaddle.GetSize().y) then
					RightPaddleSpeed = -RightPaddleSpeed
				end
				if (RightPaddleSpeed > 0) and (Ball.GetPosition().y < RightPaddle.GetPosition().y) then
					RightPaddleSpeed = -RightPaddleSpeed
				end
			end

			-- Move the ball
			local Factor = BallSpeed * App.GetFrameTime()
			Ball.Move(math.cos(BallAngle) * Factor, math.sin(BallAngle) * Factor)

			-- Check collisions between the ball and the screen
			if Ball.GetPosition().x < 0.0 then
				IsPlaying = false
				End.SetText("You lost !\n(press escape to exit)")
			end
			if Ball.GetPosition().x + Ball.GetSize().x > App.GetView().GetRect().GetWidth() then
				IsPlaying = false
				End.SetText("You won !\n(press escape to exit)")
			end
			if Ball.GetPosition().y < 0.0 then
				BallSound.Play()
				BallAngle = -BallAngle
				Ball.SetY(0.1)
			end

			if Ball.GetPosition().y + Ball.GetSize().y > App.GetView().GetRect().GetHeight() then
				BallSound.Play()
				BallAngle = -BallAngle
				Ball.SetY(App.GetView().GetRect().GetHeight() - Ball.GetSize().y - 0.1)
			end

			-- Check the collisions between the ball and the paddles
			-- Left Paddle
			if Ball.GetPosition().x < LeftPaddle.GetPosition().x + LeftPaddle.GetSize().x and 
				Ball.GetPosition().x > LeftPaddle.GetPosition().x + (LeftPaddle.GetSize().x / 2.0) and
				Ball.GetPosition().y + Ball.GetSize().y >= LeftPaddle.GetPosition().y and
				Ball.GetPosition().y <= LeftPaddle.GetPosition().y + LeftPaddle.GetSize().y
			then
				BallSound.Play()
				BallAngle = MyPai - BallAngle
				Ball.SetX(LeftPaddle.GetPosition().x + LeftPaddle.GetSize().x + 0.1)
			end

			-- Right Paddle
			if Ball.GetPosition().x + Ball.GetSize().x > RightPaddle.GetPosition().x and
				Ball.GetPosition().x + Ball.GetSize().x < RightPaddle.GetPosition().x + (RightPaddle.GetSize().x / 2.0) and
				Ball.GetPosition().y + Ball.GetSize().y >= RightPaddle.GetPosition().y and
				Ball.GetPosition().y <= RightPaddle.GetPosition().y + RightPaddle.GetSize().y
			then
				BallSound.Play()
				BallAngle = MyPai - BallAngle
				Ball.SetX(RightPaddle.GetPosition().x - Ball.GetSize().x - 0.1)
			end

		end

		-- Clear the window
		App.Clear()

		-- Draw the background, paddles and ball sprites
		App.Draw(Background)
		App.Draw(LeftPaddle)
		App.Draw(RightPaddle)
		App.Draw(Ball)

		-- If the game is over, display the end message
		if not IsPlaying then
			App.Draw(End)
		end

		-- Display things on screen
		App.Display()

	end

end

doMain()
