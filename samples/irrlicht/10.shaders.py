cpgf._import("cpgf", "builtin.debug");

device = None;
UseHighLevelShaders = False;

invWorld = None;
worldViewProj = None;
pos = None;
col = None;
world = None;

def overrideShaderCallBack(callback) :
	def OnSetConstants(me, services, userData) :
		global UseHighLevelShaders;
		global device;
		
		global invWorld;
		global worldViewProj;
		global pos;
		global col;
		global world;
		
		driver = services.getVideoDriver();

		invWorld = irr.matrix4(driver.getTransform(irr.ETS_WORLD));
		invWorld.makeInverse();

		if UseHighLevelShaders :
			services.setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);
		else :
			services.setVertexShaderConstant(invWorld.pointer(), 0, 4);


		worldViewProj = irr.matrix4(driver.getTransform(irr.ETS_PROJECTION));
		worldViewProj = worldViewProj * driver.getTransform(irr.ETS_VIEW);
		worldViewProj = worldViewProj * driver.getTransform(irr.ETS_WORLD);

		if UseHighLevelShaders :
			services.setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
		else :
			services.setVertexShaderConstant(worldViewProj.pointer(), 4, 4);


		pos = device.getSceneManager().getActiveCamera().getAbsolutePosition();

		if UseHighLevelShaders :
			services.setVertexShaderConstant("mLightPos", pos, 3);
		else :
			services.setVertexShaderConstant(pos, 8, 1);


		col = irr.SColorf(0.0,1.0,1.0,0.0);

		if UseHighLevelShaders :
			services.setVertexShaderConstant("mLightColor",	col, 4);
		else :
			services.setVertexShaderConstant(col, 9, 1);


		world = driver.getTransform(irr.ETS_WORLD);
		world = world.getTransposed();

		if UseHighLevelShaders :
			services.setVertexShaderConstant("mTransWorld", world.pointer(), 16);
		else :
			services.setVertexShaderConstant(world.pointer(), 10, 4);

	callback.OnSetConstants = OnSetConstants;

