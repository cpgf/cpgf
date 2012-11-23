cpgf.import("cpgf", "builtin.debug");

local device = nil;
local UseHighLevelShaders = false;

function overrideShaderCallBack(callback)
	callback.OnSetConstants = function(me, services, userData)
		local driver = services.getVideoDriver();

		local invWorld = irr.matrix4(driver.getTransform(irr.ETS_WORLD));
		invWorld.makeInverse();

		if UseHighLevelShaders then
			services.setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);
		else
			services.setVertexShaderConstant(invWorld.pointer(), 0, 4);
		end


		local worldViewProj = irr.matrix4(driver.getTransform(irr.ETS_PROJECTION));
		worldViewProj = worldViewProj * driver.getTransform(irr.ETS_VIEW);
		worldViewProj = worldViewProj * driver.getTransform(irr.ETS_WORLD);

		if UseHighLevelShaders then
			services.setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
		else
			services.setVertexShaderConstant(worldViewProj.pointer(), 4, 4);
		end


		local pos = device.getSceneManager().getActiveCamera().getAbsolutePosition();

		if UseHighLevelShaders then
			services.setVertexShaderConstant("mLightPos", pos, 3);
		else
			services.setVertexShaderConstant(pos, 8, 1);
		end


		local col = irr.SColorf(0.0,1.0,1.0,0.0);

		if UseHighLevelShaders then
			services.setVertexShaderConstant("mLightColor",	col, 4);
		else
			services.setVertexShaderConstant(col, 9, 1);
		end


		local world = driver.getTransform(irr.ETS_WORLD);
		world = world.getTransposed();

		if UseHighLevelShaders then
			services.setVertexShaderConstant("mTransWorld", world.pointer(), 16);
		else
			services.setVertexShaderConstant(world.pointer(), 10, 4);
		end
	end
end

