local resourcePath = "../../media/";
local ID_IsNotPickable = 0;
local IDFlag_IsPickable = 1;
local IDFlag_IsHighlightable = 2;

function start()
	local driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT then
		return 1;
	end

	local device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480), 16, false);

	if device == nil then
		return 1;
	end

	local driver = device.getVideoDriver();
	local smgr = device.getSceneManager();

	device.getFileSystem().addZipFileArchive(resourcePath .. "map-20kdm2.pk3");

	local q3levelmesh = smgr.getMesh("20kdm2.bsp");
	local q3node = nil;

	if q3levelmesh then
		q3node = smgr.addOctreeSceneNode(q3levelmesh.getMesh(0), nil, IDFlag_IsPickable);
	end
	
	local selector = nil;

	if q3node then
		q3node.setPosition(irr.vector3df(-1350,-130,-1400));

		selector = smgr.createOctreeTriangleSelector(q3node.getMesh(), q3node, 128);
		q3node.setTriangleSelector(selector);
	end

	local camera = smgr.addCameraSceneNodeFPS(nil, 100.0, 0.3, ID_IsNotPickable, nil, 0, true, 3.0);
	camera.setPosition(irr.vector3df(50,50,-60));
	camera.setTarget(irr.vector3df(-70,30,-60));

	if selector then
		local anim = smgr.createCollisionResponseAnimator(selector, camera, irr.vector3df(30,50,30),	irr.vector3df(0,-10,0), irr.vector3df(0,30,0));
		selector.drop();
		camera.addAnimator(anim);
		anim.drop();
	end

	device.getCursorControl().setVisible(false);
	
	local bill = smgr.addBillboardSceneNode();
	bill.setMaterialType(irr.EMT_TRANSPARENT_ADD_COLOR );
	bill.setMaterialTexture(0, driver.getTexture(resourcePath .. "particle.bmp"));
	bill.setMaterialFlag(irr.EMF_LIGHTING, false);
	bill.setMaterialFlag(irr.EMF_ZBUFFER, false);
	bill.setSize(irr.dimension2d_f32(20.0, 20.0));
	bill.setID(ID_IsNotPickable);

	local node = nil;

	node = smgr.addAnimatedMeshSceneNode(smgr.getMesh(resourcePath .. "faerie.md2"),	nil, IDFlag_IsPickable + IDFlag_IsHighlightable);
	node.setPosition(irr.vector3df(-70,-15,-120));
	node.setScale(irr.vector3df(2, 2, 2));
	node.setMD2Animation(irr.EMAT_POINT);
	node.setAnimationSpeed(20.0);
	local material = irr.SMaterial();
	material.setTexture(0, driver.getTexture(resourcePath .. "/faerie2.bmp"));
	material.Lighting = true;
	material.NormalizeNormals = true;

	node.getMaterial(0).setTexture(0, driver.getTexture(resourcePath .. "/faerie2.bmp"));
	node.getMaterial(0).Lighting = true;
	node.getMaterial(0).NormalizeNormals = true;

	selector = smgr.createTriangleSelector(node);
	node.setTriangleSelector(selector);
	selector.drop();

	node = smgr.addAnimatedMeshSceneNode(smgr.getMesh(resourcePath .. "dwarf.x"), nil, IDFlag_IsPickable + IDFlag_IsHighlightable);
	node.setPosition(irr.vector3df(-70,-66,0));
	node.setRotation(irr.vector3df(0,-90,0));
	node.setAnimationSpeed(20.0);
	selector = smgr.createTriangleSelector(node);
	node.setTriangleSelector(selector);
	selector.drop();

	node = smgr.addAnimatedMeshSceneNode(smgr.getMesh(resourcePath .. "ninja.b3d"), nil, IDFlag_IsPickable + IDFlag_IsHighlightable);
	node.setScale(irr.vector3df(10, 10, 10));
	node.setPosition(irr.vector3df(-70,-66,-60));
	node.setRotation(irr.vector3df(0,90,0));
	node.setAnimationSpeed(10.0);
	node.getMaterial(0).NormalizeNormals = true;

	selector = smgr.createTriangleSelector(node);
	node.setTriangleSelector(selector);
	selector.drop();

	material.setTexture(0, nil);
	material.Lighting = false;

	local light = smgr.addLightSceneNode(nil, irr.vector3df(-60,100,400), irr.SColorf(1.0,1.0,1.0,1.0), 600.0);
	light.setID(ID_IsNotPickable);

	local highlightedSceneNode = nil;
	local collMan = smgr.getSceneCollisionManager();
	local lastFPS = -1;

	material.Wireframe=true;

	while device.run() do
		if device.isWindowActive() then
			driver.beginScene(true, true, 0);
			smgr.drawAll();

			if highlightedSceneNode then
				highlightedSceneNode.setMaterialFlag(irr.EMF_LIGHTING, true);
				highlightedSceneNode = nil;
			end

			local s = camera.getPosition();
			--local e = s._opAdd(camera.getTarget()._opSub(s).normalize()._opMul(1000.0));
			-- We use Lua operator overload to calculate it
			local e = camera.getTarget() - s;
			e = s + e.normalize() * 1000.0;
			local ray = irr.line3d_f32(s.X, s.Y, s.Z, e.X, e.Y, e.Z);

			local intersection = irr.vector3df();
			local hitTriangle = irr.triangle3df();
			local selectedSceneNode = collMan.getSceneNodeAndCollisionPointFromRay(ray,	intersection, hitTriangle, IDFlag_IsPickable, nil);

			if selectedSceneNode then
				bill.setPosition(intersection);
				
				driver.setTransform(irr.ETS_WORLD, irr.matrix4());
				driver.setMaterial(material);
				driver.draw3DTriangle(hitTriangle, irr.SColor(0,255,0,0));

				-- if (selectedSceneNode.getID() & IDFlag_IsHighlightable) == IDFlag_IsHighlightable then
				if selectedSceneNode.getID() > 1 then
					highlightedSceneNode = selectedSceneNode;
					highlightedSceneNode.setMaterialFlag(irr.EMF_LIGHTING, false);
				end
			end

			driver.endScene();

			local fps = driver.getFPS();

			if lastFPS ~= fps then

				local str = "Collision detection example - cpgf Irrlicht Lua Binding [";
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
