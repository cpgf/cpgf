def start() :
	shadows = True;

	driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT :
		return 1;

	device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480), 16, False, shadows);

	if device == None :
		return 1;

	driver = device.getVideoDriver();
	smgr = device.getSceneManager();

	mesh = smgr.getMesh("../../media/room.3ds");

	smgr.getMeshManipulator().makePlanarTextureMapping(mesh.getMesh(0), 0.004);

	node = None;

	node = smgr.addAnimatedMeshSceneNode(mesh);
	node.setMaterialTexture(0, driver.getTexture("../../media/wall.jpg"));
	node.getMaterial(0).SpecularColor.set(0,0,0,0);

	mesh = smgr.addHillPlaneMesh( "myHill", irr.dimension2d_f32(20,20), irr.dimension2d_u32(40,40), None, 0, irr.dimension2d_f32(0,0), irr.dimension2d_f32(10,10));

	node = smgr.addWaterSurfaceSceneNode(mesh.getMesh(0), 3.0, 300.0, 30.0);
	node.setPosition(irr.vector3df(0,7,0));

	node.setMaterialTexture(0, driver.getTexture("../../media/stones.jpg"));
	node.setMaterialTexture(1, driver.getTexture("../../media/water.jpg"));

	node.setMaterialType(irr.EMT_REFLECTION_2_LAYER);


	node = smgr.addLightSceneNode(None, irr.vector3df(0,0,0), irr.SColorf(1.0, 0.6, 0.7, 1.0), 800.0);
	anim = 0;
	anim = smgr.createFlyCircleAnimator (irr.vector3df(0,150,0),250.0);
	node.addAnimator(anim);
	anim.drop();


	node = smgr.addBillboardSceneNode(node, irr.dimension2d_f32(50, 50));
	node.setMaterialFlag(irr.EMF_LIGHTING, False);
	node.setMaterialType(irr.EMT_TRANSPARENT_ADD_COLOR);
	node.setMaterialTexture(0, driver.getTexture("../../media/particlewhite.bmp"));


	ps = smgr.addParticleSystemSceneNode(False);

	em = ps.createBoxEmitter(irr.aabbox3d_f32(-7,0,-7,7,1,7), irr.vector3df(0.0,0.06,0.0), 80,100, irr.SColor(0,255,255,255), irr.SColor(0,255,255,255),	800,2000,0, irr.dimension2df(10.0,10.0), irr.dimension2df(20.0,20.0));

	ps.setEmitter(em);
	em.drop();

	paf = ps.createFadeOutParticleAffector();

	ps.addAffector(paf);
	paf.drop();

	ps.setPosition(irr.vector3df(-70,60,40));
	ps.setScale(irr.vector3df(2,2,2));
	ps.setMaterialFlag(irr.EMF_LIGHTING, False);
	ps.setMaterialFlag(irr.EMF_ZWRITE_ENABLE, False);
	ps.setMaterialTexture(0, driver.getTexture("../../media/fire.bmp"));
	ps.setMaterialType(irr.EMT_TRANSPARENT_VERTEX_ALPHA);

	n = smgr.addVolumeLightSceneNode(None, -1, 32, 32,	irr.SColor(0, 255, 255, 255), irr.SColor(0, 0, 0, 0));


	mesh = smgr.getMesh("../../media/dwarf.x");
	anode = None;

	anode = smgr.addAnimatedMeshSceneNode(mesh);
	anode.setPosition(irr.vector3df(-50,20,-60));
	anode.setAnimationSpeed(15);

	anode.addShadowVolumeSceneNode();
	smgr.setShadowColor(irr.SColor(150,0,0,0));

	anode.setScale(irr.vector3df(2,2,2));
	anode.setMaterialFlag(irr.EMF_NORMALIZE_NORMALS, True);

	camera = smgr.addCameraSceneNodeFPS();
	camera.setPosition(irr.vector3df(-50,50,-150));

	device.getCursorControl().setVisible(False);

	lastFPS = -1;

	while device.run() :
		if device.isWindowActive() :
			driver.beginScene(True, True, 0);

			smgr.drawAll();

			driver.endScene();

			fps = driver.getFPS();

			if lastFPS != fps :
				tmp = "cpgf Irrlicht Python Binding - SpecialFX example [";
				tmp = tmp + driver.getName();
				tmp = tmp + "] FPS:";
				tmp = tmp + str(fps);

				device.setWindowCaption(tmp);
				lastFPS = fps;

	device.drop();

	return 0;

start();
