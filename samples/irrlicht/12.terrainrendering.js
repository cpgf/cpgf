function overrideEventReceiver(receiver, terrain, skybox, skydome)
{
	this.Terrain = terrain;
	this.Skybox = skybox;
	this.Skydome = skydome;
	this.showBox = true;
	
	this.Skybox.setVisible(true);
	this.Skydome.setVisible(false);

	var self = this;
	receiver.OnEvent = function(me, event)
	{
		// check if user presses the key 'W' or 'D'
		if (event.EventType == irr.EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{
			case irr.KEY_KEY_W: // switch wire frame mode
				self.Terrain.setMaterialFlag(irr.EMF_WIREFRAME, !self.Terrain.getMaterial(0).Wireframe);
				self.Terrain.setMaterialFlag(irr.EMF_POINTCLOUD, false);
				return true;
			case irr.KEY_KEY_P: // switch wire frame mode
				self.Terrain.setMaterialFlag(irr.EMF_POINTCLOUD, !self.Terrain.getMaterial(0).PointCloud);
				self.Terrain.setMaterialFlag(irr.EMF_WIREFRAME, false);
				return true;
			case irr.KEY_KEY_D: // toggle detail map
				self.Terrain.setMaterialType(self.Terrain.getMaterial(0).MaterialType == irr.EMT_SOLID ? irr.EMT_DETAIL_MAP : irr.EMT_SOLID);
				return true;
			case irr.KEY_KEY_S: // toggle skies
				self.showBox=!self.showBox;
				self.Skybox.setVisible(self.showBox);
				self.Skydome.setVisible(!self.showBox);
				return true;
			default:
				break;
			}
		}

		return false;
	}
}


