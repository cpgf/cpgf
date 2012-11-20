cpgf._import("cpgf", "builtin.debug");


def overrideEventReceiver(receiver, room, env, driver) :
	Room = room;
	Driver = driver;

	skin = env.getSkin();
	font = env.getFont("../../media/fonthaettenschweiler.bmp");
	if font :
		skin.setFont(font);

	window = env.addWindow(irr.rect_s32(460,375,630,470), False, "Use 'E' + 'R' to change");

	ListBox = env.addListBox(irr.rect_s32(2,22,165,88), window);

	ListBox.addItem("Diffuse");
	ListBox.addItem("Bump mapping");
	ListBox.addItem("Parallax mapping");
	ListBox.setSelected(1);

	ProblemText = env.addStaticText("Your hardware or this renderer is not able to use the needed shaders for this material. Using fall back materials.", irr.rect_s32(150,20,470,80));

	ProblemText.setOverrideColor(irr.SColor(100,255,255,255));

	renderer = Driver.getMaterialRenderer(irr.EMT_PARALLAX_MAP_SOLID);
	if renderer and renderer.getRenderCapability() == 0 :
		ListBox.setSelected(2);

	def setMaterial() :
		t = irr.EMT_SOLID;

		index = ListBox.getSelected();
		if index == 0 :
			t = irr.EMT_SOLID;

		elif index == 1 :
			t = irr.EMT_NORMAL_MAP_SOLID;

		elif index == 2 :
			t = irr.EMT_PARALLAX_MAP_SOLID;

		Room.setMaterialType(t);

		renderer = Driver.getMaterialRenderer(t);

		if not renderer or renderer.getRenderCapability() != 0 :
			ProblemText.setVisible(True);
		else :
			ProblemText.setVisible(False);

	setMaterial();
	
	def OnEvent(me, event) :
		if event.EventType == irr.EET_KEY_INPUT_EVENT and not event.KeyInput.PressedDown and Room and ListBox :

			sel = ListBox.getSelected();
			if event.KeyInput.Key == irr.KEY_KEY_R :
				sel = sel + 1;
			elif event.KeyInput.Key == irr.KEY_KEY_E :
				sel = sel - 1;
			else :
				return False;

			if sel > 2 :
				sel = 0;
			if sel < 0 :
				sel = 2;
			ListBox.setSelected(sel);

			setMaterial();

		return False;

	receiver.OnEvent = OnEvent;

