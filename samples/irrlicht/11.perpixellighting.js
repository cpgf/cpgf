cpgf.import("cpgf", "builtin.debug");


function overrideEventReceiver(receiver, room, env, driver)
{
	// store pointer to room so we can change its drawing mode
	this.Room = room;
	this.Driver = driver;

	// set a nicer font
	var skin = env.getSkin();
	var font = env.getFont("../../media/fonthaettenschweiler.bmp");
	if (font)
		skin.setFont(font);

	// add window and listbox
	var window = env.addWindow(new irr.rect_s32(460,375,630,470), false, "Use 'E' + 'R' to change");

	this.ListBox = env.addListBox(new irr.rect_s32(2,22,165,88), window);

	this.ListBox.addItem("Diffuse");
	this.ListBox.addItem("Bump mapping");
	this.ListBox.addItem("Parallax mapping");
	this.ListBox.setSelected(1);

	// create problem text
	this.ProblemText = env.addStaticText("Your hardware or this renderer is not able to use the needed shaders for this material. Using fall back materials.", new irr.rect_s32(150,20,470,80));

	this.ProblemText.setOverrideColor(new irr.SColor(100,255,255,255));

	// set start material (prefer parallax mapping if available)
	var renderer = this.Driver.getMaterialRenderer(irr.EMT_PARALLAX_MAP_SOLID);
	if (renderer && renderer.getRenderCapability() == 0)
		this.ListBox.setSelected(2);

	var self = this;
	// sets the material of the room mesh the the one set in the
	// list box.
	this.setMaterial = function()
	{
		var type = irr.EMT_SOLID;

		// change material setting
		switch(self.ListBox.getSelected())
		{
		case 0: type = irr.EMT_SOLID;
			break;
		case 1: type = irr.EMT_NORMAL_MAP_SOLID;
			break;
		case 2: type = irr.EMT_PARALLAX_MAP_SOLID;
			break;
		}

		self.Room.setMaterialType(type);

		/*
		We need to add a warning if the materials will not be able to
		be displayed 100% correctly. This is no problem, they will be
		renderered using fall back materials, but at least the user
		should know that it would look better on better hardware. We
		simply check if the material renderer is able to draw at full
		quality on the current hardware. The
		IMaterialRenderer::getRenderCapability() returns 0 if this is
		the case.
		*/
		var renderer = self.Driver.getMaterialRenderer(type);

		// display some problem text when problem
		if (!renderer || renderer.getRenderCapability() != 0)
			self.ProblemText.setVisible(true);
		else
			self.ProblemText.setVisible(false);
	}

	// set the material which is selected in the listbox
	this.setMaterial();
	
	receiver.OnEvent = function(me, event)
	{
		// check if user presses the key 'E' or 'R'
		if (event.EventType == irr.EET_KEY_INPUT_EVENT &&!event.KeyInput.PressedDown && self.Room && self.ListBox)
		{
			// change selected item in listbox

			var sel = self.ListBox.getSelected();
			if (event.KeyInput.Key == irr.KEY_KEY_R)
				++sel;
			else
			if (event.KeyInput.Key == irr.KEY_KEY_E)
				--sel;
			else
				return false;

			if (sel > 2) sel = 0;
			if (sel < 0) sel = 2;
			self.ListBox.setSelected(sel);

			// set the material which is selected in the listbox
			self.setMaterial();
		}

		return false;
	}

}


