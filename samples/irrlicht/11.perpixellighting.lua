cpgf.import("cpgf", "builtin.debug");


function overrideEventReceiver(receiver, room, env, driver)
	local Room = room;
	local Driver = driver;

	local skin = env.getSkin();
	local font = env.getFont("../../media/fonthaettenschweiler.bmp");
	if font then
		skin.setFont(font);
	end

	local window = env.addWindow(irr.rect_s32(460,375,630,470), false, "Use 'E' + 'R' to change");

	local ListBox = env.addListBox(irr.rect_s32(2,22,165,88), window);

	ListBox.addItem("Diffuse");
	ListBox.addItem("Bump mapping");
	ListBox.addItem("Parallax mapping");
	ListBox.setSelected(1);

	ProblemText = env.addStaticText("Your hardware or this renderer is not able to use the needed shaders for this material. Using fall back materials.", irr.rect_s32(150,20,470,80));

	ProblemText.setOverrideColor(irr.SColor(100,255,255,255));

	local renderer = Driver.getMaterialRenderer(irr.EMT_PARALLAX_MAP_SOLID);
	if renderer and renderer.getRenderCapability() == 0 then
		ListBox.setSelected(2);
	end

	local setMaterial = function()
		local type = irr.EMT_SOLID;

		local index = ListBox.getSelected();
		if index == 0 then
			type = irr.EMT_SOLID;

		elseif index == 1 then
			type = irr.EMT_NORMAL_MAP_SOLID;

		elseif index == 2 then
			type = irr.EMT_PARALLAX_MAP_SOLID;
		end

		Room.setMaterialType(type);

		local renderer = Driver.getMaterialRenderer(type);

		if not renderer or renderer.getRenderCapability() ~= 0 then
			ProblemText.setVisible(true);
		else
			ProblemText.setVisible(false);
		end
	end

	setMaterial();
	
	receiver.OnEvent = function(me, event)
		if event.EventType == irr.EET_KEY_INPUT_EVENT and not event.KeyInput.PressedDown and Room and ListBox then

			local sel = ListBox.getSelected();
			if event.KeyInput.Key == irr.KEY_KEY_R then
				sel = sel + 1;
			elseif event.KeyInput.Key == irr.KEY_KEY_E then
				sel = sel - 1;
			else
				return false;
			end

			if sel > 2 then
				sel = 0;
			end
			if sel < 0 then
				sel = 2;
			end
			ListBox.setSelected(sel);

			setMaterial();
		end

		return false;
	end

end