/*
The start of the main function starts like in most other example. We ask the
user for the desired renderer and start it up. This time with the advanced
parameter handling.
*/
function start()
{
	// ask user for driver
	var driverType=irr.driverChoiceConsole();
	if (driverType==irr.EDT_COUNT)
		return 1;

	// create device with full flexibility over creation parameters
	// you can add more parameters if desired, check irr.SIrrlichtCreationParameters
	var params = new irr.SIrrlichtCreationParameters();
	params.DriverType=driverType;
	params.WindowSize=new irr.dimension2d_u32(640, 480);
	var device = irr.createDeviceEx(params);

	if (device == 0)
		return 1; // could not create selected driver.

	
	/*
	First, we add standard stuff to the scene: A nice irrlicht engine
	logo, a small help text, a user controlled camera, and we disable
	the mouse cursor.
	*/

	var driver = device.getVideoDriver();
	var smgr = device.getSceneManager();
	var env = device.getGUIEnvironment();

	driver.setTextureCreationFlag(irr.ETCF_ALWAYS_32_BIT, true);

	// add irrlicht logo
	env.addImage(driver.getTexture("../../media/irrlichtlogo2.png"), new irr.position2d_s32(10,10));

	//set other font
	env.getSkin().setFont(env.getFont("../../media/fontlucida.png"));

	// add some help text
	env.addStaticText("Press 'W' to change wireframe mode\nPress 'D' to toggle detail map\nPress 'S' to toggle skybox/skydome",	new irr.rect_s32(10,421,250,475), true, true, 0, -1, true);

	// add camera
	var camera = smgr.addCameraSceneNodeFPS(0,100.0,1.2);

	camera.setPosition(new irr.vector3df(2700*2,255*2,2600*2));
	camera.setTarget(new irr.vector3df(2397*2,343*2,2700*2));
	camera.setFarValue(42000.0);

	// disable mouse cursor
	device.getCursorControl().setVisible(false);

	/*
	Here comes the terrain renderer scene node: We add it just like any
	other scene node to the scene using
	ISceneManager::addTerrainSceneNode(). The only parameter we use is a
	file name to the heightmap we use. A heightmap is simply a gray scale
	texture. The terrain renderer loads it and creates the 3D terrain from
	it.

	To make the terrain look more big, we change the scale factor of
	it to (40, 4.4, 40). Because we don't have any dynamic lights in the
	scene, we switch off the lighting, and we set the file
	terrain-texture.jpg as texture for the terrain and detailmap3.jpg as
	second texture, called detail map. At last, we set the scale values for
	the texture: The first texture will be repeated only one time over the
	whole terrain, and the second one (detail map) 20 times.
	*/

	// add terrain scene node
	var terrain = smgr.addTerrainSceneNode("../../media/terrain-heightmap.bmp", 0, -1, new irr.vector3df(0.0, 0.0, 0.0), new irr.vector3df(0.0, 0.0, 0.0), new irr.vector3df(40.0, 4.40, 40.0), new irr.SColor ( 255, 255, 255, 255 ), 5, irr.ETPS_17,4);

	terrain.setMaterialFlag(irr.EMF_LIGHTING, false);

	terrain.setMaterialTexture(0, driver.getTexture("../../media/terrain-texture.jpg"));
	terrain.setMaterialTexture(1, driver.getTexture("../../media/detailmap3.jpg"));
	
	terrain.setMaterialType(irr.EMT_DETAIL_MAP);

	terrain.scaleTexture(1.0, 20.0);
	//terrain.setDebugDataVisible ( true );

	/*
	To be able to do collision with the terrain, we create a triangle selector.
	If you want to know what triangle selectors do, just take a look into the
	collision tutorial. The terrain triangle selector works together with the
	terrain. To demonstrate this, we create a collision response animator
	and attach it to the camera, so that the camera will not be able to fly
	through the terrain.
	*/

	// create triangle selector for the terrain	
	var selector = smgr.createTerrainTriangleSelector(terrain, 0);
	terrain.setTriangleSelector(selector);

	// create collision response animator and attach it to the camera
	var anim = smgr.createCollisionResponseAnimator(selector, camera, new irr.vector3df(60,100,60),	new irr.vector3df(0,0,0), new irr.vector3df(0,50,0));
	selector.drop();
	camera.addAnimator(anim);
	anim.drop();

	/* If you need access to the terrain data you can also do this directly via the following code fragment.
	*/
	var buffer = new irr.CDynamicMeshBuffer(irr.EVT_2TCOORDS, irr.EIT_16BIT);
	terrain.getMeshBufferForLOD(buffer, 0);
	var data = buffer.getVertexBuffer().getData();
	// Work on data or get the IndexBuffer with a similar call.
	// Don't drop it, the script will GC it.
	//buffer.drop(); // When done drop the buffer again.

	/*
	To make the user be able to switch between normal and wireframe mode,
	we create an instance of the event reciever from above and let Irrlicht
	know about it. In addition, we add the skybox which we already used in
	lots of Irrlicht examples and a skydome, which is shown mutually
	exclusive with the skybox by pressing 'S'.
	*/

	// create skybox and skydome
	driver.setTextureCreationFlag(irr.ETCF_CREATE_MIP_MAPS, false);

	var skybox=smgr.addSkyBoxSceneNode(driver.getTexture("../../media/irrlicht2_up.jpg"), driver.getTexture("../../media/irrlicht2_dn.jpg"), driver.getTexture("../../media/irrlicht2_lf.jpg"), driver.getTexture("../../media/irrlicht2_rt.jpg"), driver.getTexture("../../media/irrlicht2_ft.jpg"), driver.getTexture("../../media/irrlicht2_bk.jpg"));
	var skydome=smgr.addSkyDomeSceneNode(driver.getTexture("../../media/skydome.jpg"),16,8,0.95,2.0);

	driver.setTextureCreationFlag(irr.ETCF_CREATE_MIP_MAPS, true);

	// create event receiver
	var receiver = new irr.IEventReceiverWrapper();
	overrideEventReceiver(receiver, terrain, skybox, skydome);
	device.setEventReceiver(receiver);

	/*
	That's it, draw everything.
	*/

	var lastFPS = -1;

	while(device.run())
	if (device.isWindowActive())
	{
		driver.beginScene(true, true, 0 );

		smgr.drawAll();
		env.drawAll();

		driver.endScene();

		// display frames per second in window title
		var fps = driver.getFPS();
		if (lastFPS != fps)
		{
			var str = "Terrain Renderer - cpgf Irrlicht JavaScript Binding [";
			str = str + driver.getName();
			str = str + "] FPS:";
			str = str + fps;
			// Also print terrain height of current camera position
			// We can use camera position because terrain is located at coordinate origin
			str = str + " Height: ";
			str = str + terrain.getHeight(camera.getAbsolutePosition().X, camera.getAbsolutePosition().Z);

			device.setWindowCaption(str);
			lastFPS = fps;
		}
	}

	device.drop();
	
	return 0;
}

start();
