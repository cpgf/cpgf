function start()
{
	var shadows = true;

	// ask user for driver
	var driverType = irr.driverChoiceConsole();
	if(driverType == irr.EDT_COUNT)
		return 1;

	/*
	Create device and exit if creation failed. We make the stencil flag
	optional to avoid slow screen modes for runs without shadows.
	*/

	var device = irr.createDevice(driverType, new irr.dimension2d_u32(640, 480), 16, false, shadows);

	if (device == 0)
		return 1; // could not create selected driver.

	var driver = device.getVideoDriver();
	var smgr = device.getSceneManager();

	/*
	For our environment, we load a .3ds file. It is a small room I modelled
	with Anim8or and exported into the 3ds format because the Irrlicht
	Engine does not support the .an8 format. I am a very bad 3d graphic
	artist, and so the texture mapping is not very nice in this model.
	Luckily I am a better programmer than artist, and so the Irrlicht
	Engine is able to create a cool texture mapping for me: Just use the
	mesh manipulator and create a planar texture mapping for the mesh. If
	you want to see the mapping I made with Anim8or, uncomment this line. I
	also did not figure out how to set the material right in Anim8or, it
	has a specular light color which I don't really like. I'll switch it
	off too with this code.
	*/

	var mesh = smgr.getMesh("../../media/room.3ds");

	smgr.getMeshManipulator().makePlanarTextureMapping(mesh.getMesh(0), 0.004);

	var node = null;

	node = smgr.addAnimatedMeshSceneNode(mesh);
	node.setMaterialTexture(0, driver.getTexture("../../media/wall.jpg"));
	node.getMaterial(0).SpecularColor.set(0,0,0,0);

	/*
	Now, for the first special effect: Animated water. It works like this:
	The WaterSurfaceSceneNode takes a mesh as input and makes it wave like
	a water surface. And if we let this scene node use a nice material like
	the EMT_REFLECTION_2_LAYER, it looks really cool. We are doing this
	with the next few lines of code. As input mesh, we create a hill plane
	mesh, without hills. But any other mesh could be used for this, you
	could even use the room.3ds (which would look really strange) if you
	want to.
	*/

	mesh = smgr.addHillPlaneMesh( "myHill", new irr.dimension2d_f32(20,20), new irr.dimension2d_u32(40,40), 0, 0, new irr.dimension2d_f32(0,0), new irr.dimension2d_f32(10,10));

	node = smgr.addWaterSurfaceSceneNode(mesh.getMesh(0), 3.0, 300.0, 30.0);
	node.setPosition(new irr.vector3df(0,7,0));

	node.setMaterialTexture(0, driver.getTexture("../../media/stones.jpg"));
	node.setMaterialTexture(1, driver.getTexture("../../media/water.jpg"));

	node.setMaterialType(irr.EMT_REFLECTION_2_LAYER);

	/*
	The second special effect is very basic, I bet you saw it already in
	some Irrlicht Engine demos: A transparent billboard combined with a
	dynamic light. We simply create a light scene node, let it fly around,
	and to make it look more cool, we attach a billboard scene node to it.
	*/

	// create light

	node = smgr.addLightSceneNode(0, new irr.vector3df(0,0,0), new irr.SColorf(1.0, 0.6, 0.7, 1.0), 800.0);
	var anim = 0;
	anim = smgr.createFlyCircleAnimator (new irr.vector3df(0,150,0),250.0);
	node.addAnimator(anim);
	anim.drop();

	// attach billboard to light

	node = smgr.addBillboardSceneNode(node, new irr.dimension2d_f32(50, 50));
	node.setMaterialFlag(irr.EMF_LIGHTING, false);
	node.setMaterialType(irr.EMT_TRANSPARENT_ADD_COLOR);
	node.setMaterialTexture(0, driver.getTexture("../../media/particlewhite.bmp"));

	/*
	The next special effect is a lot more interesting: A particle system.
	The particle system in the Irrlicht Engine is quite modular and
	extensible, but yet easy to use. There is a particle system scene node
	into which you can put a particle emitter, which makes particles come out
	of nothing. These emitters are quite flexible and usually have lots of
	parameters like direction, amount, and color of the particles they
	create.

	There are different emitters, for example a point emitter which lets
	particles pop out at a fixed point. If the particle emitters available
	in the engine are not enough for you, you can easily create your own
	ones, you'll simply have to create a class derived from the
	IParticleEmitter interface and attach it to the particle system using
	setEmitter(). In this example we create a box particle emitter, which
	creates particles randomly inside a box. The parameters define the box,
	direction of the particles, minimal and maximal new particles per
	second, color, and minimal and maximal lifetime of the particles.

	Because only with emitters particle system would be a little bit
	boring, there are particle affectors which modify particles while
	they fly around. Affectors can be added to a particle system for
	simulating additional effects like gravity or wind.
	The particle affector we use in this example is an affector which
	modifies the color of the particles: It lets them fade out. Like the
	particle emitters, additional particle affectors can also be
	implemented by you, simply derive a class from IParticleAffector and
	add it with addAffector().

	After we set a nice material to the particle system, we have a cool
	looking camp fire. By adjusting material, texture, particle emitter,
	and affector parameters, it is also easily possible to create smoke,
	rain, explosions, snow, and so on.
	*/

	// create a particle system

	var ps = smgr.addParticleSystemSceneNode(false);

	var em = ps.createBoxEmitter(new irr.aabbox3d_f32(-7,0,-7,7,1,7), new irr.vector3df(0.0,0.06,0.0), 80,100, new irr.SColor(0,255,255,255), new irr.SColor(0,255,255,255),	800,2000,0, new irr.dimension2df(10.0,10.0), new irr.dimension2df(20.0,20.0));

	ps.setEmitter(em); // this grabs the emitter
	em.drop(); // so we can drop it here without deleting it

	var paf = ps.createFadeOutParticleAffector();

	ps.addAffector(paf); // same goes for the affector
	paf.drop();

	ps.setPosition(new irr.vector3df(-70,60,40));
	ps.setScale(new irr.vector3df(2,2,2));
	ps.setMaterialFlag(irr.EMF_LIGHTING, false);
	ps.setMaterialFlag(irr.EMF_ZWRITE_ENABLE, false);
	ps.setMaterialTexture(0, driver.getTexture("../../media/fire.bmp"));
	ps.setMaterialType(irr.EMT_TRANSPARENT_VERTEX_ALPHA);

	/*
	Next we add a volumetric light node, which adds a glowing fake area light to
	the scene. Like with the billboards and particle systems we also assign a
	texture for the desired effect, though this time we'll use a texture animator
	to create the illusion of a magical glowing area effect.
	*/
	var n = smgr.addVolumeLightSceneNode(0, -1, 32, 32,	new irr.SColor(0, 255, 255, 255), new irr.SColor(0, 0, 0, 0));

/*	
	if (n)
	{
		n.setScale(new irr.vector3df(56.0, 56.0, 56.0));
		n.setPosition(new irr.vector3df(-120,50,40));

		// load textures for animation
		var textures;
		for (var g=7; g > 0; --g)
		{
			var tmp;
			tmp = "../../media/portal";
			tmp = tmp + g;
			tmp = tmp + ".bmp";
			var t = driver.getTexture( tmp.c_str() );
			textures.push_back(t);
		}

		// create texture animator
		scene::ISceneNodeAnimator* glow = smgr.createTextureAnimator(textures, 150);

		// add the animator
		n.addAnimator(glow);

		// drop the animator because it was created with a create() function
		glow.drop();
	}
*/

	/*
	As our last special effect, we want a dynamic shadow be casted from an
	animated character. For this we load a DirectX .x model and place it
	into our world. For creating the shadow, we simply need to call
	addShadowVolumeSceneNode(). The color of shadows is only adjustable
	globally for all shadows, by calling ISceneManager::setShadowColor().
	Voila, here is our dynamic shadow.

	Because the character is a little bit too small for this scene, we make
	it bigger using setScale(). And because the character is lighted by a
	dynamic light, we need to normalize the normals to make the lighting on
	it correct. This is always necessary if the scale of a dynamic lighted
	model is not (1,1,1). Otherwise it would get too dark or too bright
	because the normals will be scaled too.
	*/

	// add animated character

	mesh = smgr.getMesh("../../media/dwarf.x");
	var anode = null;

	anode = smgr.addAnimatedMeshSceneNode(mesh);
	anode.setPosition(new irr.vector3df(-50,20,-60));
	anode.setAnimationSpeed(15);

	// add shadow
	anode.addShadowVolumeSceneNode();
	smgr.setShadowColor(new irr.SColor(150,0,0,0));

	// make the model a little bit bigger and normalize its normals
	// because of the scaling, for correct lighting
	anode.setScale(new irr.vector3df(2,2,2));
	anode.setMaterialFlag(irr.EMF_NORMALIZE_NORMALS, true);

	/*
	Finally we simply have to draw everything, that's all.
	*/

	var camera = smgr.addCameraSceneNodeFPS();
	camera.setPosition(new irr.vector3df(-50,50,-150));

	// disable mouse cursor
	device.getCursorControl().setVisible(false);

	var lastFPS = -1;

	while(device.run())
	if (device.isWindowActive())
	{
		driver.beginScene(true, true, 0);

		smgr.drawAll();

		driver.endScene();

		var fps = driver.getFPS();

		if (lastFPS != fps)
		{
			var str = "cpgf Irrlicht JavaScript Binding - SpecialFX example [";
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
