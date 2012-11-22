cpgf._import("cpgf", "builtin.core");
cpgf._import("cpgf", "builtin.debug");

def start() :
	driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT :
		return 1;

	device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480));

	if device == None :
		return 1;

	device.setWindowCaption("cpgf Irrlicht Python binding Load .irr file example");

	driver = device.getVideoDriver();
	smgr = device.getSceneManager();

	smgr.loadScene("../../media/example.irr");

	camera = smgr.addCameraSceneNodeFPS(None, 50.0, 0.1);

	meta = smgr.createMetaTriangleSelector();

	nodes = irr.array_ISceneNodePointer();
	smgr.getSceneNodesFromType(irr.ESNT_ANY, nodes);

	for i in range(nodes.size()) :
		node = nodes._opArrayGet(i);
		selector = None;
		t = node.getType();
		
		if t == irr.ESNT_CUBE or t == irr.ESNT_ANIMATED_MESH :
			selector = smgr.createTriangleSelectorFromBoundingBox(node);

		elif t == irr.ESNT_MESH or t == irr.ESNT_SPHERE :
			selector = smgr.createTriangleSelector(cpgf.cast(node, irr.IMeshSceneNode).getMesh(), node);

		elif t == irr.ESNT_TERRAIN :
			selector = smgr.createTerrainTriangleSelector(cpgf.cast(node, irr.ITerrainSceneNode));

		elif t == irr.ESNT_OCTREE :
			selector = smgr.createOctreeTriangleSelector((cpgf.cast(node, irr.IMeshSceneNode)).getMesh(), node);

		if selector :
			meta.addTriangleSelector(selector);
			selector.drop();

	anim = smgr.createCollisionResponseAnimator(meta, camera, irr.vector3df(5,5,5),	irr.vector3df(0,0,0));
	meta.drop();

	camera.addAnimator(anim);
	anim.drop();

	camera.setPosition(irr.vector3df(0.0, 20.0, 0.0));

	cube = smgr.getSceneNodeFromType(irr.ESNT_CUBE);
	if cube :
		camera.setTarget(cube.getAbsolutePosition());

	lastFPS = -1;

	while device.run() :
		if device.isWindowActive() :
			driver.beginScene(True, True, irr.SColor(0,200,200,200));
			smgr.drawAll();
			driver.endScene();

			fps = driver.getFPS();

			if lastFPS != fps :
				tmp = "Load Irrlicht File example - cpgf Irrlicht Python binding [";
				tmp = tmp + str(driver.getName());
				tmp = tmp + "] FPS:";
				tmp = tmp + str(fps);

				device.setWindowCaption(tmp);
				lastFPS = fps;


	device.drop();

	return 0;

start();
