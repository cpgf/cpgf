local ResX=800;
local ResY=600;
local fullScreen=false;

local SplitScreen=true;

local camera= { nil, nil, nil, nil };

function overrideEventReceiver(receiver)
	receiver.OnEvent = function(me, event)
		if event.EventType == irr.EET_KEY_INPUT_EVENT and	event.KeyInput.Key == irr.KEY_KEY_S and event.KeyInput.PressedDown then
			SplitScreen = not SplitScreen;
			return true;
		end
		if camera[3] then
			return camera[3].OnEvent(event);
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

	local device = irr.createDevice(driverType, irr.dimension2du(ResX,ResY), 32, fullScreen, false, false, receiver);
	if device == nil then
		return 1;
	end

	local smgr = device.getSceneManager();
	local driver = device.getVideoDriver();

	local model = smgr.getMesh("../../media/sydney.md2");
	if model == nil then
		return 1;
	end
	
	local model_node = smgr.addAnimatedMeshSceneNode(model);
	if model_node then
		local texture = driver.getTexture("../../media/sydney.bmp");
		model_node.setMaterialTexture(0,texture);
		model_node.setMD2Animation(irr.EMAT_RUN);
		model_node.setMaterialFlag(irr.EMF_LIGHTING,false);
	end

	device.getFileSystem().addZipFileArchive("../../media/map-20kdm2.pk3");
	local map = smgr.getMesh("20kdm2.bsp");
	if map then
		local map_node = smgr.addOctreeSceneNode(map.getMesh(0));
		map_node.setPosition(irr.vector3df(-850,-220,-850));
	end

	camera[0] = smgr.addCameraSceneNode(nil, irr.vector3df(50,0,0), irr.vector3df(0,0,0));
	camera[1] = smgr.addCameraSceneNode(nil, irr.vector3df(0,50,0), irr.vector3df(0,0,0));
	camera[2] = smgr.addCameraSceneNode(nil, irr.vector3df(0,0,50), irr.vector3df(0,0,0));
	camera[3] = smgr.addCameraSceneNodeFPS();
	if camera[3] then
		camera[3].setPosition(irr.vector3df(-50,0,-50));
	end

	device.getCursorControl().setVisible(false);
	local lastFPS = -1;

	while device.run() do
		driver.setViewPort(irr.rect_s32(0,0,ResX,ResY));
		driver.beginScene(true,true,irr.SColor(255,100,100,100));
		if SplitScreen then
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
		end
		smgr.setActiveCamera(camera[3]);
		smgr.drawAll();
		driver.endScene();

		if driver.getFPS() ~= lastFPS then
			lastFPS = driver.getFPS();
			local tmp = "cpgf Irrlicht Lua Binding SplitScreen-Example (FPS: ";
			tmp = tmp .. lastFPS;
			tmp = tmp .. ")";
			device.setWindowCaption(tmp);
		end
	end
	device.drop();
	return 0;
end

start();
