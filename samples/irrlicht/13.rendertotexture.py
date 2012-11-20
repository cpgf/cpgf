def start() :
	driverType=irr.driverChoiceConsole();
	if driverType==irr.EDT_COUNT :
		return 1;

	device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480), 16, False, False);

	if device == None :
		return 1;

	driver = device.getVideoDriver();
	smgr = device.getSceneManager();
	env = device.getGUIEnvironment();
	

	fairy = smgr.addAnimatedMeshSceneNode(smgr.getMesh("../../media/faerie.md2"));

	if fairy :
		fairy.setMaterialTexture(0,	driver.getTexture("../../media/faerie2.bmp"));
		fairy.setMaterialFlag(irr.EMF_LIGHTING, True);
		fairy.getMaterial(0).Shininess = 20.0;
		fairy.setPosition(irr.vector3df(-10,0,-100));
		fairy.setMD2Animation ( irr.EMAT_STAND );
	
	smgr.addLightSceneNode(None, irr.vector3df(-15,5,-105), irr.SColorf(1.0, 1.0, 1.0));

	smgr.setAmbientLight(irr.SColor(0,60,60,60));
	
	test = smgr.addCubeSceneNode(60);

	anim = smgr.createRotationAnimator(irr.vector3df(0.3, 0.3,0));

	test.setPosition(irr.vector3df(-100,0,-100));
	test.setMaterialFlag(irr.EMF_LIGHTING, False);
	test.addAnimator(anim);
	anim.drop();

	device.setWindowCaption("Irrlicht Engine - Render to Texture and Specular Highlights example");
	
	rt = None;
	fixedCam = None;

	if driver.queryFeature(irr.EVDF_RENDER_TO_TARGET) :
		rt = driver.addRenderTargetTexture(irr.dimension2d_u32(256,256), "RTT1");
		test.setMaterialTexture(0, rt);

		fixedCam = smgr.addCameraSceneNode(None, irr.vector3df(10,10,-80), irr.vector3df(-10,10,-100));
	else :
		skin = env.getSkin();
		font = env.getFont("../../media/fonthaettenschweiler.bmp");
		if font :
			skin.setFont(font);

		text = env.addStaticText("Your hardware or this renderer is not able to use the render to texture feature. RTT Disabled.", irr.rect_s32(150,20,470,60));

		text.setOverrideColor(irr.SColor(100,255,255,255));
	
	fpsCamera = smgr.addCameraSceneNodeFPS();
	fpsCamera.setPosition(irr.vector3df(-50,50,-150));

	device.getCursorControl().setVisible(False);

	lastFPS = -1;

	while device.run() :
		if device.isWindowActive() :
			driver.beginScene(True, True, 0);

			if rt :
				
				driver.setRenderTarget(rt, True, True, irr.SColor(0,0,0,255));

				test.setVisible(False);
				smgr.setActiveCamera(fixedCam);

				smgr.drawAll();

				driver.setRenderTarget(0, True, True, 0);

				test.setVisible(True);
				smgr.setActiveCamera(fpsCamera);
			
			smgr.drawAll();
			env.drawAll();

			driver.endScene();

			fps = driver.getFPS();
			if lastFPS != fps :
				tmp = "cpgf Irrlicht Python Binding - Render to Texture and Specular Highlights example";
				tmp = tmp + " FPS:";
				tmp = tmp + str(fps);

				device.setWindowCaption(tmp);
				lastFPS = fps;

	device.drop();
	return 0;

start()
