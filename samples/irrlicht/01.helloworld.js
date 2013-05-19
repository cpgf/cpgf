function start()
{
	var device = irr.createDevice(irr.EDT_SOFTWARE, new irr.dimension2d_u32(640, 480), 16, false, false, false, null);

	if (!device)
		return 1;

	device.setWindowCaption("Hello World! - cpgf Irrlicht JavaScript Binding Demo");

	var driver = device.getVideoDriver();
	var smgr = device.getSceneManager();
	var guienv = device.getGUIEnvironment();

	guienv.addStaticText("Hello World! This is the Irrlicht Software renderer!", new irr.rect_s32(10,10,260,22), true);

	var mesh = smgr.getMesh("../../media/sydney.md2");
	
	if(!mesh)
	{
		device.drop();
		return 1;
	}
	
	var node = smgr.addAnimatedMeshSceneNode( mesh );

	if (node)
	{
		node.setMaterialFlag(irr.EMF_LIGHTING, false);
		node.setMD2Animation(irr.EMAT_STAND);
		node.setMaterialTexture( 0, driver.getTexture("../../media/sydney.bmp") );
	}


	smgr.addCameraSceneNode(null, new irr.vector3df(0,30,-40), new irr.vector3df(0,5,0));

	while(device.run())
	{
		driver.beginScene(true, true, new irr.SColor(255,100,101,140));

		smgr.drawAll();
		guienv.drawAll();

		driver.endScene();
	}

	device.drop();


	return 0;
}

start();
