cpgf._import(None, "builtin.debug");

resourcePath = "../../media/";
ID_IsNotPickable = 0;
IDFlag_IsPickable = 1;
IDFlag_IsHighlightable = 2;

def start() :
	driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT :
		return 1;

	device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480), 16, False);

	if device == None :
		return 1;

	driver = device.getVideoDriver();
	smgr = device.getSceneManager();

	device.getFileSystem().addZipFileArchive(resourcePath + "map-20kdm2.pk3");

	q3levelmesh = smgr.getMesh("20kdm2.bsp");
	q3node = None;

	if q3levelmesh :
		q3node = smgr.addOctreeSceneNode(q3levelmesh.getMesh(0), None, IDFlag_IsPickable);
	
	selector = None;

	if q3node :
		q3node.setPosition(irr.vector3df(-1350,-130,-1400));

		selector = smgr.createOctreeTriangleSelector(q3node.getMesh(), q3node, 128);
		q3node.setTriangleSelector(selector);

	camera = smgr.addCameraSceneNodeFPS(None, 100.0, 0.3, ID_IsNotPickable, None, 0, True, 3.0);
	camera.setPosition(irr.vector3df(50,50,-60));
	camera.setTarget(irr.vector3df(-70,30,-60));

	if selector :
		anim = smgr.createCollisionResponseAnimator(selector, camera, irr.vector3df(30,50,30),	irr.vector3df(0,-10,0), irr.vector3df(0,30,0));
		selector.drop();
		camera.addAnimator(anim);
		anim.drop();

	device.getCursorControl().setVisible(False);
	
	bill = smgr.addBillboardSceneNode();
	bill.setMaterialType(irr.EMT_TRANSPARENT_ADD_COLOR );
	bill.setMaterialTexture(0, driver.getTexture(resourcePath + "particle.bmp"));
	bill.setMaterialFlag(irr.EMF_LIGHTING, False);
	bill.setMaterialFlag(irr.EMF_ZBUFFER, False);
	bill.setSize(irr.dimension2d_f32(20.0, 20.0));
	bill.setID(ID_IsNotPickable);

	node = None;

	node = smgr.addAnimatedMeshSceneNode(smgr.getMesh(resourcePath + "faerie.md2"),	None, IDFlag_IsPickable + IDFlag_IsHighlightable);
	node.setPosition(irr.vector3df(-70,-15,-120));
	node.setScale(irr.vector3df(2, 2, 2));
	node.setMD2Animation(irr.EMAT_POINT);
	node.setAnimationSpeed(20.0);
	material = irr.SMaterial();
	material.setTexture(0, driver.getTexture(resourcePath + "/faerie2.bmp"));
	material.Lighting = True;
	material.NormalizeNormals = True;

	node.getMaterial(0).setTexture(0, driver.getTexture(resourcePath + "/faerie2.bmp"));
	node.getMaterial(0).Lighting = True;
	node.getMaterial(0).NormalizeNormals = True;

	selector = smgr.createTriangleSelector(node);
	node.setTriangleSelector(selector);
	selector.drop();

	node = smgr.addAnimatedMeshSceneNode(smgr.getMesh(resourcePath + "dwarf.x"), None, IDFlag_IsPickable + IDFlag_IsHighlightable);
	node.setPosition(irr.vector3df(-70,-66,0));
	node.setRotation(irr.vector3df(0,-90,0));
	node.setAnimationSpeed(20.0);
	selector = smgr.createTriangleSelector(node);
	node.setTriangleSelector(selector);
	selector.drop();

	node = smgr.addAnimatedMeshSceneNode(smgr.getMesh(resourcePath + "ninja.b3d"), None, IDFlag_IsPickable + IDFlag_IsHighlightable);
	node.setScale(irr.vector3df(10, 10, 10));
	node.setPosition(irr.vector3df(-70,-66,-60));
	node.setRotation(irr.vector3df(0,90,0));
	node.setAnimationSpeed(10.0);
	node.getMaterial(0).NormalizeNormals = True;

	selector = smgr.createTriangleSelector(node);
	node.setTriangleSelector(selector);
	selector.drop();

	material.setTexture(0, None);
	material.Lighting = False;

	light = smgr.addLightSceneNode(None, irr.vector3df(-60,100,400), irr.SColorf(1.0,1.0,1.0,1.0), 600.0);
	light.setID(ID_IsNotPickable);

	highlightedSceneNode = None;
	collMan = smgr.getSceneCollisionManager();
	lastFPS = -1;

	material.Wireframe=True;

	while device.run() :
		if device.isWindowActive() :
			driver.beginScene(True, True, 0);
			smgr.drawAll();

			if highlightedSceneNode :
				highlightedSceneNode.setMaterialFlag(irr.EMF_LIGHTING, True);
				highlightedSceneNode = None;

			_start = camera.getPosition();
			target = camera.getTarget();
			t = irr.vector3df(target.X - _start.X, target.Y - _start.Y, target.Z - _start.Z);
			t = t.normalize();
			_end = irr.vector3df(_start.X + t.X * 1000.0, _start.Y + t.Y * 1000.0, _start.Z + t.Z * 1000.0);
			ray = irr.line3d_f32(_start.X, _start.Y, _start.Z, _end.X, _end.Y, _end.Z);
			
			s = camera.getPosition();
			#e = s._opAdd(camera.getTarget()._opSub(s).normalize()._opMul(1000.0));
			#We use Python operator overload to calculate it
			e = camera.getTarget() - s;
			e = s + e.normalize() * 1000.0;
			ray = irr.line3d_f32(s.X, s.Y, s.Z, e.X, e.Y, e.Z);

			intersection = irr.vector3df();
			hitTriangle = irr.triangle3df();
			selectedSceneNode = collMan.getSceneNodeAndCollisionPointFromRay(ray,	intersection, hitTriangle, IDFlag_IsPickable, None);

			if selectedSceneNode != None :
				bill.setPosition(intersection);
				
				driver.setTransform(irr.ETS_WORLD, irr.matrix4());
				driver.setMaterial(material);
				driver.draw3DTriangle(hitTriangle, irr.SColor(0,255,0,0));

				if (selectedSceneNode.getID() & IDFlag_IsHighlightable) == IDFlag_IsHighlightable :
					highlightedSceneNode = selectedSceneNode;
					highlightedSceneNode.setMaterialFlag(irr.EMF_LIGHTING, False);

			driver.endScene();

			fps = driver.getFPS();

			if lastFPS != fps :
				tmp = "Collision detection example - cpgf Irrlicht Python Binding [";
				tmp = tmp + driver.getName();
				tmp = tmp + "] FPS:";
				tmp = tmp + str(fps);

				device.setWindowCaption(tmp);
				lastFPS = fps;

	device.drop();
	
	return 0;


start();