/*
Now for the real fun. We create an Irrlicht Device and start to setup the scene.
*/
function start()
{
	// ask user for driver
	var driverType=irr.driverChoiceConsole();
	if (driverType==irr.EDT_COUNT)
		return 1;

	// create device

	var device = irr.createDevice(driverType, new irr.dimension2d_u32(640, 480));

	if (device == null)
		return 1; // could not create selected driver.

	/*
	Before we start with the interesting stuff, we do some simple things:
	Store pointers to the most important parts of the engine (video driver,
	scene manager, gui environment) to safe us from typing too much, add an
	irrlicht engine logo to the window and a user controlled first person
	shooter style camera. Also, we let the engine know that it should store
	all textures in 32 bit. This necessary because for parallax mapping, we
	need 32 bit textures.
	*/

	var driver = device.getVideoDriver();
	var smgr = device.getSceneManager();
	var env = device.getGUIEnvironment();

	driver.setTextureCreationFlag(irr.ETCF_ALWAYS_32_BIT, true);

	// add irrlicht logo
	env.addImage(driver.getTexture("../../media/irrlichtlogo2.png"), new irr.position2d_s32(10,10));

	// add camera
	var camera = smgr.addCameraSceneNodeFPS();
	camera.setPosition(new irr.vector3df(-200,200,-200));

	// disable mouse cursor
	device.getCursorControl().setVisible(false);


	/*
	Because we want the whole scene to look a little bit scarier, we add
	some fog to it. This is done by a call to IVideoDriver::setFog(). There
	you can set various fog settings. In this example, we use pixel fog,
	because it will work well with the materials we'll use in this example.
	Please note that you will have to set the material flag EMF_FOG_ENABLE
	to 'true' in every scene node which should be affected by this fog.
	*/
	driver.setFog(new irr.SColor(0,138,125,81), irr.EFT_FOG_LINEAR, 250, 1000, 0.003, true, false);

	/*
	To be able to display something interesting, we load a mesh from a .3ds
	file which is a room I modeled with anim8or. It is the same room as
	from the specialFX example. Maybe you remember from that tutorial, I am
	no good modeler at all and so I totally messed up the texture mapping
	in this model, but we can simply repair it with the
	IMeshManipulator::makePlanarTextureMapping() method.
	*/

	var roomMesh = smgr.getMesh("../../media/room.3ds");
	var room = null;

	if (roomMesh)
	{
		smgr.getMeshManipulator().makePlanarTextureMapping(roomMesh.getMesh(0), 0.003);

		/*
		Now for the first exciting thing: If we successfully loaded the
		mesh we need to apply textures to it. Because we want this room
		to be displayed with a very cool material, we have to do a
		little bit more than just set the textures. Instead of only
		loading a color map as usual, we also load a height map which
		is simply a grayscale texture. From this height map, we create
		a normal map which we will set as second texture of the room.
		If you already have a normal map, you could directly set it,
		but I simply didn't find a nice normal map for this texture.
		The normal map texture is being generated by the
		makeNormalMapTexture method of the VideoDriver. The second
		parameter specifies the height of the heightmap. If you set it
		to a bigger value, the map will look more rocky.
		*/

		var normalMap = driver.getTexture("../../media/rockwall_height.bmp");

		if (normalMap)
			driver.makeNormalMapTexture(normalMap, 9.0);

		/*
		But just setting color and normal map is not everything. The
		material we want to use needs some additional informations per
		vertex like tangents and binormals. Because we are too lazy to
		calculate that information now, we let Irrlicht do this for us.
		That's why we call IMeshManipulator::createMeshWithTangents().
		It creates a mesh copy with tangents and binormals from another
		mesh. After we've done that, we simply create a standard
		mesh scene node with this mesh copy, set color and normal map
		and adjust some other material settings. Note that we set
		EMF_FOG_ENABLE to true to enable fog in the room.
		*/

		var tangentMesh = smgr.getMeshManipulator().createMeshWithTangents(roomMesh.getMesh(0));

		room = smgr.addMeshSceneNode(tangentMesh);
		room.setMaterialTexture(0, driver.getTexture("../../media/rockwall.jpg"));
		room.setMaterialTexture(1, normalMap);

		room.getMaterial(0).SpecularColor.set(0,0,0,0);

		room.setMaterialFlag(irr.EMF_FOG_ENABLE, true);
		room.setMaterialType(irr.EMT_PARALLAX_MAP_SOLID);
		// adjust height for parallax effect
		room.getMaterial(0).MaterialTypeParam = 0.035;

		// drop mesh because we created it with a create.. call.
		tangentMesh.drop();
	}

	/*
	After we've created a room shaded by per pixel lighting, we add a
	sphere into it with the same material, but we'll make it transparent.
	In addition, because the sphere looks somehow like a familiar planet,
	we make it rotate. The procedure is similar as before. The difference
	is that we are loading the mesh from an .x file which already contains
	a color map so we do not need to load it manually. But the sphere is a
	little bit too small for our needs, so we scale it by the factor 50.
	*/

	// add earth sphere

	var earthMesh = smgr.getMesh("../../media/earth.x");

	if (earthMesh)
	{
		//perform various task with the mesh manipulator
		var manipulator = smgr.getMeshManipulator();

		// create mesh copy with tangent informations from original earth.x mesh
		var tangentSphereMesh =	manipulator.createMeshWithTangents(earthMesh.getMesh(0));

		// set the alpha value of all vertices to 200
		manipulator.setVertexColorAlpha(tangentSphereMesh, 200);

		// scale the mesh by factor 50
		var m = new irr.matrix4();
		m.setScale (new irr.vector3df(50,50,50) );
		manipulator.transformMesh( tangentSphereMesh, m );

		var sphere = smgr.addMeshSceneNode(tangentSphereMesh);

		sphere.setPosition(new irr.vector3df(-70,130,45));

		// load heightmap, create normal map from it and set it
		var earthNormalMap = driver.getTexture("../../media/earthbump.jpg");
		if (earthNormalMap)
		{
			driver.makeNormalMapTexture(earthNormalMap, 20.0);
			sphere.setMaterialTexture(1, earthNormalMap);
			sphere.setMaterialType(irr.EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA);
		}

		// adjust material settings
		sphere.setMaterialFlag(irr.EMF_FOG_ENABLE, true);

		// add rotation animator
		var anim = smgr.createRotationAnimator(new irr.vector3df(0,0.1,0));
		sphere.addAnimator(anim);
		anim.drop();

		// drop mesh because we created it with a create.. call.
		tangentSphereMesh.drop();
	}

	/*
	Per pixel lighted materials only look cool when there are moving
	lights. So we add some. And because moving lights alone are so boring,
	we add billboards to them, and a whole particle system to one of them.
	We start with the first light which is red and has only the billboard
	attached.
	*/

	// add light 1 (nearly red)
	var light1 = smgr.addLightSceneNode(0, new irr.vector3df(0,0,0), new irr.SColorf(0.5, 1.0, 0.5, 0.0), 800.0);

	light1.setDebugDataVisible ( irr.EDS_BBOX );


	// add fly circle animator to light 1
	var anim = smgr.createFlyCircleAnimator (new irr.vector3df(50,300,0),190.0, -0.003);
	light1.addAnimator(anim);
	anim.drop();

	// attach billboard to the light
	var bill = smgr.addBillboardSceneNode(light1, new irr.dimension2d_f32(60, 60));

	bill.setMaterialFlag(irr.EMF_LIGHTING, false);
	bill.setMaterialFlag(irr.EMF_ZWRITE_ENABLE, false);
	bill.setMaterialType(irr.EMT_TRANSPARENT_ADD_COLOR);
	bill.setMaterialTexture(0, driver.getTexture("../../media/particlered.bmp"));

	/*
	Now the same again, with the second light. The difference is that we
	add a particle system to it too. And because the light moves, the
	particles of the particlesystem will follow. If you want to know more
	about how particle systems are created in Irrlicht, take a look at the
	specialFx example. Maybe you will have noticed that we only add 2
	lights, this has a simple reason: The low end version of this material
	was written in ps1.1 and vs1.1, which doesn't allow more lights. You
	could add a third light to the scene, but it won't be used to shade the
	walls. But of course, this will change in future versions of Irrlicht
	where higher versions of pixel/vertex shaders will be implemented too.
	*/

	// add light 2 (gray)
	var light2 = smgr.addLightSceneNode(0, new irr.vector3df(0,0,0), new irr.SColorf(1.0, 0.2, 0.2, 0.0), 800.0);

	// add fly circle animator to light 2
	anim = smgr.createFlyCircleAnimator(new irr.vector3df(0,150,0), 200.0, 0.001, new irr.vector3df(0.2, 0.9, 0.0));
	light2.addAnimator(anim);
	anim.drop();

	// attach billboard to light
	bill = smgr.addBillboardSceneNode(light2, new irr.dimension2d_f32(120, 120));
	bill.setMaterialFlag(irr.EMF_LIGHTING, false);
	bill.setMaterialFlag(irr.EMF_ZWRITE_ENABLE, false);
	bill.setMaterialType(irr.EMT_TRANSPARENT_ADD_COLOR);
	bill.setMaterialTexture(0, driver.getTexture("../../media/particlewhite.bmp"));

	// add particle system
	var ps = smgr.addParticleSystemSceneNode(false, light2);

	// create and set emitter
	var em = ps.createBoxEmitter(new irr.aabbox3d_f32(-3,0,-3,3,1,3), new irr.vector3df(0.0,0.03,0.0), 80,100, new irr.SColor(0,255,255,255), new irr.SColor(0,255,255,255), 400,1100);
	em.setMinStartSize(new irr.dimension2d_f32(30.0, 40.0));
	em.setMaxStartSize(new irr.dimension2d_f32(30.0, 40.0));

	ps.setEmitter(em);
	em.drop();

	// create and set affector
	var paf = ps.createFadeOutParticleAffector();
	ps.addAffector(paf);
	paf.drop();

	// adjust some material settings
	ps.setMaterialFlag(irr.EMF_LIGHTING, false);
	ps.setMaterialFlag(irr.EMF_ZWRITE_ENABLE, false);
	ps.setMaterialTexture(0, driver.getTexture("../../media/fireball.bmp"));
	ps.setMaterialType(irr.EMT_TRANSPARENT_VERTEX_ALPHA);


	var receiver = new irr.IEventReceiverWrapper();
	overrideEventReceiver(receiver, room, env, driver);
	device.setEventReceiver(receiver);

	/*
	Finally, draw everything. That's it.
	*/

	var lastFPS = -1;

	while(device.run())
	if (device.isWindowActive())
	{
		driver.beginScene(true, true, 0);

		smgr.drawAll();
		env.drawAll();

		driver.endScene();

		var fps = driver.getFPS();

		if (lastFPS != fps)
		{
			var str = "Per pixel lighting example - cpgf Irrlicht JavaScript Binding [";
			str = str + driver.getName();
			str = str + "] FPS:";
			str = str + fps;

			device.setWindowCaption(str);
			lastFPS = fps;
		}
	}

	device.drop();

	return 0;
}

start();
