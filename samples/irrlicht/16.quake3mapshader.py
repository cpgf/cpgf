cpgf._import(None, "builtin.core");
cpgf._import(None, "builtin.collections.bytearray");
cpgf._import(None, "builtin.debug");

def overrideScreenShotFactory(screenshotFactory, device, templateName, node) :
	Device = device;
	Number = 0;
	FilenameTemplate = templateName;
	Node = node;
	
	def OnEvent(me, event) :
		if event.EventType == irr.EET_KEY_INPUT_EVENT and event.KeyInput.PressedDown :
			if event.KeyInput.Key == irr.KEY_F9 :
				image = Device.getVideoDriver().createScreenShot();
				if image :
					Number = Number + 1;
					buf = FilenameTemplate + "_shot" + Number + ".jpg";
					Device.getVideoDriver().writeImageToFile(image, buf, 85 );
					image.drop();
			elif event.KeyInput.Key == irr.KEY_F8 :
				if Node.isDebugDataVisible() :
					Node.setDebugDataVisible(irr.EDS_OFF);
				else :
					Node.setDebugDataVisible(irr.EDS_BBOX_ALL);
		return False;

	screenshotFactory.OnEvent = OnEvent;

def start() :
	driverType=irr.driverChoiceConsole();
	if driverType==irr.EDT_COUNT :
		return 1;

	videoDim = irr.dimension2du(800,600);

	device = irr.createDevice(driverType, videoDim, 32, False );

	if device == None :
		return 1;

	mapname = "maps/20kdm2.bsp";

	driver = device.getVideoDriver();
	smgr = device.getSceneManager();
	gui = device.getGUIEnvironment();

	device.getFileSystem().addFolderFileArchive("../../media/");
	device.getFileSystem().addZipFileArchive("../../media/map-20kdm2.pk3");

	smgr.getParameters().setAttribute(irr.ALLOW_ZWRITE_ON_TRANSPARENT, True);

	mesh = cpgf.cast(smgr.getMesh(mapname), irr.IQ3LevelMesh);

	node = None;
	if mesh :
		geometry = mesh.getMesh(irr.E_Q3_MESH_GEOMETRY);
		node = smgr.addOctreeSceneNode(geometry, None, -1, 4096);

	screenshotFactory = irr.IEventReceiverWrapper();
	overrideScreenShotFactory(screenshotFactory, device, mapname, node);
	device.setEventReceiver(screenshotFactory);

	if  mesh  :
		additional_mesh = mesh.getMesh(irr.E_Q3_MESH_ITEMS);

		for i in range(additional_mesh.getMeshBufferCount()) :
			meshBuffer = additional_mesh.getMeshBuffer(i);
			material = meshBuffer.getMaterial();

			shaderIndex = material.MaterialTypeParam2;

			shader = mesh.getShader(shaderIndex);
			if shader != None :
				node = smgr.addQuake3SceneNode(meshBuffer, shader);

	camera = smgr.addCameraSceneNodeFPS();

	if mesh :
		entityList = mesh.getEntityList();

		se = irr.IShader();
		se.name = "info_player_deathmatch";

		index = entityList.binary_search(se);
		if index >= 0 :
			notEndList = 1;
			while notEndList :
				group = entityList._opArrayGet(index).getGroup(1);

				parsepos = cpgf.createByteArray(8);
				parsepos.writeInt32(0);
				pos = irr.getAsVector3df(group.get("origin"), parsepos.getPointer());

				parsepos.setPosition(0);
				parsepos.writeInt32(0);
				angle = irr.getAsFloat(group.get("angle"), parsepos.getPointer());

				target = irr.vector3df(0.0, 0.0, 1.0);
				target.rotateXZBy(angle);

				camera.setPosition(pos);
				camera.setTarget(pos + target);

				index = index + 1;
				notEndList = (index == 2);
	
	device.getCursorControl().setVisible(False);

	gui.addImage(driver.getTexture("irrlichtlogo2.png"), irr.position2d_s32(10, 10));

	pos = irr.position2di(videoDim.Width - 128, videoDim.Height - 64);

	if driverType == irr.EDT_BURNINGSVIDEO :
		gui.addImage(driver.getTexture("burninglogo.png"), pos);

	elif driverType == irr.EDT_OPENGL :
		gui.addImage(driver.getTexture("opengllogo.png"), pos);

	elif driverType == irr.EDT_DIRECT3D8 or driverType == irr.EDT_DIRECT3D9 :
		gui.addImage(driver.getTexture("directxlogo.png"), pos);

	lastFPS = -1;

	while device.run() :
		if device.isWindowActive() :
			driver.beginScene(True, True, irr.SColor(255,20,20,40));
			smgr.drawAll();
			gui.drawAll();
			driver.endScene();

			fps = driver.getFPS();
			attr = smgr.getParameters();
			tmp = "Q3 [";
			tmp = tmp + driver.getName();
			tmp = tmp + "] FPS:";
			tmp = tmp + str(fps);
			tmp = tmp + " Cull:";
			tmp = tmp + str(attr.getAttributeAsInt("calls"));
			tmp = tmp + "/";
			tmp = tmp + str(attr.getAttributeAsInt("culled"));
			tmp = tmp + " Draw: ";
			tmp = tmp + str(attr.getAttributeAsInt("drawn_solid"));
			tmp = tmp + "/";
			tmp = tmp + str(attr.getAttributeAsInt("drawn_transparent"));
			tmp = tmp + "/";
			tmp = tmp + str(attr.getAttributeAsInt("drawn_transparent_effect"));

			device.setWindowCaption(tmp);
			lastFPS = fps;

	device.drop();

	return 0;

start();
