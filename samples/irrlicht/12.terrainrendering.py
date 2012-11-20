showBox = True;

def overrideEventReceiver(receiver, terrain, skybox, skydome) :
	Terrain = terrain;
	Skybox = skybox;
	Skydome = skydome;
	
	Skybox.setVisible(True);
	Skydome.setVisible(False);

	def OnEvent(me, event) :
		global showBox;
		if event.EventType == irr.EET_KEY_INPUT_EVENT and not event.KeyInput.PressedDown :
			if event.KeyInput.Key == irr.KEY_KEY_W :
				Terrain.setMaterialFlag(irr.EMF_WIREFRAME, not Terrain.getMaterial(0).Wireframe);
				Terrain.setMaterialFlag(irr.EMF_POINTCLOUD, False);
				return True;
				
			elif event.KeyInput.Key == irr.KEY_KEY_P :
				Terrain.setMaterialFlag(irr.EMF_POINTCLOUD, not Terrain.getMaterial(0).PointCloud);
				Terrain.setMaterialFlag(irr.EMF_WIREFRAME, False);
				return True;
				
			elif event.KeyInput.Key == irr.KEY_KEY_D :
				if Terrain.getMaterial(0).MaterialType == irr.EMT_SOLID :
					Terrain.setMaterialType(irr.EMT_DETAIL_MAP);
				else :
					Terrain.setMaterialType(irr.EMT_SOLID);
				return True;
				
			elif event.KeyInput.Key == irr.KEY_KEY_S :
				showBox = not showBox;
				Skybox.setVisible(showBox);
				Skydome.setVisible(not showBox);
				return True;
		return False;
	
	receiver.OnEvent = OnEvent;

def start() :
	driverType=irr.driverChoiceConsole();
	if driverType==irr.EDT_COUNT :
		return 1;

	params = irr.SIrrlichtCreationParameters();
	params.DriverType=driverType;
	params.WindowSize=irr.dimension2d_u32(640, 480);
	device = irr.createDeviceEx(params);

	if device == 0 :
		return 1;

	driver = device.getVideoDriver();
	smgr = device.getSceneManager();
	env = device.getGUIEnvironment();

	driver.setTextureCreationFlag(irr.ETCF_ALWAYS_32_BIT, True);

	env.addImage(driver.getTexture("../../media/irrlichtlogo2.png"), irr.position2d_s32(10,10));

	env.getSkin().setFont(env.getFont("../../media/fontlucida.png"));

	env.addStaticText("Press 'W' to change wireframe mode\nPress 'D' to toggle detail map\nPress 'S' to toggle skybox/skydome",	irr.rect_s32(10,421,250,475), True, True, None, -1, True);

	camera = smgr.addCameraSceneNodeFPS(None, 100.0,1.2);

	camera.setPosition(irr.vector3df(2700*2,255*2,2600*2));
	camera.setTarget(irr.vector3df(2397*2,343*2,2700*2));
	camera.setFarValue(42000.0);

	device.getCursorControl().setVisible(False);

	terrain = smgr.addTerrainSceneNode("../../media/terrain-heightmap.bmp", None, -1, irr.vector3df(0.0, 0.0, 0.0), irr.vector3df(0.0, 0.0, 0.0), irr.vector3df(40.0, 4.40, 40.0), irr.SColor ( 255, 255, 255, 255 ), 5, irr.ETPS_17,4);

	terrain.setMaterialFlag(irr.EMF_LIGHTING, False);

	terrain.setMaterialTexture(0, driver.getTexture("../../media/terrain-texture.jpg"));
	terrain.setMaterialTexture(1, driver.getTexture("../../media/detailmap3.jpg"));
	
	terrain.setMaterialType(irr.EMT_DETAIL_MAP);

	terrain.scaleTexture(1.0, 20.0);

	selector = smgr.createTerrainTriangleSelector(terrain, 0);
	terrain.setTriangleSelector(selector);

	anim = smgr.createCollisionResponseAnimator(selector, camera, irr.vector3df(60,100,60),	irr.vector3df(0,0,0), irr.vector3df(0,50,0));
	selector.drop();
	camera.addAnimator(anim);
	anim.drop();

	buffer = irr.CDynamicMeshBuffer(irr.EVT_2TCOORDS, irr.EIT_16BIT);
	terrain.getMeshBufferForLOD(buffer, 0);
	data = buffer.getVertexBuffer().getData();
	#buffer.drop();

	driver.setTextureCreationFlag(irr.ETCF_CREATE_MIP_MAPS, False);

	skybox=smgr.addSkyBoxSceneNode(driver.getTexture("../../media/irrlicht2_up.jpg"), driver.getTexture("../../media/irrlicht2_dn.jpg"), driver.getTexture("../../media/irrlicht2_lf.jpg"), driver.getTexture("../../media/irrlicht2_rt.jpg"), driver.getTexture("../../media/irrlicht2_ft.jpg"), driver.getTexture("../../media/irrlicht2_bk.jpg"));
	skydome=smgr.addSkyDomeSceneNode(driver.getTexture("../../media/skydome.jpg"),16,8,0.95,2.0);

	driver.setTextureCreationFlag(irr.ETCF_CREATE_MIP_MAPS, True);

	receiver = irr.IEventReceiverWrapper();
	overrideEventReceiver(receiver, terrain, skybox, skydome);
	device.setEventReceiver(receiver);

	lastFPS = -1;

	while device.run() :
		if device.isWindowActive() :
			driver.beginScene(True, True, 0 );

			smgr.drawAll();
			env.drawAll();

			driver.endScene();

			fps = driver.getFPS();
			if lastFPS != fps :
				tmp = "Terrain Renderer - cpgf Irrlicht Python Binding [";
				tmp = tmp + driver.getName();
				tmp = tmp + "] FPS:";
				tmp = tmp + str(fps);
				tmp = tmp + " Height: ";
				tmp = tmp + str(terrain.getHeight(camera.getAbsolutePosition().X, camera.getAbsolutePosition().Z));

				device.setWindowCaption(tmp);
				lastFPS = fps;

	device.drop();
	
	return 0;

start();
