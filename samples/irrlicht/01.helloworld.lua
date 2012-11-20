function start()
	local device = irr.createDevice(irr.EDT_SOFTWARE, irr.dimension2d_u32(640, 480), 16, false, false, false, nil);

	if device == nil then
		return 1
	end

	device.setWindowCaption("Hello World! - cpgf Irrlicht Lua Binding Demo")

	local driver = device.getVideoDriver()
	local smgr = device.getSceneManager()
	local guienv = device.getGUIEnvironment()

	guienv.addStaticText("Hello World! This is the Irrlicht Software renderer!", irr.rect_s32(10,10,260,22), true)

	local mesh = smgr.getMesh("../../media/sydney.md2")
	
	if mesh == nil then
		device.drop()
		return 1
	end
	
	local node = smgr.addAnimatedMeshSceneNode( mesh )

	if node ~= nil then
		node.setMaterialFlag(irr.EMF_LIGHTING, false)
		node.setMD2Animation(irr.EMAT_STAND)
		node.setMaterialTexture( 0, driver.getTexture("../../media/sydney.bmp") )
	end


	smgr.addCameraSceneNode(nil, irr.vector3df(0,30,-40), irr.vector3df(0,5,0))

	while device.run() do
		driver.beginScene(true, true, irr.SColor(255,100,101,140))

		smgr.drawAll()
		guienv.drawAll()

		driver.endScene()
	end

	device.drop()

	return 0
end

start()
