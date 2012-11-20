KeyIsDown = {};

function makeMyEventReceiver(receiver)
	for i = 1, irr.KEY_KEY_CODES_COUNT do
		KeyIsDown[i] = false;
	end

	receiver.OnEvent = function(me, event)
		if event.EventType == irr.EET_KEY_INPUT_EVENT then
			KeyIsDown[event.KeyInput.Key + 1] = event.KeyInput.PressedDown;
		end

		return false;
	end

end


function IsKeyDown(keyCode)
	return KeyIsDown[keyCode + 1];
end

cpgf.import(nil, "builtin.core");

function start()
	local driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT then
		return 1;
	end


	local MyEventReceiver = cpgf.cloneClass(irr.IEventReceiverWrapper);
	makeMyEventReceiver(MyEventReceiver);
	local receiver = MyEventReceiver();

	local device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480), 16, false, false, false, receiver);

	if device == nil then
		return 1;
	end

	local driver = device.getVideoDriver();
	local smgr = device.getSceneManager();

	local node = smgr.addSphereSceneNode();
	if node then

		node.setPosition(irr.vector3df(0, 0, 30));
		node.setMaterialTexture(0, driver.getTexture("../../media/wall.bmp"));
		node.setMaterialFlag(irr.EMF_LIGHTING, false);
	end

	local n = smgr.addCubeSceneNode();

	if n then
		n.setMaterialTexture(0, driver.getTexture("../../media/t351sml.jpg"));
		n.setMaterialFlag(irr.EMF_LIGHTING, false);
		local anim = smgr.createFlyCircleAnimator(irr.vector3df(0,0,30), 20.0);
		if anim then
			n.addAnimator(anim);
			anim.drop();
		end
	end

	local anms = smgr.addAnimatedMeshSceneNode(smgr.getMesh("../../media/ninja.b3d"));

	if anms then
		local anim = smgr.createFlyStraightAnimator(irr.vector3df(100,0,60), irr.vector3df(-100,0,60), 3500, true);
		if anim then

			anms.addAnimator(anim);
			anim.drop();
		end

		anms.setMaterialFlag(irr.EMF_LIGHTING, false);

		anms.setFrameLoop(0, 13);
		anms.setAnimationSpeed(15);

		anms.setScale(irr.vector3df(2.0,2.0,2.0));
		anms.setRotation(irr.vector3df(0,-90,0));

	end

	smgr.addCameraSceneNodeFPS();
	device.getCursorControl().setVisible(false);

	device.getGUIEnvironment().addImage(driver.getTexture("../../media/irrlichtlogoalpha2.tga"), irr.position2d_s32(10,20));

	local diagnostics = device.getGUIEnvironment().addStaticText("", irr.rect_s32(10, 10, 400, 20));
	diagnostics.setOverrideColor(irr.SColor(255, 255, 255, 0));

	local lastFPS = -1;
	local t = device.getTimer().getTime();
	local MOVEMENT_SPEED = 5.0;

	while device.run() do
		local now = device.getTimer().getTime();
		local frameDeltaTime = (now - t) / 1000.0;
		t = now;

		local nodePosition = node.getPosition();

		if IsKeyDown(irr.KEY_KEY_W) then
			nodePosition.Y = nodePosition.Y + MOVEMENT_SPEED * frameDeltaTime;
		elseif IsKeyDown(irr.KEY_KEY_S) then
			nodePosition.Y = nodePosition.Y - MOVEMENT_SPEED * frameDeltaTime;
		end

		if IsKeyDown(irr.KEY_KEY_A) then
			nodePosition.X = nodePosition.X - MOVEMENT_SPEED * frameDeltaTime;
		elseif IsKeyDown(irr.KEY_KEY_D) then
			nodePosition.X = nodePosition.X + MOVEMENT_SPEED * frameDeltaTime;
		end

		node.setPosition(nodePosition);

		driver.beginScene(true, true, irr.SColor(255,113,113,133));

		smgr.drawAll();
		device.getGUIEnvironment().drawAll();

		driver.endScene();

		local fps = driver.getFPS();

		if lastFPS ~= fps then
			local tmp = "Movement Example - cpgf Irrlicht Lua Binding Demo [";
			tmp = tmp .. driver.getName();
			tmp = tmp .. "] fps: ";
			tmp = tmp .. fps;

			device.setWindowCaption(tmp);
			lastFPS = fps;
		end
	end
	
	device.drop();
	
	return 0;
end

start();
