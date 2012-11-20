def start() :
	driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT :
		return 1;

	device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480));

	if device == None :
		return 1;

	driver = device.getVideoDriver();
	smgr = device.getSceneManager();

	device.getFileSystem().addZipFileArchive("../../media/map-20kdm2.pk3");

	mesh = smgr.getMesh("20kdm2.bsp");
	node = None;

	if mesh != None :
		node = smgr.addOctreeSceneNode(mesh.getMesh(0), None, -1, 1024);

	if node != None :
		node.setPosition(irr.vector3df(-1300,-144,-1249));

	smgr.addCameraSceneNodeFPS();

	device.getCursorControl().setVisible(False);

	lastFPS = -1;

	while device.run() :
		if device.isWindowActive() :
			driver.beginScene(True, True, irr.SColor(255,200,200,200));
			smgr.drawAll();
			driver.endScene();

			fps = driver.getFPS();

			if lastFPS != fps :
				tmp = "cpgf Irrlicht Python Binding Demo - Quake 3 Map example [";
				tmp = tmp + driver.getName();
				tmp = tmp + "] fps: ";
				tmp = tmp + str(fps);

				device.setWindowCaption(tmp);
				lastFPS = fps;

	device.drop();
	return 0;


start();