function start()
	local driverType=irr.driverChoiceConsole();
	if driverType==irr.EDT_COUNT then
		return 1;
	end

	local device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480));

	if device == nil then
		return 1;
	end

	local driver = device.getVideoDriver();
	local smgr = device.getSceneManager();
	local env = device.getGUIEnvironment();

	driver.setTextureCreationFlag(irr.ETCF_ALWAYS_32_BIT, true);

	env.addImage(driver.getTexture("../../media/irrlichtlogo2.png"), irr.position2d_s32(10,10));

	local camera = smgr.addCameraSceneNodeFPS();
	camera.setPosition(irr.vector3df(-200,200,-200));

	device.getCursorControl().setVisible(false);

	driver.setFog(irr.SColor(0,138,125,81), irr.EFT_FOG_LINEAR, 250, 1000, 0.003, true, false);

	local roomMesh = smgr.getMesh("../../media/room.3ds");
	local room = nil;

	if roomMesh then
		smgr.getMeshManipulator().makePlanarTextureMapping(roomMesh.getMesh(0), 0.003);

		local normalMap = driver.getTexture("../../media/rockwall_height.bmp");

		if normalMap then
			driver.makeNormalMapTexture(normalMap, 9.0);
		end

		local tangentMesh = smgr.getMeshManipulator().createMeshWithTangents(roomMesh.getMesh(0));

		room = smgr.addMeshSceneNode(tangentMesh);
		room.setMaterialTexture(0, driver.getTexture("../../media/rockwall.jpg"));
		room.setMaterialTexture(1, normalMap);

		room.getMaterial(0).SpecularColor.set(0,0,0,0);

		room.setMaterialFlag(irr.EMF_FOG_ENABLE, true);
		room.setMaterialType(irr.EMT_PARALLAX_MAP_SOLID);
		room.getMaterial(0).MaterialTypeParam = 0.035;

		tangentMesh.drop();
	end

	local earthMesh = smgr.getMesh("../../media/earth.x");

	if earthMesh then
		local manipulator = smgr.getMeshManipulator();

		local tangentSphereMesh =	manipulator.createMeshWithTangents(earthMesh.getMesh(0));

		manipulator.setVertexColorAlpha(tangentSphereMesh, 200);

		local m = irr.matrix4();
		m.setScale (irr.vector3df(50,50,50) );
		manipulator.transformMesh( tangentSphereMesh, m );

		local sphere = smgr.addMeshSceneNode(tangentSphereMesh);

		sphere.setPosition(irr.vector3df(-70,130,45));

		local earthNormalMap = driver.getTexture("../../media/earthbump.jpg");
		if earthNormalMap then
			driver.makeNormalMapTexture(earthNormalMap, 20.0);
			sphere.setMaterialTexture(1, earthNormalMap);
			sphere.setMaterialType(irr.EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA);
		end

		sphere.setMaterialFlag(irr.EMF_FOG_ENABLE, true);

		local anim = smgr.createRotationAnimator(irr.vector3df(0,0.1,0));
		sphere.addAnimator(anim);
		anim.drop();

		tangentSphereMesh.drop();
	end

	local light1 = smgr.addLightSceneNode(nil, irr.vector3df(0,0,0), irr.SColorf(0.5, 1.0, 0.5, 0.0), 800.0);

	light1.setDebugDataVisible ( irr.EDS_BBOX );


	local anim = smgr.createFlyCircleAnimator (irr.vector3df(50,300,0),190.0, -0.003);
	light1.addAnimator(anim);
	anim.drop();

	local bill = smgr.addBillboardSceneNode(light1, irr.dimension2d_f32(60, 60));

	bill.setMaterialFlag(irr.EMF_LIGHTING, false);
	bill.setMaterialFlag(irr.EMF_ZWRITE_ENABLE, false);
	bill.setMaterialType(irr.EMT_TRANSPARENT_ADD_COLOR);
	bill.setMaterialTexture(0, driver.getTexture("../../media/particlered.bmp"));

	local light2 = smgr.addLightSceneNode(nil, irr.vector3df(0,0,0), irr.SColorf(1.0, 0.2, 0.2, 0.0), 800.0);

	anim = smgr.createFlyCircleAnimator(irr.vector3df(0,150,0), 200.0, 0.001, irr.vector3df(0.2, 0.9, 0.0));
	light2.addAnimator(anim);
	anim.drop();

	bill = smgr.addBillboardSceneNode(light2, irr.dimension2d_f32(120, 120));
	bill.setMaterialFlag(irr.EMF_LIGHTING, false);
	bill.setMaterialFlag(irr.EMF_ZWRITE_ENABLE, false);
	bill.setMaterialType(irr.EMT_TRANSPARENT_ADD_COLOR);
	bill.setMaterialTexture(0, driver.getTexture("../../media/particlewhite.bmp"));

	local ps = smgr.addParticleSystemSceneNode(false, light2);

	local em = ps.createBoxEmitter(irr.aabbox3d_f32(-3,0,-3,3,1,3), irr.vector3df(0.0,0.03,0.0), 80,100, irr.SColor(0,255,255,255), irr.SColor(0,255,255,255), 400,1100);
	em.setMinStartSize(irr.dimension2d_f32(30.0, 40.0));
	em.setMaxStartSize(irr.dimension2d_f32(30.0, 40.0));

	ps.setEmitter(em);
	em.drop();

	local paf = ps.createFadeOutParticleAffector();
	ps.addAffector(paf);
	paf.drop();

	ps.setMaterialFlag(irr.EMF_LIGHTING, false);
	ps.setMaterialFlag(irr.EMF_ZWRITE_ENABLE, false);
	ps.setMaterialTexture(0, driver.getTexture("../../media/fireball.bmp"));
	ps.setMaterialType(irr.EMT_TRANSPARENT_VERTEX_ALPHA);


	local receiver = irr.IEventReceiverWrapper();
	overrideEventReceiver(receiver, room, env, driver);
	device.setEventReceiver(receiver);

	local lastFPS = -1;

	while device.run() do
		if device.isWindowActive() then
			driver.beginScene(true, true, 0);

			smgr.drawAll();
			env.drawAll();

			driver.endScene();

			local fps = driver.getFPS();

			if lastFPS ~= fps then
				local tmp = "Per pixel lighting example - cpgf Irrlicht Lua Binding [";
				tmp = tmp .. driver.getName();
				tmp = tmp .. "] FPS:";
				tmp = tmp .. fps;

				device.setWindowCaption(tmp);
				lastFPS = fps;
			end
		end
	end

	device.drop();

	return 0;
end

start();
