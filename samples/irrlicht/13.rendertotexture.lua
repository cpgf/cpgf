function start()
	local driverType=irr.driverChoiceConsole();
	if driverType==irr.EDT_COUNT then
		return 1;
	end

	local device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480), 16, false, false);

	if device == nil then
		return 1;
	end

	local driver = device.getVideoDriver();
	local smgr = device.getSceneManager();
	local env = device.getGUIEnvironment();
	

	local fairy = smgr.addAnimatedMeshSceneNode(smgr.getMesh("../../media/faerie.md2"));

	if fairy then
		fairy.setMaterialTexture(0,	driver.getTexture("../../media/faerie2.bmp"));
		fairy.setMaterialFlag(irr.EMF_LIGHTING, true);
		fairy.getMaterial(0).Shininess = 20.0;
		fairy.setPosition(irr.vector3df(-10,0,-100));
		fairy.setMD2Animation ( irr.EMAT_STAND );
	end
	
	smgr.addLightSceneNode(nil, irr.vector3df(-15,5,-105), irr.SColorf(1.0, 1.0, 1.0));

	smgr.setAmbientLight(irr.SColor(0,60,60,60));
	
	local test = smgr.addCubeSceneNode(60);

	local anim = smgr.createRotationAnimator(irr.vector3df(0.3, 0.3,0));

	test.setPosition(irr.vector3df(-100,0,-100));
	test.setMaterialFlag(irr.EMF_LIGHTING, false);
	test.addAnimator(anim);
	anim.drop();

	device.setWindowCaption("Irrlicht Engine - Render to Texture and Specular Highlights example");
	
	local rt = nil;
	local fixedCam = nil;

	if driver.queryFeature(irr.EVDF_RENDER_TO_TARGET) then
		rt = driver.addRenderTargetTexture(irr.dimension2d_u32(256,256), "RTT1");
		test.setMaterialTexture(0, rt);

		fixedCam = smgr.addCameraSceneNode(nil, irr.vector3df(10,10,-80), irr.vector3df(-10,10,-100));
	else
		local skin = env.getSkin();
		local font = env.getFont("../../media/fonthaettenschweiler.bmp");
		if font then
			skin.setFont(font);
		end

		local text = env.addStaticText("Your hardware or this renderer is not able to use the render to texture feature. RTT Disabled.", irr.rect_s32(150,20,470,60));

		text.setOverrideColor(irr.SColor(100,255,255,255));
	end
	
	local fpsCamera = smgr.addCameraSceneNodeFPS();
	fpsCamera.setPosition(irr.vector3df(-50,50,-150));

	device.getCursorControl().setVisible(false);

	local lastFPS = -1;

	while device.run() do
		if device.isWindowActive() then
			driver.beginScene(true, true, 0);

			if rt then
				
				driver.setRenderTarget(rt, true, true, irr.SColor(0,0,0,255));

				test.setVisible(false);
				smgr.setActiveCamera(fixedCam);

				smgr.drawAll();

				driver.setRenderTarget(0, true, true, 0);

				test.setVisible(true);
				smgr.setActiveCamera(fpsCamera);
			end
			
			smgr.drawAll();
			env.drawAll();

			driver.endScene();

			local fps = driver.getFPS();
			if lastFPS ~= fps then
				local tmp = "cpgf Irrlicht Lua Binding - Render to Texture and Specular Highlights example";
				tmp = tmp .. " FPS:";
				tmp = tmp .. fps;

				device.setWindowCaption(tmp);
				lastFPS = fps;
			end
		end
	end

	device.drop();
	return 0;
end

start()
