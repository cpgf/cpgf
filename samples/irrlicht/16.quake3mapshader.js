cpgf.import(null, "builtin.core");
cpgf.import(null, "builtin.collections.bytearray");

/*
A class to produce a series of screenshots
*/
function overrideScreenShotFactory(screenshotFactory, device, templateName, node)
{
	this.Device = device
	this.Number = 0;
	this.FilenameTemplate = templateName;
	this.Node = node;
	
	this.FilenameTemplate.replace ( '/', '_' );
	this.FilenameTemplate.replace ( '\\', '_' );

	var self = this;

	screenshotFactory.OnEvent = function(me, event)
	{
		// check if user presses the key F9
		if ((event.EventType == irr.EET_KEY_INPUT_EVENT) && event.KeyInput.PressedDown)
		{
			if (event.KeyInput.Key == irr.KEY_F9)
			{
				var image = self.Device.getVideoDriver().createScreenShot();
				if (image)
				{
					var buf = self.FilenameTemplate + "_shot" + (++self.Number) + ".jpg";
					selfDevice.getVideoDriver().writeImageToFile(image, buf, 85 );
					image.drop();
				}
			}
			else
			if (event.KeyInput.Key == irr.KEY_F8)
			{
				if (self.Node.isDebugDataVisible())
					self.Node.setDebugDataVisible(irr.EDS_OFF);
				else
					self.Node.setDebugDataVisible(irr.EDS_BBOX_ALL);
			}
		}
		return false;
	}
}


/*
Ok, lets start.
*/

