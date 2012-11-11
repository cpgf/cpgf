cpgf.import(nil, "builtin.core");

function overrideScreenShotFactory(screenshotFactory, device, templateName, node)
	local Device = device;
	local Number = 0;
	local FilenameTemplate = templateName;
	Node = node;
	
	screenshotFactory.OnEvent = function(me, event)
		if event.EventType == irr.EET_KEY_INPUT_EVENT and event.KeyInput.PressedDown then
			if event.KeyInput.Key == irr.KEY_F9 then
				local image = Device.getVideoDriver().createScreenShot();
				if image then
					Number = Number + 1;
					local buf = FilenameTemplate .. "_shot" .. Number .. ".jpg";
					Device.getVideoDriver().writeImageToFile(image, buf, 85 );
				end
			elseif event.KeyInput.Key == irr.KEY_F8 then
				if Node.isDebugDataVisible() then
					Node.setDebugDataVisible(irr.EDS_OFF);
				else
					Node.setDebugDataVisible(irr.EDS_BBOX_ALL);
				end
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

	local videoDim = irr.dimension2du(800,600);

	local device = irr.createDevice(driverType, videoDim, 32, false );

	if device == nil then
		return 1;
	end

	local mapname = "maps/20kdm2.bsp";

	local driver = device.getVideoDriver();
	local smgr = device.getSceneManager();
	local gui = device.getGUIEnvironment();

	device.getFileSystem().addFolderFileArchive("../../media/");
	device.getFileSystem().addZipFileArchive("../../media/map-20kdm2.pk3");

	smgr.getParameters().setAttribute(irr.ALLOW_ZWRITE_ON_TRANSPARENT, true);

	local mesh = cpgf.cast(smgr.getMesh(mapname), irr.IQ3LevelMesh);

	local node = nil;
	if mesh then
		local geometry = mesh.getMesh(irr.E_Q3_MESH_GEOMETRY);
		node = smgr.addOctreeSceneNode(geometry, nil, -1, 4096);
	end

	local screenshotFactory = irr.IEventReceiverWrapper();
	overrideScreenShotFactory(screenshotFactory, device, mapname, node);
	device.setEventReceiver(screenshotFactory);

	if  mesh  then
		local additional_mesh = mesh.getMesh(irr.E_Q3_MESH_ITEMS);

		for i = 0, additional_mesh.getMeshBufferCount() - 1 do
			local meshBuffer = additional_mesh.getMeshBuffer(i);
			local material = meshBuffer.getMaterial();

			local shaderIndex = material.MaterialTypeParam2;

			local shader = mesh.getShader(shaderIndex);
			if shader ~= nil then
				node = smgr.addQuake3SceneNode(meshBuffer, shader);
			end
		end
	end

	local camera = smgr.addCameraSceneNodeFPS();

	device.getCursorControl().setVisible(false);

	gui.addImage(driver.getTexture("irrlichtlogo2.png"), irr.position2d_s32(10, 10));

	local pos = irr.position2di(videoDim.Width - 128, videoDim.Height - 64);

	if driverType == irr.EDT_BURNINGSVIDEO then
		gui.addImage(driver.getTexture("burninglogo.png"), pos);

	elseif driverType == irr.EDT_OPENGL then
		gui.addImage(driver.getTexture("opengllogo.png"), pos);

	elseif driverType == irr.EDT_DIRECT3D8 or driverType == irr.EDT_DIRECT3D9 then
		gui.addImage(driver.getTexture("directxlogo.png"), pos);
	end

	local lastFPS = -1;

	while device.run() do
		if device.isWindowActive() then
			driver.beginScene(true, true, irr.SColor(255,20,20,40));
			smgr.drawAll();
			gui.drawAll();
			driver.endScene();

			local fps = driver.getFPS();
			local attr = smgr.getParameters();
			local tmp = "Q3 [";
			tmp = tmp .. driver.getName();
			tmp = tmp .. "] FPS:";
			tmp = tmp .. fps;
			tmp = tmp .. " Cull:";
			tmp = tmp .. attr.getAttributeAsInt("calls");
			tmp = tmp .. "/";
			tmp = tmp .. attr.getAttributeAsInt("culled");
			tmp = tmp .. " Draw: ";
			tmp = tmp .. attr.getAttributeAsInt("drawn_solid");
			tmp = tmp .. "/";
			tmp = tmp .. attr.getAttributeAsInt("drawn_transparent");
			tmp = tmp .. "/";
			tmp = tmp .. attr.getAttributeAsInt("drawn_transparent_effect");

			device.setWindowCaption(tmp);
			lastFPS = fps;
		end
	end


	return 0;
end

start();
