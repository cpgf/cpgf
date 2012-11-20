function overrideEventReceiver(receiver, terrain, skybox, skydome)
	local Terrain = terrain;
	local Skybox = skybox;
	local Skydome = skydome;
	local showBox = true;
	
	Skybox.setVisible(true);
	Skydome.setVisible(false);

	receiver.OnEvent = function(me, event)
		if event.EventType == irr.EET_KEY_INPUT_EVENT and not event.KeyInput.PressedDown then
			if event.KeyInput.Key == irr.KEY_KEY_W then
				Terrain.setMaterialFlag(irr.EMF_WIREFRAME, not Terrain.getMaterial(0).Wireframe);
				Terrain.setMaterialFlag(irr.EMF_POINTCLOUD, false);
				return true;
				
			elseif event.KeyInput.Key == irr.KEY_KEY_P then
				Terrain.setMaterialFlag(irr.EMF_POINTCLOUD, not Terrain.getMaterial(0).PointCloud);
				Terrain.setMaterialFlag(irr.EMF_WIREFRAME, false);
				return true;
				
			elseif event.KeyInput.Key == irr.KEY_KEY_D then
				if Terrain.getMaterial(0).MaterialType == irr.EMT_SOLID then
					Terrain.setMaterialType(irr.EMT_DETAIL_MAP);
				else
					Terrain.setMaterialType(irr.EMT_SOLID);
				end
				return true;
				
			elseif event.KeyInput.Key == irr.KEY_KEY_S then
				showBox= not showBox;
				Skybox.setVisible(showBox);
				Skydome.setVisible(not showBox);
				return true;
				
			end
		end

		return false;
	end
end


function start()
	local driverType=irr.driverChoiceConsole();
	if driverType==irr.EDT_COUNT then
		return 1;
	end

	local params = irr.SIrrlichtCreationParameters();
	params.DriverType=driverType;
	params.WindowSize=irr.dimension2d_u32(640, 480);
	local device = irr.createDeviceEx(params);

	if device == 0 then
		return 1;
	end

	local driver = device.getVideoDriver();
	local smgr = device.getSceneManager();
	local env = device.getGUIEnvironment();

	driver.setTextureCreationFlag(irr.ETCF_ALWAYS_32_BIT, true);

	env.addImage(driver.getTexture("../../media/irrlichtlogo2.png"), irr.position2d_s32(10,10));

	env.getSkin().setFont(env.getFont("../../media/fontlucida.png"));

	env.addStaticText("Press 'W' to change wireframe mode\nPress 'D' to toggle detail map\nPress 'S' to toggle skybox/skydome",	irr.rect_s32(10,421,250,475), true, true, nil, -1, true);

	local camera = smgr.addCameraSceneNodeFPS(nil, 100.0,1.2);

	camera.setPosition(irr.vector3df(2700*2,255*2,2600*2));
	camera.setTarget(irr.vector3df(2397*2,343*2,2700*2));
	camera.setFarValue(42000.0);

	device.getCursorControl().setVisible(false);

	local terrain = smgr.addTerrainSceneNode("../../media/terrain-heightmap.bmp", nil, -1, irr.vector3df(0.0, 0.0, 0.0), irr.vector3df(0.0, 0.0, 0.0), irr.vector3df(40.0, 4.40, 40.0), irr.SColor ( 255, 255, 255, 255 ), 5, irr.ETPS_17,4);

	terrain.setMaterialFlag(irr.EMF_LIGHTING, false);

	terrain.setMaterialTexture(0, driver.getTexture("../../media/terrain-texture.jpg"));
	terrain.setMaterialTexture(1, driver.getTexture("../../media/detailmap3.jpg"));
	
	terrain.setMaterialType(irr.EMT_DETAIL_MAP);

	terrain.scaleTexture(1.0, 20.0);

	local selector = smgr.createTerrainTriangleSelector(terrain, 0);
	terrain.setTriangleSelector(selector);

	local anim = smgr.createCollisionResponseAnimator(selector, camera, irr.vector3df(60,100,60),	irr.vector3df(0,0,0), irr.vector3df(0,50,0));
	selector.drop();
	camera.addAnimator(anim);
	anim.drop();

	local buffer = irr.CDynamicMeshBuffer(irr.EVT_2TCOORDS, irr.EIT_16BIT);
	terrain.getMeshBufferForLOD(buffer, 0);
	local data = buffer.getVertexBuffer().getData();
	-- buffer.drop();

	driver.setTextureCreationFlag(irr.ETCF_CREATE_MIP_MAPS, false);

	local skybox=smgr.addSkyBoxSceneNode(driver.getTexture("../../media/irrlicht2_up.jpg"), driver.getTexture("../../media/irrlicht2_dn.jpg"), driver.getTexture("../../media/irrlicht2_lf.jpg"), driver.getTexture("../../media/irrlicht2_rt.jpg"), driver.getTexture("../../media/irrlicht2_ft.jpg"), driver.getTexture("../../media/irrlicht2_bk.jpg"));
	local skydome=smgr.addSkyDomeSceneNode(driver.getTexture("../../media/skydome.jpg"),16,8,0.95,2.0);

	driver.setTextureCreationFlag(irr.ETCF_CREATE_MIP_MAPS, true);

	local receiver = irr.IEventReceiverWrapper();
	overrideEventReceiver(receiver, terrain, skybox, skydome);
	device.setEventReceiver(receiver);

	local lastFPS = -1;

	while device.run() do
		if device.isWindowActive() then
			driver.beginScene(true, true, 0 );

			smgr.drawAll();
			env.drawAll();

			driver.endScene();

			local fps = driver.getFPS();
			if lastFPS ~= fps then
				local tmp = "Terrain Renderer - cpgf Irrlicht Lua Binding [";
				tmp = tmp .. driver.getName();
				tmp = tmp .. "] FPS:";
				tmp = tmp .. fps;
				tmp = tmp .. " Height: ";
				tmp = tmp .. terrain.getHeight(camera.getAbsolutePosition().X, camera.getAbsolutePosition().Z);

				device.setWindowCaption(tmp);
				lastFPS = fps;
			end
		end
	end

	device.drop();
	
	return 0;
end

start();
