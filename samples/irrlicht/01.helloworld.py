cpgf._import(None, "builtin");

def start() :
	device = irr.createDevice(irr.EDT_SOFTWARE, irr.dimension2d_u32(640, 480), 16, False, False, False, None);

	if device == None :
		return 1

	device.setWindowCaption("Hello World! - cpgf Irrlicht Python Binding Demo")

	driver = device.getVideoDriver()
	smgr = device.getSceneManager()
	guienv = device.getGUIEnvironment()

	guienv.addStaticText("Hello World! This is the Irrlicht Software renderer!", irr.rect_s32(10,10,260,22), True)

	mesh = smgr.getMesh("../../media/sydney.md2")
	
	if mesh == None :
		device.drop()
		return 1
	
	node = smgr.addAnimatedMeshSceneNode( mesh )

	if node != None :
		node.setMaterialFlag(irr.EMF_LIGHTING, False)
		node.setMD2Animation(irr.EMAT_STAND)
		node.setMaterialTexture( 0, driver.getTexture("../../media/sydney.bmp") )


	smgr.addCameraSceneNode(None, irr.vector3df(0,30,-40), irr.vector3df(0,5,0))

	col = irr.SColor(255,100,101,140)
	while device.run() :
		driver.beginScene(True, True, col)

		smgr.drawAll()
		guienv.drawAll()

		driver.endScene()

	device.drop()

	return 0


start()
