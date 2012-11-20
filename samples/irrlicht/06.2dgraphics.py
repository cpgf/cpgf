def start() :
	driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT :
		return 1;

	device = irr.createDevice(driverType, irr.dimension2d_u32(512, 384));

	if device == None :
		return 1;

	device.setWindowCaption("cpgf Irrlicht Python Binding - 2D Graphics Demo");

	driver = device.getVideoDriver();

	images = driver.getTexture("../../media/2ddemo.png");
	driver.makeColorKeyTexture(images, irr.position2d_s32(0,0));

	font = device.getGUIEnvironment().getBuiltInFont();
	font2 =	device.getGUIEnvironment().getFont("../../media/fonthaettenschweiler.bmp");

	imp1 = irr.rect_s32(349,15,385,78);
	imp2 = irr.rect_s32(387,15,423,78);

	driver.getMaterial2D().AntiAliasing=irr.EAAM_FULL_BASIC;

	while device.run() and driver :
		if device.isWindowActive() :
			time = device.getTimer().getTime();

			driver.beginScene(True, True, irr.SColor(255,120,102,136));

			driver.draw2DImage(images, irr.position2d_s32(50,50), irr.rect_s32(0,0,342,224), None, irr.SColor(255,255,255,255), True);
			driver.draw2DImage(images, irr.position2d_s32(164,125),	(time/500 % 2) and imp1 or imp2, None, irr.SColor(255,255,255,255), True);
			driver.draw2DImage(images, irr.position2d_s32(270,105),	(time/500 % 2) and imp1 or imp2, None, irr.SColor(255,(time) % 255,255,255), True);

			if font :
				font.draw("This demo shows that Irrlicht is also capable of drawing 2D graphics.", irr.rect_s32(130,10,300,50), irr.SColor(255,255,255,255));


			if font2 :
				font2.draw("Also mixing with 3d graphics is possible.", irr.rect_s32(130,20,300,60), irr.SColor(255,time % 255,time % 255,255));


			driver.enableMaterial2D();
			driver.draw2DImage(images, irr.rect_s32(10,10,108,48), irr.rect_s32(354,87,442,118));
			driver.enableMaterial2D(False);

			m = device.getCursorControl().getPosition();
			driver.draw2DRectangle(irr.SColor(100,255,255,255), irr.rect_s32(m.X-20, m.Y-20, m.X+20, m.Y+20));

			driver.endScene();

	return 0;

	device.drop();

start();
