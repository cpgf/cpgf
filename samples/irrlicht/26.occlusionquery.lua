cpgf.import(nil, "builtin.core");

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

	smgr.getGUIEnvironment().addStaticText("Press Space to hide occluder.", irr.rect_s32(10,10, 200,50));

	local node = smgr.addSphereSceneNode(10, 64);
	if node then
		node.setPosition(irr.vector3df(0,0,60));
		node.setMaterialTexture(0, driver.getTexture("../../media/wall.bmp"));
		node.setMaterialFlag(irr.EMF_LIGHTING, false);
	end

	local plane = smgr.addMeshSceneNode(smgr.addHillPlaneMesh("plane", irr.dimension2df(10,10), irr.dimension2du(2,2)), nil, -1, irr.vector3df(0,0,20), irr.vector3df(270,0,0));

	if plane then
		plane.setMaterialTexture(0, driver.getTexture("../../media/t351sml.jpg"));
		plane.setMaterialFlag(irr.EMF_LIGHTING, false);
		plane.setMaterialFlag(irr.EMF_BACK_FACE_CULLING, true);
	end

	driver.addOcclusionQuery(node, cpgf.cast(node, irr.IMeshSceneNode).getMesh());

	smgr.addCameraSceneNode();
	local lastFPS = -1;
	local timeNow = device.getTimer().getTime();
	local nodeVisible=true;

	while device.run() do
		plane.setVisible(not IsKeyDown(irr.KEY_SPACE));

		driver.beginScene(true, true, irr.SColor(255,113,113,133));
		node.setVisible(nodeVisible);
		smgr.drawAll();
		smgr.getGUIEnvironment().drawAll();

		if device.getTimer().getTime()-timeNow>100 then
			driver.runAllOcclusionQueries(false);
			driver.updateAllOcclusionQueries();
			nodeVisible=driver.getOcclusionQueryResult(node)>0;
			timeNow=device.getTimer().getTime();
		end

		driver.endScene();

		local fps = driver.getFPS();

		if lastFPS ~= fps then
			local tmp = "cpgf Irrlicht Lua binding OcclusionQuery Example [";
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
