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

	/*
	Get a pointer to the video driver and the SceneManager so that
	we do not always have to call irr::IrrlichtDevice::getVideoDriver() and
	irr::IrrlichtDevice::getSceneManager().
	*/
	var driver = device.getVideoDriver();
	var smgr = device.getSceneManager();

	/*
	To display the Quake 3 map, we first need to load it. Quake 3 maps
	are packed into .pk3 files which are nothing else than .zip files.
	So we add the .pk3 file to our irr::io::IFileSystem. After it was added,
	we are able to read from the files in that archive as if they are
	directly stored on the disk.
	*/
	device.getFileSystem().addZipFileArchive("../../media/map-20kdm2.pk3");

	/*
	Now we can load the mesh by calling
	irr::scene::ISceneManager::getMesh(). We get a pointer returned to an
	irr::scene::IAnimatedMesh. As you might know, Quake 3 maps are not
	really animated, they are only a huge chunk of static geometry with
	some materials attached. Hence the IAnimatedMesh consists of only one
	frame, so we get the "first frame" of the "animation", which is our
	quake level and create an Octree scene node with it, using
	irr::scene::ISceneManager::addOctreeSceneNode().
	The Octree optimizes the scene a little bit, trying to draw only geometry
	which is currently visible. An alternative to the Octree would be a
	irr::scene::IMeshSceneNode, which would always draw the complete
	geometry of the mesh, without optimization. Try it: Use
	irr::scene::ISceneManager::addMeshSceneNode() instead of
	addOctreeSceneNode() and compare the primitives drawn by the video
	driver. (There is a irr::video::IVideoDriver::getPrimitiveCountDrawn()
	method in the irr::video::IVideoDriver class). Note that this
	optimization with the Octree is only useful when drawing huge meshes
	consisting of lots of geometry.
	*/
	var mesh = smgr.getMesh("20kdm2.bsp");
	var node = null;

	if (mesh != null)
		node = smgr.addOctreeSceneNode(mesh.getMesh(0), 0, -1, 1024);

	/*
	Because the level was not modelled around the origin (0,0,0), we
	translate the whole level a little bit. This is done on
	irr::scene::ISceneNode level using the methods
	irr::scene::ISceneNode::setPosition() (in this case),
	irr::scene::ISceneNode::setRotation(), and
	irr::scene::ISceneNode::setScale().
	*/
	if (node != null)
		node.setPosition(new irr.vector3df(-1300,-144,-1249));

	/*
	Now we only need a camera to look at the Quake 3 map.
	We want to create a user controlled camera. There are some
	cameras available in the Irrlicht engine. For example the
	MayaCamera which can be controlled like the camera in Maya:
	Rotate with left mouse button pressed, Zoom with both buttons pressed,
	translate with right mouse button pressed. This could be created with
	irr::scene::ISceneManager::addCameraSceneNodeMaya(). But for this
	example, we want to create a camera which behaves like the ones in
	first person shooter games (FPS) and hence use
	irr::scene::ISceneManager::addCameraSceneNodeFPS().
	*/
	smgr.addCameraSceneNodeFPS();

	/*
	The mouse cursor needs not be visible, so we hide it via the
	irr::IrrlichtDevice::ICursorControl.
	*/
	device.getCursorControl().setVisible(false);

	/*
	We have done everything, so lets draw it. We also write the current
	frames per second and the primitives drawn into the caption of the
	window. The test for irr::IrrlichtDevice::isWindowActive() is optional,
	but prevents the engine to grab the mouse cursor after task switching
	when other programs are active. The call to
	irr::IrrlichtDevice::yield() will avoid the busy loop to eat up all CPU
	cycles when the window is not active.
	*/
	var lastFPS = -1;

	while(device.run())
	{
		if (device.isWindowActive())
		{
			driver.beginScene(true, true, new irr.SColor(255,200,200,200));
			smgr.drawAll();
			driver.endScene();

			var fps = driver.getFPS();

			if (lastFPS != fps)
			{
				var tmp = "cpgf Irrlicht JavaScript Binding Demo - Quake 3 Map example [";
				tmp = tmp + driver.getName();
				tmp = tmp + "] fps: ";
				tmp = tmp + fps;

				device.setWindowCaption(tmp);
				lastFPS = fps;
			}
		}
		else
			device.yield();
	}

	/*
	In the end, delete the Irrlicht device.
	*/
	device.drop();
	return 0;
}

start();
