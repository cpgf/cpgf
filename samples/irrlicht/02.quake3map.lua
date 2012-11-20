function start()
	local driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT then
		return 1;
	end

	local device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480));

	if device == nil then
		return 1;
	end

	local driver = device.getVideoDriver();
	local smgr = device.getSceneManager();

	device.getFileSystem().addZipFileArchive("../../media/map-20kdm2.pk3");

	local mesh = smgr.getMesh("20kdm2.bsp");
	local node = nil;

	if mesh ~= nil then
		node = smgr.addOctreeSceneNode(mesh.getMesh(0), nil, -1, 1024);
	end

	if node ~= nil then
		node.setPosition(irr.vector3df(-1300,-144,-1249));
	end

	smgr.addCameraSceneNodeFPS();

	device.getCursorControl().setVisible(false);

	local lastFPS = -1;

	while device.run() do
		if device.isWindowActive() then
			driver.beginScene(true, true, irr.SColor(255,200,200,200));
			smgr.drawAll();
			driver.endScene();

			local fps = driver.getFPS();

			if lastFPS ~= fps then
				local tmp = "cpgf Irrlicht Lua Binding Demo - Quake 3 Map example [";
				tmp = tmp .. driver.getName();
				tmp = tmp .. "] fps: ";
				tmp = tmp .. fps;

				device.setWindowCaption(tmp);
				lastFPS = fps;
			end
		else
			device.yield();
		end
	end

	device.drop();
	return 0;
end

start();
