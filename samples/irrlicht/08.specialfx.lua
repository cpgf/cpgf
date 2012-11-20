function start()
	local shadows = true;

	local driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT then
		return 1;
	end

	local device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480), 16, false, shadows);

	if device == nil then
		return 1;
	end

	local driver = device.getVideoDriver();
	local smgr = device.getSceneManager();

	local mesh = smgr.getMesh("../../media/room.3ds");

	smgr.getMeshManipulator().makePlanarTextureMapping(mesh.getMesh(0), 0.004);

	local node = nil;

	node = smgr.addAnimatedMeshSceneNode(mesh);
	node.setMaterialTexture(0, driver.getTexture("../../media/wall.jpg"));
	node.getMaterial(0).SpecularColor.set(0,0,0,0);

	mesh = smgr.addHillPlaneMesh( "myHill", irr.dimension2d_f32(20,20), irr.dimension2d_u32(40,40), nil, 0, irr.dimension2d_f32(0,0), irr.dimension2d_f32(10,10));

	node = smgr.addWaterSurfaceSceneNode(mesh.getMesh(0), 3.0, 300.0, 30.0);
	node.setPosition(irr.vector3df(0,7,0));

	node.setMaterialTexture(0, driver.getTexture("../../media/stones.jpg"));
	node.setMaterialTexture(1, driver.getTexture("../../media/water.jpg"));

	node.setMaterialType(irr.EMT_REFLECTION_2_LAYER);


	node = smgr.addLightSceneNode(nil, irr.vector3df(0,0,0), irr.SColorf(1.0, 0.6, 0.7, 1.0), 800.0);
	local anim = 0;
	anim = smgr.createFlyCircleAnimator (irr.vector3df(0,150,0),250.0);
	node.addAnimator(anim);
	anim.drop();


	node = smgr.addBillboardSceneNode(node, irr.dimension2d_f32(50, 50));
	node.setMaterialFlag(irr.EMF_LIGHTING, false);
	node.setMaterialType(irr.EMT_TRANSPARENT_ADD_COLOR);
	node.setMaterialTexture(0, driver.getTexture("../../media/particlewhite.bmp"));


	local ps = smgr.addParticleSystemSceneNode(false);

	local em = ps.createBoxEmitter(irr.aabbox3d_f32(-7,0,-7,7,1,7), irr.vector3df(0.0,0.06,0.0), 80,100, irr.SColor(0,255,255,255), irr.SColor(0,255,255,255),	800,2000,0, irr.dimension2df(10.0,10.0), irr.dimension2df(20.0,20.0));

	ps.setEmitter(em);
	em.drop();

	local paf = ps.createFadeOutParticleAffector();

	ps.addAffector(paf);
	paf.drop();

	ps.setPosition(irr.vector3df(-70,60,40));
	ps.setScale(irr.vector3df(2,2,2));
	ps.setMaterialFlag(irr.EMF_LIGHTING, false);
	ps.setMaterialFlag(irr.EMF_ZWRITE_ENABLE, false);
	ps.setMaterialTexture(0, driver.getTexture("../../media/fire.bmp"));
	ps.setMaterialType(irr.EMT_TRANSPARENT_VERTEX_ALPHA);

	local n = smgr.addVolumeLightSceneNode(nil, -1, 32, 32,	irr.SColor(0, 255, 255, 255), irr.SColor(0, 0, 0, 0));


	mesh = smgr.getMesh("../../media/dwarf.x");
	local anode = nil;

	anode = smgr.addAnimatedMeshSceneNode(mesh);
	anode.setPosition(irr.vector3df(-50,20,-60));
	anode.setAnimationSpeed(15);

	anode.addShadowVolumeSceneNode();
	smgr.setShadowColor(irr.SColor(150,0,0,0));

	anode.setScale(irr.vector3df(2,2,2));
	anode.setMaterialFlag(irr.EMF_NORMALIZE_NORMALS, true);

	local camera = smgr.addCameraSceneNodeFPS();
	camera.setPosition(irr.vector3df(-50,50,-150));

	device.getCursorControl().setVisible(false);

	local lastFPS = -1;

	while device.run() do
		if device.isWindowActive() then
			driver.beginScene(true, true, 0);

			smgr.drawAll();

			driver.endScene();

			local fps = driver.getFPS();

			if lastFPS ~= fps then
				local str = "cpgf Irrlicht Lua Binding - SpecialFX example [";
				str = str .. driver.getName();
				str = str .. "] FPS:";
				str = str .. fps;

				device.setWindowCaption(str);
				lastFPS = fps;
			end
		end
	end

	device.drop();

	return 0;
end

start();
