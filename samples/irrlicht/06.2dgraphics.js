function start()
{
	// ask user for driver
	var driverType = irr.driverChoiceConsole();
	if(driverType == irr.EDT_COUNT)
		return 1;

	// create device

	var device = irr.createDevice(driverType, new irr.dimension2d_u32(512, 384));

	if (device == null)
		return 1; // could not create selected driver.

	device.setWindowCaption("cpgf Irrlicht JavaScript Binding - 2D Graphics Demo");

	var driver = device.getVideoDriver();

	/*
	All 2d graphics in this example are put together into one texture,
	2ddemo.png. Because we want to draw colorkey based sprites, we need to
	load this texture and tell the engine, which part of it should be
	transparent based on a colorkey.

	In this example, we don't tell it the color directly, we just say "Hey
	Irrlicht Engine, you'll find the color I want at position (0,0) on the
	texture.". Instead, it would be also possible to call
	driver.makeColorKeyTexture(images, video::SColor(0,0,0,0)), to make
	e.g. all black pixels transparent. Please note that
	makeColorKeyTexture just creates an alpha channel based on the color.
	*/
	var images = driver.getTexture("../../media/2ddemo.png");
	driver.makeColorKeyTexture(images, new irr.position2d_s32(0,0));

	/*
	To be able to draw some text with two different fonts, we first load
	them. Ok, we load just one. As the first font we just use the default
	font which is built into the engine. Also, we define two rectangles
	which specify the position of the images of the red imps (little flying
	creatures) in the texture.
	*/
	var font = device.getGUIEnvironment().getBuiltInFont();
	var font2 =	device.getGUIEnvironment().getFont("../../media/fonthaettenschweiler.bmp");

	var imp1 = new irr.rect_s32(349,15,385,78);
	var imp2 = new irr.rect_s32(387,15,423,78);

	/*
	Prepare a nicely filtering 2d render mode for special cases.
	*/
//	driver.getMaterial2D().TextureLayer[0].BilinearFilter=true;
	driver.getMaterial2D().AntiAliasing=irr.EAAM_FULL_BASIC;

	/*
	Everything is prepared, now we can draw everything in the draw loop,
	between the begin scene and end scene calls. In this example, we are
	just doing 2d graphics, but it would be no problem to mix them with 3d
	graphics. Just try it out, and draw some 3d vertices or set up a scene
	with the scene manager and draw it.
	*/
	while(device.run() && driver)
	{
		if (device.isWindowActive())
		{
			var time = device.getTimer().getTime();

			driver.beginScene(true, true, new irr.SColor(255,120,102,136));

			/*
			First, we draw 3 sprites, using the alpha channel we
			created with makeColorKeyTexture. The last parameter
			specifies that the drawing method should use this alpha
			channel. The last-but-one parameter specifies a
			color, with which the sprite should be colored.
			(255,255,255,255) is full white, so the sprite will
			look like the original. The third sprite is drawn
			with the red channel modulated based on the time.
			*/

			// draw fire & dragons background world
			driver.draw2DImage(images, new irr.position2d_s32(50,50), new irr.rect_s32(0,0,342,224), 0, new irr.SColor(255,255,255,255), true);

			// draw flying imp
			driver.draw2DImage(images, new irr.position2d_s32(164,125),	(time/500 % 2) ? imp1 : imp2, 0, new irr.SColor(255,255,255,255), true);

			// draw second flying imp with colorcylce
			driver.draw2DImage(images, new irr.position2d_s32(270,105),	(time/500 % 2) ? imp1 : imp2, 0, new irr.SColor(255,(time) % 255,255,255), true);

			/*
			Drawing text is really simple. The code should be self
			explanatory.
			*/

			// draw some text
			if (font)
				font.draw("This demo shows that Irrlicht is also capable of drawing 2D graphics.", new irr.rect_s32(130,10,300,50), new irr.SColor(255,255,255,255));

			// draw some other text
			if (font2)
				font2.draw("Also mixing with 3d graphics is possible.", new irr.rect_s32(130,20,300,60), new irr.SColor(255,time % 255,time % 255,255));

			/*
			Next, we draw the Irrlicht Engine logo (without
			using a color or an alpha channel). Since we slightly scale
			the image we use the prepared filter mode.
			*/
			driver.enableMaterial2D();
			driver.draw2DImage(images, new irr.rect_s32(10,10,108,48), new irr.rect_s32(354,87,442,118));
			driver.enableMaterial2D(false);

			/*
			Finally draw a half-transparent rect under the mouse cursor.
			*/
			var m = device.getCursorControl().getPosition();
			driver.draw2DRectangle(new irr.SColor(100,255,255,255), new irr.rect_s32(m.X-20, m.Y-20, m.X+20, m.Y+20));

			driver.endScene();
		}
	}

	device.drop();

	return 0;
}

start();