def start() :
	driverType=irr.driverChoiceConsole();
	if driverType==irr.EDT_COUNT :
		return 1;

	device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480));

	if device == None :
		return 1;

	driver = device.getVideoDriver();
	smgr = device.getSceneManager();
	env = device.getGUIEnvironment();

	driver.setTextureCreationFlag(irr.ETCF_ALWAYS_32_BIT, True);

	env.addImage(driver.getTexture("../../media/irrlichtlogo2.png"), irr.position2d_s32(10,10));

	camera = smgr.addCameraSceneNodeFPS();
	camera.setPosition(irr.vector3df(-200,200,-200));

	device.getCursorControl().setVisible(False);

	driver.setFog(irr.SColor(0,138,125,81), irr.EFT_FOG_LINEAR, 250, 1000, 0.003, True, False);

	roomMesh = smgr.getMesh("../../media/room.3ds");
	room = None;

	if roomMesh :
		smgr.getMeshManipulator().makePlanarTextureMapping(roomMesh.getMesh(0), 0.003);

		normalMap = driver.getTexture("../../media/rockwall_height.bmp");

		if normalMap :
			driver.makeNormalMapTexture(normalMap, 9.0);

		tangentMesh = smgr.getMeshManipulator().createMeshWithTangents(roomMesh.getMesh(0));

		room = smgr.addMeshSceneNode(tangentMesh);
		room.setMaterialTexture(0, driver.getTexture("../../media/rockwall.jpg"));
		room.setMaterialTexture(1, normalMap);

		room.getMaterial(0).SpecularColor.set(0,0,0,0);

		room.setMaterialFlag(irr.EMF_FOG_ENABLE, True);
		room.setMaterialType(irr.EMT_PARALLAX_MAP_SOLID);
		room.getMaterial(0).MaterialTypeParam = 0.035;

		tangentMesh.drop();

	earthMesh = smgr.getMesh("../../media/earth.x");

	if earthMesh :
		manipulator = smgr.getMeshManipulator();

		tangentSphereMesh =	manipulator.createMeshWithTangents(earthMesh.getMesh(0));

		manipulator.setVertexColorAlpha(tangentSphereMesh, 200);

		m = irr.matrix4();
		m.setScale (irr.vector3df(50,50,50) );
		manipulator.transformMesh( tangentSphereMesh, m );

		sphere = smgr.addMeshSceneNode(tangentSphereMesh);

		sphere.setPosition(irr.vector3df(-70,130,45));

		earthNormalMap = driver.getTexture("../../media/earthbump.jpg");
		if earthNormalMap :
			driver.makeNormalMapTexture(earthNormalMap, 20.0);
			sphere.setMaterialTexture(1, earthNormalMap);
			sphere.setMaterialType(irr.EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA);

		sphere.setMaterialFlag(irr.EMF_FOG_ENABLE, True);

		anim = smgr.createRotationAnimator(irr.vector3df(0,0.1,0));
		sphere.addAnimator(anim);
		anim.drop();

		tangentSphereMesh.drop();


	light1 = smgr.addLightSceneNode(None, irr.vector3df(0,0,0), irr.SColorf(0.5, 1.0, 0.5, 0.0), 800.0);

	light1.setDebugDataVisible ( irr.EDS_BBOX );


	anim = smgr.createFlyCircleAnimator (irr.vector3df(50,300,0),190.0, -0.003);
	light1.addAnimator(anim);
	anim.drop();

	bill = smgr.addBillboardSceneNode(light1, irr.dimension2d_f32(60, 60));

	bill.setMaterialFlag(irr.EMF_LIGHTING, False);
	bill.setMaterialFlag(irr.EMF_ZWRITE_ENABLE, False);
	bill.setMaterialType(irr.EMT_TRANSPARENT_ADD_COLOR);
	bill.setMaterialTexture(0, driver.getTexture("../../media/particlered.bmp"));

	light2 = smgr.addLightSceneNode(None, irr.vector3df(0,0,0), irr.SColorf(1.0, 0.2, 0.2, 0.0), 800.0);

	anim = smgr.createFlyCircleAnimator(irr.vector3df(0,150,0), 200.0, 0.001, irr.vector3df(0.2, 0.9, 0.0));
	light2.addAnimator(anim);
	anim.drop();

	bill = smgr.addBillboardSceneNode(light2, irr.dimension2d_f32(120, 120));
	bill.setMaterialFlag(irr.EMF_LIGHTING, False);
	bill.setMaterialFlag(irr.EMF_ZWRITE_ENABLE, False);
	bill.setMaterialType(irr.EMT_TRANSPARENT_ADD_COLOR);
	bill.setMaterialTexture(0, driver.getTexture("../../media/particlewhite.bmp"));

	ps = smgr.addParticleSystemSceneNode(False, light2);

	em = ps.createBoxEmitter(irr.aabbox3d_f32(-3,0,-3,3,1,3), irr.vector3df(0.0,0.03,0.0), 80,100, irr.SColor(0,255,255,255), irr.SColor(0,255,255,255), 400,1100);
	em.setMinStartSize(irr.dimension2d_f32(30.0, 40.0));
	em.setMaxStartSize(irr.dimension2d_f32(30.0, 40.0));

	ps.setEmitter(em);
	em.drop();

	paf = ps.createFadeOutParticleAffector();
	ps.addAffector(paf);
	paf.drop();

	ps.setMaterialFlag(irr.EMF_LIGHTING, False);
	ps.setMaterialFlag(irr.EMF_ZWRITE_ENABLE, False);
	ps.setMaterialTexture(0, driver.getTexture("../../media/fireball.bmp"));
	ps.setMaterialType(irr.EMT_TRANSPARENT_VERTEX_ALPHA);


	receiver = irr.IEventReceiverWrapper();
	overrideEventReceiver(receiver, room, env, driver);
	device.setEventReceiver(receiver);

	lastFPS = -1;

	while device.run() :
		if device.isWindowActive() :
			driver.beginScene(True, True, 0);

			smgr.drawAll();
			env.drawAll();

			driver.endScene();

			fps = driver.getFPS();

			if lastFPS != fps :
				tmp = "Per pixel lighting example - cpgf Irrlicht Python Binding [";
				tmp = tmp + driver.getName();
				tmp = tmp + "] FPS:";
				tmp = tmp + str(fps);

				device.setWindowCaption(tmp);
				lastFPS = fps;

	device.drop();

	return 0;

start();
