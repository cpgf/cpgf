cpgf.import("cpgf", "builtin.core");
cpgf.import("cpgf", "builtin.debug");

function start()
{
	// ask user for driver
	var driverType = irr.driverChoiceConsole();
	if(driverType == irr.EDT_COUNT)
		return 1;


	// create device and exit if creation failed

	var device = irr.createDevice(driverType, new irr.dimension2d_u32(640, 480));

	if (device == null)
		return 1; // could not create selected driver.

	device.setWindowCaption("cpgf Irrlicht JavaScript binding Load .irr file example");

	var driver = device.getVideoDriver();
	var smgr = device.getSceneManager();

	/*
	Now load our .irr file.
	.irr files can store the whole scene graph including animators,
	materials and particle systems. And there is also the possibility to
	store arbitrary user data for every scene node in that file. To keep
	this example simple, we are simply loading the scene here. See the
	documentation at ISceneManager::loadScene and ISceneManager::saveScene
	for more information. So to load and display a complicated huge scene,
	we only need a single call to loadScene().
	*/

	// load the scene
	smgr.loadScene("../../media/example.irr");

	/*
	Now we'll create a camera, and give it a collision response animator
	that's built from the mesh nodes in the scene we just loaded.
	*/
	var camera = smgr.addCameraSceneNodeFPS(null, 50.0, 0.1);

	// Create a meta triangle selector to hold several triangle selectors.
	var meta = smgr.createMetaTriangleSelector();

	/*
	Now we will find all the nodes in the scene and create triangle
	selectors for all suitable nodes.  Typically, you would want to make a
	more informed decision about which nodes to performs collision checks
	on; you could capture that information in the node name or Id.
	*/
	var nodes = new irr.array_ISceneNodePointer();
	smgr.getSceneNodesFromType(irr.ESNT_ANY, nodes); // Find all nodes

	for(var i=0; i < nodes.size(); ++i)
	{
		var node = nodes._opArrayGet(i);
		var selector = null;

		switch(node.getType())
		{
		case irr.ESNT_CUBE:
		case irr.ESNT_ANIMATED_MESH:
			// Because the selector won't animate with the mesh,
			// and is only being used for camera collision, we'll just use an approximate
			// bounding box instead of ((irr.IAnimatedMeshSceneNode*)node).getMesh(0)
			selector = smgr.createTriangleSelectorFromBoundingBox(node);
		break;

		case irr.ESNT_MESH:
		case irr.ESNT_SPHERE: // Derived from IMeshSceneNode
			selector = smgr.createTriangleSelector(cpgf.cast(node, irr.IMeshSceneNode).getMesh(), node);
			break;

		case irr.ESNT_TERRAIN:
			selector = smgr.createTerrainTriangleSelector(cpgf.cast(node, irr.ITerrainSceneNode));
			break;

		case irr.ESNT_OCTREE:
			selector = smgr.createOctreeTriangleSelector((cpgf.cast(node, irr.IMeshSceneNode)).getMesh(), node);
			break;

		default:
			// Don't create a selector for this node type
			break;
		}

		if(selector)
		{
			// Add it to the meta selector, which will take a reference to it
			meta.addTriangleSelector(selector);
			// And drop my reference to it, so that the meta selector owns it.
			selector.drop();
		}
	}

	/*
	Now that the mesh scene nodes have had triangle selectors created and added
	to the meta selector, create a collision response animator from that meta selector.
	*/
	var anim = smgr.createCollisionResponseAnimator(meta, camera, new irr.vector3df(5,5,5),	new irr.vector3df(0,0,0));
	meta.drop(); // I'm done with the meta selector now

	camera.addAnimator(anim);
	anim.drop(); // I'm done with the animator now

	// And set the camera position so that it doesn't start off stuck in the geometry
	camera.setPosition(new irr.vector3df(0.0, 20.0, 0.0));

	// Point the camera at the cube node, by finding the first node of type ESNT_CUBE
	var cube = smgr.getSceneNodeFromType(irr.ESNT_CUBE);
	if(cube) {
		camera.setTarget(cube.getAbsolutePosition());
	}

	/*
	That's it. Draw everything and finish as usual.
	*/

	var lastFPS = -1;

	while(device.run()) {
		if (device.isWindowActive())
		{
			driver.beginScene(true, true, new irr.SColor(0,200,200,200));
			smgr.drawAll();
			driver.endScene();

			var fps = driver.getFPS();

			if (lastFPS != fps)
			{
				var tmp = "Load Irrlicht File example - cpgf Irrlicht JavaScript binding [";
				tmp = tmp + driver.getName();
				tmp = tmp + "] FPS:";
				tmp = tmp + fps;

				device.setWindowCaption(tmp);
				lastFPS = fps;
			}

		}
	}

	device.drop();

	return 0;
}

start();