function start()
	local driverType= irr.driverChoiceConsole();
	if driverType==irr.EDT_COUNT then
		return 1;
	end

	if driverType == irr.EDT_DIRECT3D9 or driverType == irr.EDT_OPENGL then
		UseHighLevelShaders = true;
	end

	device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480));

	if device == nil then
		return 1;
	end


	local driver = device.getVideoDriver();
	local smgr = device.getSceneManager();
	local gui = device.getGUIEnvironment();

	local vsFileName;
	local psFileName;

	if driverType == irr.EDT_DIRECT3D8 then
		psFileName = "../../media/d3d8.psh";
		vsFileName = "../../media/d3d8.vsh";

	elseif driverType == irr.EDT_DIRECT3D9 then
		if UseHighLevelShaders then
			psFileName = "../../media/d3d9.hlsl";
			vsFileName = psFileName;
		else
			psFileName = "../../media/d3d9.psh";
			vsFileName = "../../media/d3d9.vsh";
		end

	elseif driverType == irr.EDT_OPENGL then
		if UseHighLevelShaders then
			psFileName = "../../media/opengl.frag";
			vsFileName = "../../media/opengl.vert";
		else
			psFileName = "../../media/opengl.psh";
			vsFileName = "../../media/opengl.vsh";
		end
	end

	if not driver.queryFeature(irr.EVDF_PIXEL_SHADER_1_1) and not driver.queryFeature(irr.EVDF_ARB_FRAGMENT_PROGRAM_1) then
		device.getLogger().log("WARNING: Pixel shaders disabled because of missing driver/hardware support.");
		psFileName = "";
	end

	if not driver.queryFeature(irr.EVDF_VERTEX_SHADER_1_1) and not driver.queryFeature(irr.EVDF_ARB_VERTEX_PROGRAM_1) then
		device.getLogger().log("WARNING: Vertex shaders disabled because of missing driver/hardware support.");
		vsFileName = "";
	end


	local gpu = driver.getGPUProgrammingServices();
	local newMaterialType1 = 0;
	local newMaterialType2 = 0;

	local mc = nil;
	if gpu then
		mc = irr.IShaderConstantSetCallBackWrapper();
		overrideShaderCallBack(mc);

		if UseHighLevelShaders then
			newMaterialType1 = gpu.addHighLevelShaderMaterialFromFiles(vsFileName, "vertexMain", irr.EVST_VS_1_1, psFileName, "pixelMain", irr.EPST_PS_1_1, mc, irr.EMT_SOLID);
			newMaterialType2 = gpu.addHighLevelShaderMaterialFromFiles(vsFileName, "vertexMain", irr.EVST_VS_1_1, psFileName, "pixelMain", irr.EPST_PS_1_1, mc, irr.EMT_TRANSPARENT_ADD_COLOR);
		else
			newMaterialType1 = gpu.addShaderMaterialFromFiles(vsFileName, psFileName, mc, irr.EMT_SOLID);
			newMaterialType2 = gpu.addShaderMaterialFromFiles(vsFileName, psFileName, mc, irr.EMT_TRANSPARENT_ADD_COLOR);
		end
		
		-- mc.drop();
	end


	local node = smgr.addCubeSceneNode(50);
	node.setPosition(irr.vector3df(0,0,0));
	node.setMaterialTexture(0, driver.getTexture("../../media/wall.bmp"));
	node.setMaterialFlag(irr.EMF_LIGHTING, false);
	node.setMaterialType(newMaterialType1);

	smgr.addTextSceneNode(gui.getBuiltInFont(), "PS & VS & EMT_SOLID", irr.SColor(255,255,255,255),	node);

	local anim = smgr.createRotationAnimator(irr.vector3df(0,0.3,0));
	node.addAnimator(anim);
	anim.drop();

	node = smgr.addCubeSceneNode(50);
	node.setPosition(irr.vector3df(0,-10,50));
	node.setMaterialTexture(0, driver.getTexture("../../media/wall.bmp"));
	node.setMaterialFlag(irr.EMF_LIGHTING, false);
	node.setMaterialType(newMaterialType2);

	smgr.addTextSceneNode(gui.getBuiltInFont(), "PS & VS & EMT_TRANSPARENT", irr.SColor(255,255,255,255), node);

	anim = smgr.createRotationAnimator(irr.vector3df(0,0.3,0));
	node.addAnimator(anim);
	anim.drop();


	node = smgr.addCubeSceneNode(50);
	node.setPosition(irr.vector3df(0,50,25));
	node.setMaterialTexture(0, driver.getTexture("../../media/wall.bmp"));
	node.setMaterialFlag(irr.EMF_LIGHTING, false);
	smgr.addTextSceneNode(gui.getBuiltInFont(), "NO SHADER", irr.SColor(255,255,255,255), node);


	driver.setTextureCreationFlag(irr.ETCF_CREATE_MIP_MAPS, false);

	smgr.addSkyBoxSceneNode(driver.getTexture("../../media/irrlicht2_up.jpg"), driver.getTexture("../../media/irrlicht2_dn.jpg"), driver.getTexture("../../media/irrlicht2_lf.jpg"), driver.getTexture("../../media/irrlicht2_rt.jpg"), driver.getTexture("../../media/irrlicht2_ft.jpg"), driver.getTexture("../../media/irrlicht2_bk.jpg"));

	driver.setTextureCreationFlag(irr.ETCF_CREATE_MIP_MAPS, true);

	local cam = smgr.addCameraSceneNodeFPS();
	cam.setPosition(irr.vector3df(-100,50,100));
	cam.setTarget(irr.vector3df(0,0,0));
	device.getCursorControl().setVisible(false);

	local lastFPS = -1;

	while device.run() do
		if device.isWindowActive() then
			driver.beginScene(true, true, irr.SColor(255,0,0,0));
			smgr.drawAll();
			driver.endScene();

			local fps = driver.getFPS();

			if lastFPS ~= fps then
				local str = "cpgf Irrlicht Lua Binding - Vertex and pixel shader example [";
				str = str .. driver.getName();
				str = str .. "] FPS:";
				str = str .. fps;

				device.setWindowCaption(str);
				lastFPS = fps;
			end
		end
	end

	device.drop();

	return 0;
end

start();
