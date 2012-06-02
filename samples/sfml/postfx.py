def doMain() :
	# Check that the system can use post effects
	if not sfml.PostFX.CanUsePostFX() :
		return sfml.EXIT_SUCCESS

	# Create the main window
	App = sfml.RenderWindow(sfml.VideoMode(800, 600), "SFML PostFX -- cpgf SFML binding demo -- Python")


	# Load a cute background image to display
	BackgroundImage = sfml.Image()
	if not BackgroundImage.LoadFromFile("datas/post-fx/background.jpg") :
		return sfml.EXIT_FAILURE
	Background = sfml.Sprite(BackgroundImage)

	# Load the text font
	Cheeseburger = sfml.Font()
	if not Cheeseburger.LoadFromFile("datas/post-fx/cheeseburger.ttf") :
		return sfml.EXIT_FAILURE

	# Load the image needed for the wave effect
	WaveImage = sfml.Image()
	if not WaveImage.LoadFromFile("datas/post-fx/wave.jpg") :
		return sfml.EXIT_FAILURE

	# Load all effects
	Effects = {
		'nothing' : sfml.PostFX(),
		'blur' : sfml.PostFX(),
		'colorize' : sfml.PostFX(),
		'fisheye' : sfml.PostFX(),
		'wave' : sfml.PostFX()
	}
	if not Effects["nothing"].LoadFromFile("datas/post-fx/nothing.sfx") :   return sfml.EXIT_FAILURE
	if not Effects["blur"].LoadFromFile("datas/post-fx/blur.sfx") :         return sfml.EXIT_FAILURE
	if not Effects["colorize"].LoadFromFile("datas/post-fx/colorize.sfx") : return sfml.EXIT_FAILURE
	if not Effects["fisheye"].LoadFromFile("datas/post-fx/fisheye.sfx") :   return sfml.EXIT_FAILURE
	if not Effects["wave"].LoadFromFile("datas/post-fx/wave.sfx")       :   return sfml.EXIT_FAILURE

	EffectIndexes = [
		"nothing",
		"blur",
		"colorize",
		"fisheye",
		"wave"
	]

	CurrentEffect = len(EffectIndexes) - 1

	# Do specific initializations
	Effects["nothing"].SetTexture("framebuffer", None)
	Effects["blur"].SetTexture("framebuffer", None)
	Effects["blur"].SetParameter("offset", 0.0)
	Effects["colorize"].SetTexture("framebuffer", None)
	Effects["colorize"].SetParameter("color", 1.0, 1.0, 1.0)
	Effects["fisheye"].SetTexture("framebuffer", None)
	Effects["wave"].SetTexture("framebuffer", None)
	Effects["wave"].SetTexture("wave", WaveImage)

	# Define a string for displaying current effect description
	CurFXStr = sfml.String()
	CurFXStr.SetText("Current effect is \"" + EffectIndexes[CurrentEffect] + "\"")
	CurFXStr.SetFont(Cheeseburger)
	CurFXStr.SetPosition(20.0, 0.0)

	# Define a string for displaying help
	InfoStr = sfml.String()
	InfoStr.SetText("Move your mouse to change the effect parameters\nPress numpad + and - to change effect\nWarning : some effects may not work\ndepending on your graphics card")
	InfoStr.SetFont(Cheeseburger)
	InfoStr.SetPosition(20.0, 460.0)
	InfoStr.SetColor(sfml.Color(200, 100, 150))
	
	# Start the game loop
	while App.IsOpened() :
		# Process events
		Event = sfml.Event()
		while App.GetEvent(Event) :

			# Close window : exit
			if Event.Type == sfml.Event.Closed :
				App.Close()

			if Event.Type == sfml.Event.KeyPressed :
				# Escape key : exit
				if Event.Key.Code == sfml.Escape :
					App.Close()
					break

				# Add key : next effect
				if Event.Key.Code == sfml.Add :
					CurrentEffect = CurrentEffect + 1
					if CurrentEffect >= len(EffectIndexes) :
						CurrentEffect = 0
					CurFXStr.SetText("Current effect is \"" + EffectIndexes[CurrentEffect] + "\"")

				# Subtract key : previous effect
				if Event.Key.Code == sfml.Subtract :
					if CurrentEffect == 0 :
						CurrentEffect = len(EffectIndexes)
					CurrentEffect = CurrentEffect - 1
					CurFXStr.SetText("Current effect is \"" + EffectIndexes[CurrentEffect] + "\"")

			# Get the mouse position in the range [0, 1]
			X = float(App.GetInput().GetMouseX()) / float(App.GetWidth())
			Y = float(App.GetInput().GetMouseY()) / float(App.GetHeight())
			
			EffectName = EffectIndexes[CurrentEffect]

			# Update the current effect
			if EffectName == "blur" :     Effects[EffectName].SetParameter("offset", X * Y * 0.1)
			if EffectName == "colorize" : Effects[EffectName].SetParameter("color", 0.3, X, Y)
			if EffectName == "fisheye" :  Effects[EffectName].SetParameter("mouse", X, 1.0 - Y)
			if EffectName == "wave" :     Effects[EffectName].SetParameter("offset", X, Y)

			# Clear the window
			App.Clear()

			# Draw background and apply the post-fx
			App.Draw(Background)
			App.Draw(Effects[EffectName])
			
			# Draw interface strings
			App.Draw(CurFXStr)
			App.Draw(InfoStr)

			# Finally, display the rendered frame on screen
			App.Display()


	return sfml.EXIT_SUCCESS



doMain()
