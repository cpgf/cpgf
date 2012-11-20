
cpgf.import(nil, "builtin.debug");

local MouseState = {
	Position = irr.position2di(),
	LeftButtonDown = false
};
local JoystickState = irr.SEvent.SJoystickEvent();

function GetJoystickState()
	return JoystickState;
end

function GetMouseState()
	return MouseState;
end

function overrideEventReceiver(receiver)
	receiver.OnEvent = function(me, event)
		if event.EventType == irr.EET_MOUSE_INPUT_EVENT then
			local e = event.MouseInput.Event;
			if e == irr.EMIE_LMOUSE_PRESSED_DOWN then
				MouseState.LeftButtonDown = true;
			
			elseif e == irr.EMIE_LMOUSE_LEFT_UP then
				MouseState.LeftButtonDown = false;

			elseif e == irr.EMIE_MOUSE_MOVED then
				MouseState.Position.X = event.MouseInput.X;
				MouseState.Position.Y = event.MouseInput.Y;
			end
		end

		if event.EventType == irr.EET_JOYSTICK_INPUT_EVENT	and event.JoystickEvent.Joystick == 0 then
			JoystickState = event.JoystickEvent;
		end

		return false;
	end

end


function start()
	local driverType=irr.driverChoiceConsole();
	if driverType==irr.EDT_COUNT then
		return 1;
	end

	local receiver = irr.IEventReceiverWrapper();
	overrideEventReceiver(receiver);

	local device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480), 16, false, false, false, receiver);

	if device == nil then
		return 1;
	end

	local tmp = "cpgf Irrlicht Lua Binding Joystick Example";
	device.setWindowCaption(tmp);

	local driver = device.getVideoDriver();
	local smgr = device.getSceneManager();

	local node = smgr.addMeshSceneNode(smgr.addArrowMesh( "Arrow", irr.SColor(255, 255, 0, 0), irr.SColor(255, 0, 255, 0), 16,16, 2.0, 1.3, 0.1, 0.6));
	node.setMaterialFlag(irr.EMF_LIGHTING, false);

	local camera = smgr.addCameraSceneNode();
	camera.setPosition(irr.vector3df(0, 0, -10));

	local t = device.getTimer().getTime();
	local MOVEMENT_SPEED = 5.0;

	while device.run() do
		local now = device.getTimer().getTime();
		local frameDeltaTime = (now - t) / 1000.0;
		t = now;

		local movedWithJoystick = false;
		local nodePosition = irr.vector3df(node.getPosition());

		if not movedWithJoystick then
			local ray = smgr.getSceneCollisionManager().getRayFromScreenCoordinates(GetMouseState().Position, camera);

			local plane = irr.plane3df(nodePosition, irr.vector3df(0, 0, -1));
			local mousePosition = irr.vector3df();
			if plane.getIntersectionWithLine(ray.start, ray.getVector(), mousePosition) then
				local toMousePosition = irr.vector3df(mousePosition);
				toMousePosition.X = toMousePosition.X - nodePosition.X;
				toMousePosition.Y = toMousePosition.Y - nodePosition.Y;
				toMousePosition.Z = toMousePosition.Z - nodePosition.Z;
				
				local availableMovement = MOVEMENT_SPEED * frameDeltaTime;
				
				if toMousePosition.getLength() <= availableMovement then
					nodePosition = mousePosition;
				else
					local v = toMousePosition.normalize();
					v.X = v.X * availableMovement;
					v.Y = v.Y * availableMovement;
					v.Z = v.Z * availableMovement;
					nodePosition.X = nodePosition.X + v.X;
					nodePosition.Y = nodePosition.Y + v.Y;
					nodePosition.Z = nodePosition.Z + v.Z;
				end
			end
		end

		node.setPosition(nodePosition);

		node.setMaterialFlag(irr.EMF_LIGHTING, GetMouseState().LeftButtonDown);

		driver.beginScene(true, true, irr.SColor(255,113,113,133));
		smgr.drawAll();
		driver.endScene();
	end

	device.drop();
	return 0;
end

start();
