cpgf._import(None, "builtin.debug");

class SMouseState:
	Position = irr.position2di();
	LeftButtonDown = False;

JoystickState = irr.SEvent.SJoystickEvent();
MouseState = SMouseState();

def GetJoystickState() :
	return JoystickState;

def GetMouseState() :
	return MouseState;

def overrideEventReceiver(receiver) :
	def OnEvent(me, event) :
		if event.EventType == irr.EET_MOUSE_INPUT_EVENT :
			e = event.MouseInput.Event;
			if e == irr.EMIE_LMOUSE_PRESSED_DOWN :
				MouseState.LeftButtonDown = True;
			
			elif e == irr.EMIE_LMOUSE_LEFT_UP :
				MouseState.LeftButtonDown = False;

			elif e == irr.EMIE_MOUSE_MOVED :
				MouseState.Position.X = event.MouseInput.X;
				MouseState.Position.Y = event.MouseInput.Y;

		if event.EventType == irr.EET_JOYSTICK_INPUT_EVENT	and event.JoystickEvent.Joystick == 0 :
			JoystickState = event.JoystickEvent;

		return False;

	receiver.OnEvent = OnEvent;


def start() :
	driverType=irr.driverChoiceConsole();
	if driverType==irr.EDT_COUNT :
		return 1;

	receiver = irr.IEventReceiverWrapper();
	overrideEventReceiver(receiver);

	device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480), 16, False, False, False, receiver);

	if device == None :
		return 1;

	tmp = "cpgf Irrlicht Python Binding Joystick Example";
	device.setWindowCaption(tmp);

	driver = device.getVideoDriver();
	smgr = device.getSceneManager();

	node = smgr.addMeshSceneNode(smgr.addArrowMesh( "Arrow", irr.SColor(255, 255, 0, 0), irr.SColor(255, 0, 255, 0), 16,16, 2.0, 1.3, 0.1, 0.6));
	node.setMaterialFlag(irr.EMF_LIGHTING, False);

	camera = smgr.addCameraSceneNode();
	camera.setPosition(irr.vector3df(0, 0, -10));

	t = device.getTimer().getTime();
	MOVEMENT_SPEED = 5.0;

	while device.run() :
		now = device.getTimer().getTime();
		frameDeltaTime = (now - t) / 1000.0;
		t = now;

		movedWithJoystick = False;
		nodePosition = irr.vector3df(node.getPosition());

		if not movedWithJoystick :
			ray = smgr.getSceneCollisionManager().getRayFromScreenCoordinates(GetMouseState().Position, camera);

			plane = irr.plane3df(nodePosition, irr.vector3df(0, 0, -1));
			mousePosition = irr.vector3df();
			if plane.getIntersectionWithLine(ray.start, ray.getVector(), mousePosition) :
				toMousePosition = irr.vector3df(mousePosition);
				toMousePosition.X = toMousePosition.X - nodePosition.X;
				toMousePosition.Y = toMousePosition.Y - nodePosition.Y;
				toMousePosition.Z = toMousePosition.Z - nodePosition.Z;
				
				availableMovement = MOVEMENT_SPEED * frameDeltaTime;
				
				if toMousePosition.getLength() <= availableMovement :
					nodePosition = mousePosition;
				else :
					v = toMousePosition.normalize();
					v.X = v.X * availableMovement;
					v.Y = v.Y * availableMovement;
					v.Z = v.Z * availableMovement;
					nodePosition.X = nodePosition.X + v.X;
					nodePosition.Y = nodePosition.Y + v.Y;
					nodePosition.Z = nodePosition.Z + v.Z;

		node.setPosition(nodePosition);

		node.setMaterialFlag(irr.EMF_LIGHTING, GetMouseState().LeftButtonDown);

		driver.beginScene(True, True, irr.SColor(255,113,113,133));
		smgr.drawAll();
		driver.endScene();

	device.drop();
	return 0;

start();