function start()
{
	/*
	Like in the HelloWorld example, we create an IrrlichtDevice with
	createDevice(). The difference now is that we ask the user to select
	which hardware accelerated driver to use. The Software device would be
	too slow to draw a huge Quake 3 map, but just for the fun of it, we make
	this decision possible too.
	*/

	// ask user for driver
	var driverType=irr.driverChoiceConsole();
	if (driverType==irr.EDT_COUNT)
		return 1;

	// create device and exit if creation failed
	var videoDim = new irr.dimension2du(800,600);

	var device = irr.createDevice(driverType, videoDim, 32, false );

	if (device == null)
		return 1; // could not create selected driver.

	var mapname = "maps/20kdm2.bsp";

	/*
	Get a pointer to the video driver and the SceneManager so that
	we do not always have to write device.getVideoDriver() and
	device.getSceneManager().
	*/
	var driver = device.getVideoDriver();
	var smgr = device.getSceneManager();
	var gui = device.getGUIEnvironment();

	//! add our private media directory to the file system
	device.getFileSystem().addFolderFileArchive("../../media/");

	/*
	To display the Quake 3 map, we first need to load it. Quake 3 maps
	are packed into .pk3 files, which are nothing other than .zip files.
	So we add the .pk3 file to our FileSystem. After it was added,
	we are able to read from the files in that archive as they would
	directly be stored on disk.
	*/
	device.getFileSystem().addZipFileArchive("../../media/map-20kdm2.pk3");

	// Quake3 Shader controls Z-Writing
	smgr.getParameters().setAttribute(irr.ALLOW_ZWRITE_ON_TRANSPARENT, true);

	/*
	Now we can load the mesh by calling getMesh(). We get a pointer returned
	to a IAnimatedMesh. As you know, Quake 3 maps are not really animated,
	they are only a huge chunk of static geometry with some materials
	attached. Hence the IAnimated mesh consists of only one frame,
	so we get the "first frame" of the "animation", which is our quake level
	and create an Octree scene node with it, using addOctreeSceneNode().
	The Octree optimizes the scene a little bit, trying to draw only geometry
	which is currently visible. An alternative to the Octree would be a
	AnimatedMeshSceneNode, which would draw always the complete geometry of
	the mesh, without optimization. Try it out: Write addAnimatedMeshSceneNode
	instead of addOctreeSceneNode and compare the primitives drawed by the
	video driver. (There is a getPrimitiveCountDrawed() method in the
	IVideoDriver class). Note that this optimization with the Octree is only
	useful when drawing huge meshes consisting of lots of geometry.
	*/
	var mesh = cpgf.cast(smgr.getMesh(mapname), irr.IQ3LevelMesh);

	/*
		add the geometry mesh to the Scene ( polygon & patches )
		The Geometry mesh is optimised for faster drawing
	*/
	var node = null;
	if (mesh)
	{
		var geometry = mesh.getMesh(irr.E_Q3_MESH_GEOMETRY);
		node = smgr.addOctreeSceneNode(geometry, 0, -1, 4096);
	}

	// create an event receiver for making screenshots
	var screenshotFactory = new irr.IEventReceiverWrapper();
	overrideScreenShotFactory(screenshotFactory, device, mapname, node);
	device.setEventReceiver(screenshotFactory);

	/*
		now construct SceneNodes for each Shader
		The Objects are stored in the quake mesh irr.E_Q3_MESH_ITEMS
		and the Shader ID is stored in the MaterialParameters
		mostly dark looking skulls and moving lava.. or green flashing tubes?
	*/
	if ( mesh )
	{
		// the additional mesh can be quite huge and is unoptimized
		var additional_mesh = mesh.getMesh(irr.E_Q3_MESH_ITEMS);

		for (var i = 0; i!= additional_mesh.getMeshBufferCount(); ++i )
		{
			var meshBuffer = additional_mesh.getMeshBuffer(i);
			var material = meshBuffer.getMaterial();

			// The ShaderIndex is stored in the material parameter
			var shaderIndex = material.MaterialTypeParam2;

			// the meshbuffer can be rendered without additional support, or it has no shader
			var shader = mesh.getShader(shaderIndex);
			if (null == shader)
			{
				continue;
			}

			// we can dump the shader to the console in its
			// original but already parsed layout in a pretty
			// printers way.. commented out, because the console
			// would be full...
			// irr.dumpShader ( Shader );

			node = smgr.addQuake3SceneNode(meshBuffer, shader);

		}
	}

	/*
	Now we only need a Camera to look at the Quake 3 map. And we want to
	create a user controlled camera. There are some different cameras
	available in the Irrlicht engine. For example the Maya Camera which can
	be controlled compareable to the camera in Maya: Rotate with left mouse
	button pressed, Zoom with both buttons pressed, translate with right
	mouse button pressed. This could be created with
	addCameraSceneNodeMaya(). But for this example, we want to create a
	camera which behaves like the ones in first person shooter games (FPS).
	*/

	var camera = smgr.addCameraSceneNodeFPS();

	/*
		so we need a good starting Position in the level.
		we can ask the Quake3 Loader for all entities with class_name
		"info_player_deathmatch"
		we choose a random launch
	*/

	if ( mesh )
	{
		var entityList = mesh.getEntityList();

		var se = new irr.IShader();
		se.name = "info_player_deathmatch";

		var index = entityList.binary_search(se);
		if (index >= 0)
		{
			var notEndList;
			do
			{
				var group = entityList._opArrayGet(index).getGroup(1);

				var parsepos = cpgf.createByteArray(8);
				parsepos.writeInt32(0);
				var pos = irr.getAsVector3df(group.get("origin"), parsepos.getPointer());

				parsepos.setPosition(0);
				parsepos.writeInt32(0);
				var angle = irr.getAsFloat(group.get("angle"), parsepos.getPointer());

				var target = new irr.vector3df(0.0, 0.0, 1.0);
				target.rotateXZBy(angle);

				camera.setPosition(pos);
				camera.setTarget(pos._opAdd(target));

				++index;
				notEndList = (index == 2);
			} while ( notEndList );
		}
	}

	/*
	The mouse cursor needs not to be visible, so we make it invisible.
	*/

	device.getCursorControl().setVisible(false);

	// load the engine logo
	gui.addImage(driver.getTexture("irrlichtlogo2.png"), new irr.position2d_s32(10, 10));

	// show the driver logo
	var pos = new irr.position2di(videoDim.Width - 128, videoDim.Height - 64);

	switch ( driverType )
	{
		case irr.EDT_BURNINGSVIDEO:
			gui.addImage(driver.getTexture("burninglogo.png"), pos);
			break;
		case irr.EDT_OPENGL:
			gui.addImage(driver.getTexture("opengllogo.png"), pos);
			break;
		case irr.EDT_DIRECT3D8:
		case irr.EDT_DIRECT3D9:
			gui.addImage(driver.getTexture("directxlogo.png"), pos);
			break;
	}

	/*
	We have done everything, so lets draw it. We also write the current
	frames per second and the drawn primitives to the caption of the
	window. The 'if (device.isWindowActive())' line is optional, but
	prevents the engine render to set the position of the mouse cursor
	after task switching when other program are active.
	*/
	var lastFPS = -1;

	while(device.run())
	if (device.isWindowActive())
	{
		driver.beginScene(true, true, new irr.SColor(255,20,20,40));
		smgr.drawAll();
		gui.drawAll();
		driver.endScene();

		var fps = driver.getFPS();
		//if (lastFPS != fps)
		{
			var attr = smgr.getParameters();
			var str = "Q3 [";
			str = str + driver.getName();
			str = str + "] FPS:";
			str = str + fps;
			str = str + " Cull:";
			str = str + attr.getAttributeAsInt("calls");
			str = str + "/";
			str = str + attr.getAttributeAsInt("culled");
			str = str + " Draw: ";
			str = str + attr.getAttributeAsInt("drawn_solid");
			str = str + "/";
			str = str + attr.getAttributeAsInt("drawn_transparent");
			str = str + "/";
			str = str + attr.getAttributeAsInt("drawn_transparent_effect");

			device.setWindowCaption(str);
			lastFPS = fps;
		}
	}

	/*
	In the end, delete the Irrlicht device.
	*/
	device.drop();

	return 0;
}

start();
