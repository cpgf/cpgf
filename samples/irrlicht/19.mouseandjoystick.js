// Know issues in cpgf script binding
// Joy stick is not supported because it uses irr::array which is a template

cpgf.import(null, "builtin.debug");

function SMouseState()
{
	this.Position = new irr.position2di();
	this.LeftButtonDown = false;
}


var MouseState = new SMouseState();
var JoystickState = new irr.SEvent.SJoystickEvent();

function GetJoystickState()
{
	return JoystickState;
}

function GetMouseState()
{
	return MouseState;
}

function overrideEventReceiver(receiver)
{
	receiver.OnEvent = function(me, event)
	{
		// Remember the mouse state
		if (event.EventType == irr.EET_MOUSE_INPUT_EVENT)
		{
			switch(event.MouseInput.Event)
			{
			case irr.EMIE_LMOUSE_PRESSED_DOWN:
				MouseState.LeftButtonDown = true;
				break;

			case irr.EMIE_LMOUSE_LEFT_UP:
				MouseState.LeftButtonDown = false;
				break;

			case irr.EMIE_MOUSE_MOVED:
				MouseState.Position.X = event.MouseInput.X;
				MouseState.Position.Y = event.MouseInput.Y;
				break;

			default:
				// We won't use the wheel
				break;
			}
		}

		// The state of each connected joystick is sent to us
		// once every run() of the Irrlicht device.  Store the
		// state of the first joystick, ignoring other joysticks.
		// This is currently only supported on Windows and Linux.
		if (event.EventType == irr.EET_JOYSTICK_INPUT_EVENT	&& event.JoystickEvent.Joystick == 0)
		{
			JoystickState = event.JoystickEvent;
		}

		return false;
	}

}


/*
The event receiver for keeping the pressed keys is ready, the actual responses
will be made inside the render loop, right before drawing the scene. So lets
just create an irr.IrrlichtDevice and the scene node we want to move. We also
create some other additional scene nodes, to show that there are also some
different possibilities to move and animate scene nodes.
*/
function start()
{
	// ask user for driver
	var driverType=irr.driverChoiceConsole();
	if (driverType==irr.EDT_COUNT)
		return 1;

	// create device
	var receiver = new irr.IEventReceiverWrapper();
	overrideEventReceiver(receiver);

	var device = irr.createDevice(driverType, new irr.dimension2d_u32(640, 480), 16, false, false, false, receiver);

	if (device == null)
		return 1; // could not create selected driver.


	var tmp = "cpgf Irrlicht JavaScript Binding Joystick Example";
	device.setWindowCaption(tmp);

	var driver = device.getVideoDriver();
	var smgr = device.getSceneManager();

	/*
	We'll create an arrow mesh and move it around either with the joystick axis/hat,
	or make it follow the mouse pointer. */
	var node = smgr.addMeshSceneNode(smgr.addArrowMesh( "Arrow", new irr.SColor(255, 255, 0, 0), new irr.SColor(255, 0, 255, 0), 16,16, 2.0, 1.3, 0.1, 0.6));
	node.setMaterialFlag(irr.EMF_LIGHTING, false);

	var camera = smgr.addCameraSceneNode();
	camera.setPosition(new irr.vector3df(0, 0, -10));

	// As in example 04, we'll use framerate independent movement.
	var then = device.getTimer().getTime();
	var MOVEMENT_SPEED = 5.0;

	while(device.run())
	{
		// Work out a frame delta time.
		var now = device.getTimer().getTime();
		var frameDeltaTime = (now - then) / 1000.0;
		then = now;

		var movedWithJoystick = false;
		var nodePosition = new irr.vector3df(node.getPosition());

		// If the arrow node isn't being moved with the joystick, then have it follow the mouse cursor.
		if(!movedWithJoystick)
		{
			// Create a ray through the mouse cursor.
			var ray = smgr.getSceneCollisionManager().getRayFromScreenCoordinates(GetMouseState().Position, camera);

			// And intersect the ray with a plane around the node facing towards the camera.

			var plane = new irr.plane3df(nodePosition, new irr.vector3df(0, 0, -1));
			var mousePosition = new irr.vector3df();
			if(plane.getIntersectionWithLine(ray.start, ray.getVector(), mousePosition))
			{
				// We now have a mouse position in 3d space; move towards it.
				var toMousePosition = new irr.vector3df(mousePosition);
				toMousePosition.X = toMousePosition.X - nodePosition.X;
				toMousePosition.Y = toMousePosition.Y - nodePosition.Y;
				toMousePosition.Z = toMousePosition.Z - nodePosition.Z;
				
				var availableMovement = MOVEMENT_SPEED * frameDeltaTime;
				
				if(toMousePosition.getLength() <= availableMovement)
					nodePosition = mousePosition; // Jump to the final position
				else {
					var v = toMousePosition.normalize();
					v.X = v.X * availableMovement;
					v.Y = v.Y * availableMovement;
					v.Z = v.Z * availableMovement;
					nodePosition.X = nodePosition.X + v.X;
					nodePosition.Y = nodePosition.Y + v.Y;
					nodePosition.Z = nodePosition.Z + v.Z;
				}
			}
		}

		node.setPosition(nodePosition);

		// Turn lighting on and off depending on whether the left mouse button is down.
		node.setMaterialFlag(irr.EMF_LIGHTING, GetMouseState().LeftButtonDown);

		driver.beginScene(true, true, new irr.SColor(255,113,113,133));
		smgr.drawAll(); // draw the 3d scene
		driver.endScene();
	}

	/*
	In the end, delete the Irrlicht device.
	*/
	device.drop();

	return 0;
}

start();