def start() :
	global UseHighLevelShaders;
	global device;
		
	global invWorld;
	global worldViewProj;
	global pos;
	global col;
	global world;
	
	driverType= irr.driverChoiceConsole();
	if driverType==irr.EDT_COUNT :
		return 1;

	if driverType == irr.EDT_DIRECT3D9 or driverType == irr.EDT_OPENGL :
		UseHighLevelShaders = True;

	device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480));

	if device == None :
		return 1;


	driver = device.getVideoDriver();
	smgr = device.getSceneManager();
	gui = device.getGUIEnvironment();

	if driverType == irr.EDT_DIRECT3D8 :
		psFileName = "../../media/d3d8.psh";
		vsFileName = "../../media/d3d8.vsh";

	elif driverType == irr.EDT_DIRECT3D9 :
		if UseHighLevelShaders :
			psFileName = "../../media/d3d9.hlsl";
			vsFileName = psFileName;
		else :
			psFileName = "../../media/d3d9.psh";
			vsFileName = "../../media/d3d9.vsh";

	elif driverType == irr.EDT_OPENGL :
		if UseHighLevelShaders :
			psFileName = "../../media/opengl.frag";
			vsFileName = "../../media/opengl.vert";
		else :
			psFileName = "../../media/opengl.psh";
			vsFileName = "../../media/opengl.vsh";

	if not driver.queryFeature(irr.EVDF_PIXEL_SHADER_1_1) and not driver.queryFeature(irr.EVDF_ARB_FRAGMENT_PROGRAM_1) :
		device.getLogger().log("WARNING: Pixel shaders disabled because of missing driver/hardware support.");
		psFileName = "";

	if not driver.queryFeature(irr.EVDF_VERTEX_SHADER_1_1) and not driver.queryFeature(irr.EVDF_ARB_VERTEX_PROGRAM_1) :
		device.getLogger().log("WARNING: Vertex shaders disabled because of missing driver/hardware support.");
		vsFileName = "";


	gpu = driver.getGPUProgrammingServices();
	newMaterialType1 = None;
	newMaterialType2 = None;

	mc = None;
	if gpu != None :
		mc = irr.IShaderConstantSetCallBackWrapper();
		overrideShaderCallBack(mc);

		if UseHighLevelShaders :
			newMaterialType1 = gpu.addHighLevelShaderMaterialFromFiles(vsFileName, "vertexMain", irr.EVST_VS_1_1, psFileName, "pixelMain", irr.EPST_PS_1_1, mc, irr.EMT_SOLID);
			newMaterialType2 = gpu.addHighLevelShaderMaterialFromFiles(vsFileName, "vertexMain", irr.EVST_VS_1_1, psFileName, "pixelMain", irr.EPST_PS_1_1, mc, irr.EMT_TRANSPARENT_ADD_COLOR);
		else :
			newMaterialType1 = gpu.addShaderMaterialFromFiles(vsFileName, psFileName, mc, irr.EMT_SOLID);
			newMaterialType2 = gpu.addShaderMaterialFromFiles(vsFileName, psFileName, mc, irr.EMT_TRANSPARENT_ADD_COLOR);
		
		#mc.drop();


	node = smgr.addCubeSceneNode(50);
	node.setPosition(irr.vector3df(0,0,0));
	node.setMaterialTexture(0, driver.getTexture("../../media/wall.bmp"));
	node.setMaterialFlag(irr.EMF_LIGHTING, False);
	node.setMaterialType(newMaterialType1);

	smgr.addTextSceneNode(gui.getBuiltInFont(), "PS & VS & EMT_SOLID", irr.SColor(255,255,255,255),	node);

	anim = smgr.createRotationAnimator(irr.vector3df(0,0.3,0));
	node.addAnimator(anim);
	anim.drop();

	node = smgr.addCubeSceneNode(50);
	node.setPosition(irr.vector3df(0,-10,50));
	node.setMaterialTexture(0, driver.getTexture("../../media/wall.bmp"));
	node.setMaterialFlag(irr.EMF_LIGHTING, False);
	node.setMaterialType(newMaterialType2);

	smgr.addTextSceneNode(gui.getBuiltInFont(), "PS & VS & EMT_TRANSPARENT", irr.SColor(255,255,255,255), node);

	anim = smgr.createRotationAnimator(irr.vector3df(0,0.3,0));
	node.addAnimator(anim);
	anim.drop();


	node = smgr.addCubeSceneNode(50);
	node.setPosition(irr.vector3df(0,50,25));
	node.setMaterialTexture(0, driver.getTexture("../../media/wall.bmp"));
	node.setMaterialFlag(irr.EMF_LIGHTING, False);
	smgr.addTextSceneNode(gui.getBuiltInFont(), "NO SHADER", irr.SColor(255,255,255,255), node);


	driver.setTextureCreationFlag(irr.ETCF_CREATE_MIP_MAPS, False);

	smgr.addSkyBoxSceneNode(driver.getTexture("../../media/irrlicht2_up.jpg"), driver.getTexture("../../media/irrlicht2_dn.jpg"), driver.getTexture("../../media/irrlicht2_lf.jpg"), driver.getTexture("../../media/irrlicht2_rt.jpg"), driver.getTexture("../../media/irrlicht2_ft.jpg"), driver.getTexture("../../media/irrlicht2_bk.jpg"));

	driver.setTextureCreationFlag(irr.ETCF_CREATE_MIP_MAPS, True);

	cam = smgr.addCameraSceneNodeFPS();
	cam.setPosition(irr.vector3df(-100,50,100));
	cam.setTarget(irr.vector3df(0,0,0));
	device.getCursorControl().setVisible(False);

	lastFPS = -1;

	while device.run() :
		if device.isWindowActive() :
			driver.beginScene(True, True, irr.SColor(255,0,0,0));
			smgr.drawAll();
			driver.endScene();

			fps = driver.getFPS();

			if lastFPS != fps :
				tmp = "cpgf Irrlicht Python Binding - Vertex and pixel shader example [";
				tmp = tmp + driver.getName();
				tmp = tmp + "] FPS:";
				tmp = tmp + str(fps);

				device.setWindowCaption(tmp);
				lastFPS = fps;

	device.drop();

	return 0;

start();
