// Know issues in cpgf script binding
// The objects can't be shown if UseHighLevelShaders is true.

cpgf.import("cpgf", "builtin.debug");

var device = null;
var UseHighLevelShaders = false;

function overrideShaderCallBack(callback)
{
	callback.OnSetConstants = function(me, services, userData) {
		var driver = services.getVideoDriver();

		// set inverted world matrix
		// if we are using highlevel shaders (the user can select this when
		// starting the program), we must set the constants by name.

		var invWorld = new irr.matrix4(driver.getTransform(irr.ETS_WORLD));
		invWorld.makeInverse();

		if (UseHighLevelShaders)
			services.setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);
		else
			services.setVertexShaderConstant(invWorld.pointer(), 0, 4);

		// set clip matrix

		var worldViewProj = new irr.matrix4(driver.getTransform(irr.ETS_PROJECTION));
		worldViewProj._opMulAssign(driver.getTransform(irr.ETS_VIEW));
		worldViewProj._opMulAssign(driver.getTransform(irr.ETS_WORLD));

		if (UseHighLevelShaders)
			services.setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
		else
			services.setVertexShaderConstant(worldViewProj.pointer(), 4, 4);

		// set camera position

		var pos = device.getSceneManager().getActiveCamera().getAbsolutePosition();

		if (UseHighLevelShaders)
			services.setVertexShaderConstant("mLightPos", pos, 3);
		else
			services.setVertexShaderConstant(pos, 8, 1);

		// set light color

		var col = new irr.SColorf(0.0,1.0,1.0,0.0);

		if (UseHighLevelShaders)
			services.setVertexShaderConstant("mLightColor",	col, 4);
		else
			services.setVertexShaderConstant(col, 9, 1);

		// set transposed world matrix

		var world = driver.getTransform(irr.ETS_WORLD);
		world = world.getTransposed();

		if (UseHighLevelShaders)
			services.setVertexShaderConstant("mTransWorld", world.pointer(), 16);
		else
			services.setVertexShaderConstant(world.pointer(), 10, 4);
	}
}

