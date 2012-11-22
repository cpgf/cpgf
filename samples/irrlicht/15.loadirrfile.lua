cpgf.import("cpgf", "builtin.core");
cpgf.import("cpgf", "builtin.debug");

function start()
	local driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT then
		return 1;
	end

	local device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480));

	if device == nil then
		return 1;
	end

	device.setWindowCaption("cpgf Irrlicht Lua binding Load .irr file example");

	local driver = device.getVideoDriver();
	local smgr = device.getSceneManager();

	smgr.loadScene("../../media/example.irr");

	local camera = smgr.addCameraSceneNodeFPS(nil, 50.0, 0.1);

	local meta = smgr.createMetaTriangleSelector();

	local nodes = irr.array_ISceneNodePointer();
	smgr.getSceneNodesFromType(irr.ESNT_ANY, nodes);

	for i = 0, nodes.size() - 1 do
		local node = nodes._opArrayGet(i);
		local selector = nil;
		local t = node.getType();
		
		if t == irr.ESNT_CUBE or t == irr.ESNT_ANIMATED_MESH then
			selector = smgr.createTriangleSelectorFromBoundingBox(node);

		elseif t == irr.ESNT_MESH or t == irr.ESNT_SPHERE then
			selector = smgr.createTriangleSelector(cpgf.cast(node, irr.IMeshSceneNode).getMesh(), node);

		elseif t == irr.ESNT_TERRAIN then
			selector = smgr.createTerrainTriangleSelector(cpgf.cast(node, irr.ITerrainSceneNode));

		elseif t == irr.ESNT_OCTREE then
			selector = smgr.createOctreeTriangleSelector((cpgf.cast(node, irr.IMeshSceneNode)).getMesh(), node);
		end

		if selector then
			meta.addTriangleSelector(selector);
			selector.drop();
		end
	end

	local anim = smgr.createCollisionResponseAnimator(meta, camera, irr.vector3df(5,5,5),	irr.vector3df(0,0,0));
	meta.drop();

	camera.addAnimator(anim);
	anim.drop();

	camera.setPosition(irr.vector3df(0.0, 20.0, 0.0));

	local cube = smgr.getSceneNodeFromType(irr.ESNT_CUBE);
	if cube then
		camera.setTarget(cube.getAbsolutePosition());
	end

	local lastFPS = -1;

	while device.run() do
		if device.isWindowActive() then
			driver.beginScene(true, true, irr.SColor(0,200,200,200));
			smgr.drawAll();
			driver.endScene();

			local fps = driver.getFPS();

			if lastFPS ~= fps then
				local tmp = "Load Irrlicht File example - cpgf Irrlicht Lua binding [";
				tmp = tmp .. driver.getName();
				tmp = tmp .. "] FPS:";
				tmp = tmp .. fps;

				device.setWindowCaption(tmp);
				lastFPS = fps;
			end

		end
	end

	device.drop();

	return 0;
end

start();
