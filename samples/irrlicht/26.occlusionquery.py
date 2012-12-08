cpgf._import(None, "builtin.core");

KeyIsDown = [];

def makeMyEventReceiver(receiver) :
	for i in range(irr.KEY_KEY_CODES_COUNT) :
		KeyIsDown.append(False);

	def OnEvent(me, event) :
		if event.EventType == irr.EET_KEY_INPUT_EVENT :
			KeyIsDown[event.KeyInput.Key + 1] = event.KeyInput.PressedDown;
		return False;

	receiver.OnEvent = OnEvent;


def IsKeyDown(keyCode) :
	return KeyIsDown[keyCode + 1];


def start() :
	driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT :
		return 1;

	MyEventReceiver = cpgf.cloneClass(irr.IEventReceiverWrapper);
	makeMyEventReceiver(MyEventReceiver);
	receiver = MyEventReceiver();

	device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480), 16, False, False, False, receiver);

	if device == None :
		return 1;

	driver = device.getVideoDriver();
	smgr = device.getSceneManager();

	smgr.getGUIEnvironment().addStaticText("Press Space to hide occluder.", irr.rect_s32(10,10, 200,50));

	node = smgr.addSphereSceneNode(10, 64);
	if node :
		node.setPosition(irr.vector3df(0,0,60));
		node.setMaterialTexture(0, driver.getTexture("../../media/wall.bmp"));
		node.setMaterialFlag(irr.EMF_LIGHTING, False);

	plane = smgr.addMeshSceneNode(smgr.addHillPlaneMesh("plane", irr.dimension2df(10,10), irr.dimension2du(2,2)), None, -1, irr.vector3df(0,0,20), irr.vector3df(270,0,0));

	if plane :
		plane.setMaterialTexture(0, driver.getTexture("../../media/t351sml.jpg"));
		plane.setMaterialFlag(irr.EMF_LIGHTING, False);
		plane.setMaterialFlag(irr.EMF_BACK_FACE_CULLING, True);

	driver.addOcclusionQuery(node, cpgf.cast(node, irr.IMeshSceneNode).getMesh());

	smgr.addCameraSceneNode();
	lastFPS = -1;
	timeNow = device.getTimer().getTime();
	nodeVisible=True;

	while device.run() :
		plane.setVisible(not IsKeyDown(irr.KEY_SPACE));

		driver.beginScene(True, True, irr.SColor(255,113,113,133));
		node.setVisible(nodeVisible);
		smgr.drawAll();
		smgr.getGUIEnvironment().drawAll();

		if device.getTimer().getTime()-timeNow>100 :
			driver.runAllOcclusionQueries(False);
			driver.updateAllOcclusionQueries();
			nodeVisible=driver.getOcclusionQueryResult(node)>0;
			timeNow=device.getTimer().getTime();

		driver.endScene();

		fps = driver.getFPS();

		if lastFPS != fps :
			tmp = "cpgf Irrlicht Python binding OcclusionQuery Example [";
			tmp = tmp + driver.getName();
			tmp = tmp + "] fps: ";
			tmp = tmp + str(fps);

			device.setWindowCaption(tmp);
			lastFPS = fps;

	device.drop();
	
	return 0;

start();