/*
The next few lines start up the engine just like in most other tutorials
before. But in addition, we ask the user if he wants to use high level shaders
in this example, if he selected a driver which is capable of doing so.
*/
function start()
{
	// ask user for driver
	var driverType= irr.driverChoiceConsole();
	if (driverType==irr.EDT_COUNT)
		return 1;

	// ask the user if we should use high level shaders for this example
	if (driverType == irr.EDT_DIRECT3D9 || driverType == irr.EDT_OPENGL)
	{
		UseHighLevelShaders = true;
	}

	// create device

	device = irr.createDevice(driverType, new irr.dimension2d_u32(640, 480));

	if (device == null)
		return 1; // could not create selected driver.


	var driver = device.getVideoDriver();
	var smgr = device.getSceneManager();
	var gui = device.getGUIEnvironment();

	/*
	Now for the more interesting parts. If we are using Direct3D, we want
	to load vertex and pixel shader programs, if we have OpenGL, we want to
	use ARB fragment and vertex programs. I wrote the corresponding
	programs down into the files d3d8.ps, d3d8.vs, d3d9.ps, d3d9.vs,
	opengl.ps and opengl.vs. We only need the right filenames now. This is
	done in the following switch. Note, that it is not necessary to write
	the shaders into text files, like in this example. You can even write
	the shaders directly as strings into the cpp source file, and use later
	addShaderMaterial() instead of addShaderMaterialFromFiles().
	*/

	var vsFileName; // filename for the vertex shader
	var psFileName; // filename for the pixel shader

	switch(driverType)
	{
	case irr.EDT_DIRECT3D8:
		psFileName = "../../media/d3d8.psh";
		vsFileName = "../../media/d3d8.vsh";
		break;
	case irr.EDT_DIRECT3D9:
		if (UseHighLevelShaders)
		{
			psFileName = "../../media/d3d9.hlsl";
			vsFileName = psFileName; // both shaders are in the same file
		}
		else
		{
			psFileName = "../../media/d3d9.psh";
			vsFileName = "../../media/d3d9.vsh";
		}
		break;

	case irr.EDT_OPENGL:
		if (UseHighLevelShaders)
		{
			psFileName = "../../media/opengl.frag";
			vsFileName = "../../media/opengl.vert";
		}
		else
		{
			psFileName = "../../media/opengl.psh";
			vsFileName = "../../media/opengl.vsh";
		}
		break;
	}

	/*
	In addition, we check if the hardware and the selected renderer is
	capable of executing the shaders we want. If not, we simply set the
	filename string to 0. This is not necessary, but useful in this
	example: For example, if the hardware is able to execute vertex shaders
	but not pixel shaders, we create a new material which only uses the
	vertex shader, and no pixel shader. Otherwise, if we would tell the
	engine to create this material and the engine sees that the hardware
	wouldn't be able to fullfill the request completely, it would not
	create any new material at all. So in this example you would see at
	least the vertex shader in action, without the pixel shader.
	*/

	if (!driver.queryFeature(irr.EVDF_PIXEL_SHADER_1_1) && !driver.queryFeature(irr.EVDF_ARB_FRAGMENT_PROGRAM_1))
	{
		device.getLogger().log("WARNING: Pixel shaders disabled because of missing driver/hardware support.");
		psFileName = "";
	}

	if (!driver.queryFeature(irr.EVDF_VERTEX_SHADER_1_1) &&	!driver.queryFeature(irr.EVDF_ARB_VERTEX_PROGRAM_1))
	{
		device.getLogger().log("WARNING: Vertex shaders disabled because of missing driver/hardware support.");
		vsFileName = "";
	}

	/*
	Now lets create the new materials. As you maybe know from previous
	examples, a material type in the Irrlicht engine is set by simply
	changing the MaterialType value in the SMaterial struct. And this value
	is just a simple 32 bit value, like irr.EMT_SOLID. So we only need
	the engine to create a new value for us which we can set there. To do
	this, we get a pointer to the IGPUProgrammingServices and call
	addShaderMaterialFromFiles(), which returns such a new 32 bit value.
	That's all.

	The parameters to this method are the following: First, the names of
	the files containing the code of the vertex and the pixel shader. If
	you would use addShaderMaterial() instead, you would not need file
	names, then you could write the code of the shader directly as string.
	The following parameter is a pointer to the IShaderConstantSetCallBack
	class we wrote at the beginning of this tutorial. If you don't want to
	set constants, set this to 0. The last paramter tells the engine which
	material it should use as base material.

	To demonstrate this, we create two materials with a different base
	material, one with EMT_SOLID and one with EMT_TRANSPARENT_ADD_COLOR.
	*/

	// create materials

	var gpu = driver.getGPUProgrammingServices();
	var newMaterialType1 = 0;
	var newMaterialType2 = 0;

	// Must put mc outside of if(gpu) block, otherwise, mc maybe GCed.
	var mc = null;
	if (gpu)
	{
		mc = new irr.IShaderConstantSetCallBackWrapper();
		overrideShaderCallBack(mc);

		// create the shaders depending on if the user wanted high level
		// or low level shaders:

		if (UseHighLevelShaders)
		{
			// create material from high level shaders (hlsl or glsl)

			newMaterialType1 = gpu.addHighLevelShaderMaterialFromFiles(vsFileName, "vertexMain", irr.EVST_VS_1_1, psFileName, "pixelMain", irr.EPST_PS_1_1, mc, irr.EMT_SOLID);

			newMaterialType2 = gpu.addHighLevelShaderMaterialFromFiles(vsFileName, "vertexMain", irr.EVST_VS_1_1, psFileName, "pixelMain", irr.EPST_PS_1_1, mc, irr.EMT_TRANSPARENT_ADD_COLOR);
		}
		else
		{
			// create material from low level shaders (asm or arb_asm)

			newMaterialType1 = gpu.addShaderMaterialFromFiles(vsFileName, psFileName, mc, irr.EMT_SOLID);

			newMaterialType2 = gpu.addShaderMaterialFromFiles(vsFileName, psFileName, mc, irr.EMT_TRANSPARENT_ADD_COLOR);
		}

		// Don't drop mc because it's created by "new".
//		mc.drop();
	}

	/*
	Now it's time for testing the materials. We create a test cube and set
	the material we created. In addition, we add a text scene node to the
	cube and a rotation animator to make it look more interesting and
	important.
	*/

	// create test scene node 1, with the new created material type 1

	var node = smgr.addCubeSceneNode(50);
	node.setPosition(new irr.vector3df(0,0,0));
	node.setMaterialTexture(0, driver.getTexture("../../media/wall.bmp"));
	node.setMaterialFlag(irr.EMF_LIGHTING, false);
	node.setMaterialType(newMaterialType1);

	smgr.addTextSceneNode(gui.getBuiltInFont(), "PS & VS & EMT_SOLID", new irr.SColor(255,255,255,255),	node);

	var anim = smgr.createRotationAnimator(new irr.vector3df(0,0.3,0));
	node.addAnimator(anim);
	anim.drop();

	/*
	Same for the second cube, but with the second material we created.
	*/

	// create test scene node 2, with the new created material type 2

	node = smgr.addCubeSceneNode(50);
	node.setPosition(new irr.vector3df(0,-10,50));
	node.setMaterialTexture(0, driver.getTexture("../../media/wall.bmp"));
	node.setMaterialFlag(irr.EMF_LIGHTING, false);
	node.setMaterialType(newMaterialType2);

	smgr.addTextSceneNode(gui.getBuiltInFont(), "PS & VS & EMT_TRANSPARENT", new irr.SColor(255,255,255,255), node);

	anim = smgr.createRotationAnimator(new irr.vector3df(0,0.3,0));
	node.addAnimator(anim);
	anim.drop();

	/*
	Then we add a third cube without a shader on it, to be able to compare
	the cubes.
	*/

	// add a scene node with no shader

	node = smgr.addCubeSceneNode(50);
	node.setPosition(new irr.vector3df(0,50,25));
	node.setMaterialTexture(0, driver.getTexture("../../media/wall.bmp"));
	node.setMaterialFlag(irr.EMF_LIGHTING, false);
	smgr.addTextSceneNode(gui.getBuiltInFont(), "NO SHADER", new irr.SColor(255,255,255,255), node);

	/*
	And last, we add a skybox and a user controlled camera to the scene.
	For the skybox textures, we disable mipmap generation, because we don't
	need mipmaps on it.
	*/

	// add a nice skybox

	driver.setTextureCreationFlag(irr.ETCF_CREATE_MIP_MAPS, false);

	smgr.addSkyBoxSceneNode(driver.getTexture("../../media/irrlicht2_up.jpg"), driver.getTexture("../../media/irrlicht2_dn.jpg"), driver.getTexture("../../media/irrlicht2_lf.jpg"), driver.getTexture("../../media/irrlicht2_rt.jpg"), driver.getTexture("../../media/irrlicht2_ft.jpg"), driver.getTexture("../../media/irrlicht2_bk.jpg"));

	driver.setTextureCreationFlag(irr.ETCF_CREATE_MIP_MAPS, true);

	// add a camera and disable the mouse cursor

	var cam = smgr.addCameraSceneNodeFPS();
	cam.setPosition(new irr.vector3df(-100,50,100));
	cam.setTarget(new irr.vector3df(0,0,0));
	device.getCursorControl().setVisible(false);

	/*
	Now draw everything. That's all.
	*/

	var lastFPS = -1;

	while(device.run())
		if (device.isWindowActive())
	{
		driver.beginScene(true, true, new irr.SColor(255,0,0,0));
		smgr.drawAll();
		driver.endScene();

		var fps = driver.getFPS();

		if (lastFPS != fps)
		{
			var str = "cpgf Irrlicht JavaScript Binding - Vertex and pixel shader example [";
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
