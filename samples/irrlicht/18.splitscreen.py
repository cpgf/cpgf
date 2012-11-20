ResX=800;
ResY=600;
fullScreen=False;

SplitScreen=True;

camera= [ None, None, None, None ];

def overrideEventReceiver(receiver) :
	def OnEvent(me, event) :
		global SplitScreen;
		if event.EventType == irr.EET_KEY_INPUT_EVENT and	event.KeyInput.Key == irr.KEY_KEY_S and event.KeyInput.PressedDown :
			SplitScreen = not SplitScreen;
			return True;
		if camera[3] :
			return camera[3].OnEvent(event);
		return False;

	receiver.OnEvent = OnEvent;
	
def start() :
	driverType=irr.driverChoiceConsole();
	if driverType==irr.EDT_COUNT :
		return 1;

	receiver = irr.IEventReceiverWrapper();
	overrideEventReceiver(receiver);

	device = irr.createDevice(driverType, irr.dimension2du(ResX,ResY), 32, fullScreen, False, False, receiver);
	if device == None :
		return 1;

	smgr = device.getSceneManager();
	driver = device.getVideoDriver();

	model = smgr.getMesh("../../media/sydney.md2");
	if model == None :
		return 1;
	
	model_node = smgr.addAnimatedMeshSceneNode(model);
	if model_node :
		texture = driver.getTexture("../../media/sydney.bmp");
		model_node.setMaterialTexture(0,texture);
		model_node.setMD2Animation(irr.EMAT_RUN);
		model_node.setMaterialFlag(irr.EMF_LIGHTING,False);

	device.getFileSystem().addZipFileArchive("../../media/map-20kdm2.pk3");
	map = smgr.getMesh("20kdm2.bsp");
	if map :
		map_node = smgr.addOctreeSceneNode(map.getMesh(0));
		map_node.setPosition(irr.vector3df(-850,-220,-850));

	camera[0] = smgr.addCameraSceneNode(None, irr.vector3df(50,0,0), irr.vector3df(0,0,0));
	camera[1] = smgr.addCameraSceneNode(None, irr.vector3df(0,50,0), irr.vector3df(0,0,0));
	camera[2] = smgr.addCameraSceneNode(None, irr.vector3df(0,0,50), irr.vector3df(0,0,0));
	camera[3] = smgr.addCameraSceneNodeFPS();
	if camera[3] :
		camera[3].setPosition(irr.vector3df(-50,0,-50));

	device.getCursorControl().setVisible(False);
	lastFPS = -1;

	while device.run() :
		driver.setViewPort(irr.rect_s32(0,0,ResX,ResY));
		driver.beginScene(True,True,irr.SColor(255,100,100,100));
		if SplitScreen :
			smgr.setActiveCamera(camera[0]);
			driver.setViewPort(irr.rect_s32(0,0,ResX/2,ResY/2));
			smgr.drawAll();
			smgr.setActiveCamera(camera[1]);
			driver.setViewPort(irr.rect_s32(ResX/2,0,ResX,ResY/2));
			smgr.drawAll();
			smgr.setActiveCamera(camera[2]);
			driver.setViewPort(irr.rect_s32(0,ResY/2,ResX/2,ResY));
			smgr.drawAll();
			driver.setViewPort(irr.rect_s32(ResX/2,ResY/2,ResX,ResY));
		smgr.setActiveCamera(camera[3]);
		smgr.drawAll();
		driver.endScene();

		if driver.getFPS() != lastFPS :
			lastFPS = driver.getFPS();
			tmp = "cpgf Irrlicht Python Binding SplitScreen-Example (FPS: ";
			tmp = tmp + str(lastFPS);
			tmp = tmp + ")";
			device.setWindowCaption(tmp);
	device.drop();
	return 0;

start();
