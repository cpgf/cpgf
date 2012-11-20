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

cpgf._import(None, "builtin.core");

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

	node = smgr.addSphereSceneNode();
	if node :

		node.setPosition(irr.vector3df(0, 0, 30));
		node.setMaterialTexture(0, driver.getTexture("../../media/wall.bmp"));
		node.setMaterialFlag(irr.EMF_LIGHTING, False);


	n = smgr.addCubeSceneNode();

	if n :
		n.setMaterialTexture(0, driver.getTexture("../../media/t351sml.jpg"));
		n.setMaterialFlag(irr.EMF_LIGHTING, False);
		anim = smgr.createFlyCircleAnimator(irr.vector3df(0,0,30), 20.0);
		if anim :
			n.addAnimator(anim);
			anim.drop();



	anms = smgr.addAnimatedMeshSceneNode(smgr.getMesh("../../media/ninja.b3d"));

	if anms :
		anim = smgr.createFlyStraightAnimator(irr.vector3df(100,0,60), irr.vector3df(-100,0,60), 3500, True);
		if anim :
			anms.addAnimator(anim);
			anim.drop();

		anms.setMaterialFlag(irr.EMF_LIGHTING, False);

		anms.setFrameLoop(0, 13);
		anms.setAnimationSpeed(15);

		anms.setScale(irr.vector3df(2.0,2.0,2.0));
		anms.setRotation(irr.vector3df(0,-90,0));



	smgr.addCameraSceneNodeFPS();
	device.getCursorControl().setVisible(False);

	device.getGUIEnvironment().addImage(driver.getTexture("../../media/irrlichtlogoalpha2.tga"), irr.position2d_s32(10,20));

	diagnostics = device.getGUIEnvironment().addStaticText("", irr.rect_s32(10, 10, 400, 20));
	diagnostics.setOverrideColor(irr.SColor(255, 255, 255, 0));

	lastFPS = -1;
	t = device.getTimer().getTime();
	MOVEMENT_SPEED = 5.0;

	while device.run() :
		now = device.getTimer().getTime();
		frameDeltaTime = (now - t) / 1000.0;
		t = now;

		nodePosition = node.getPosition();

		if IsKeyDown(irr.KEY_KEY_W) :
			nodePosition.Y = nodePosition.Y + MOVEMENT_SPEED * frameDeltaTime;
		elif IsKeyDown(irr.KEY_KEY_S) :
			nodePosition.Y = nodePosition.Y - MOVEMENT_SPEED * frameDeltaTime;


		if IsKeyDown(irr.KEY_KEY_A) :
			nodePosition.X = nodePosition.X - MOVEMENT_SPEED * frameDeltaTime;
		elif IsKeyDown(irr.KEY_KEY_D) :
			nodePosition.X = nodePosition.X + MOVEMENT_SPEED * frameDeltaTime;


		node.setPosition(nodePosition);

		driver.beginScene(True, True, irr.SColor(255,113,113,133));

		smgr.drawAll();
		device.getGUIEnvironment().drawAll();

		driver.endScene();

		fps = driver.getFPS();

		if lastFPS != fps :
			tmp = "Movement Example - cpgf Irrlicht Python Binding Demo [";
			tmp = tmp + driver.getName();
			tmp = tmp + "] fps: ";
			tmp = tmp + str(fps);

			device.setWindowCaption(tmp);
			lastFPS = fps;


	
	device.drop();
	
	return 0;


start();
