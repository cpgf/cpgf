cpgf._import("cpgf", "builtin.collections");
cpgf._import("cpgf", "builtin.debug");

def overrideISceneNode(node) :
	global box;
	global Vertices;
	global Material;
	
	Box = irr.aabbox3d_f32();
	Vertices = cpgf.createObjectArray(irr.S3DVertex);
	Material = irr.SMaterial();

	Material.Wireframe = False;
	Material.Lighting = False;

	Vertices.setItem(0, irr.S3DVertex(0,0,10, 1,1,0, irr.SColor(255,0,255,255), 0, 1));
	Vertices.setItem(1, irr.S3DVertex(10,0,-10, 1,0,0, irr.SColor(255,255,0,255), 1, 1));
	Vertices.setItem(2, irr.S3DVertex(0,20,0, 0,1,1, irr.SColor(255,255,255,0), 1, 0));
	Vertices.setItem(3, irr.S3DVertex(-10,0,-10, 0,0,1, irr.SColor(255,0,255,0), 0, 0));

	Box.reset(Vertices.getItem(0).Pos);
	for i in range(3) :
		Box.addInternalPoint(Vertices.getItem(i).Pos);


	def OnRegisterSceneNode(me) :
		if me.isVisible() :
			me.getSceneManager().registerNodeForRendering(me);
		me.super_OnRegisterSceneNode();
		
	node.OnRegisterSceneNode = OnRegisterSceneNode;

	def render(me) :
		indices = [	0, 2, 3, 2, 1, 3, 1, 0, 3, 2, 0, 1 ];
		driver = me.getSceneManager().getVideoDriver();
		indicesByteArray = cpgf.createByteArray();
		for i in range(len(indices)) :
			indicesByteArray.writeInt16(indices[i]);
		driver.setMaterial(Material);
		driver.setTransform(irr.ETS_WORLD, me.getAbsoluteTransformation());
		driver.drawVertexPrimitiveList(Vertices.getItem(0), 4, indicesByteArray.getPointer(), 4, irr.EVT_STANDARD, irr.EPT_TRIANGLES, irr.EIT_16BIT);
	
	node.render = render;


	def getBoundingBox(me) :
		return Box;
		
	node.getBoundingBox = getBoundingBox;

	def getMaterialCount(me) :
		return 1;

	node.getMaterialCount = getMaterialCount;

	def getMaterial(me, i) :
		return Material;

	node.getMaterial = getMaterial;


def start() :


	driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT :
		return 1;

	device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480), 16, False);

	if device == None :
		return 1;

	device.setWindowCaption("Custom Scene Node - cpgf Irrlicht Python binding Demo");

	driver = device.getVideoDriver();
	smgr = device.getSceneManager();

	smgr.addCameraSceneNode(None, irr.vector3df(0, -40, 0), irr.vector3df(0, 0, 0));

	myNode = irr.ISceneNodeWrapper(smgr.getRootSceneNode(), smgr, 666);
	overrideISceneNode(myNode);
	
	anim = smgr.createRotationAnimator(irr.vector3df(0.8, 0, 0.8));

	if anim :
		myNode.addAnimator(anim);
		anim.drop();
		anim = None;
		
	frames = 0;
	while device.run() :

		driver.beginScene(True, True, irr.SColor(0, 100, 100, 100));

		smgr.drawAll();

		driver.endScene();
		frames = frames + 1;
		if frames == 100 :

			tmp = "cpgf Irrlicht Python Binding Demo [";
			tmp = tmp + driver.getName();
			tmp = tmp + "] FPS:";
			tmp = tmp + str(driver.getFPS());

			device.setWindowCaption(tmp);
			frames = 0;

	device.drop();

	return 0;


start();
