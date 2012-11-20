cpgf.import("cpgf", "builtin.collections");
cpgf.import("cpgf", "builtin.debug");

function overrideISceneNode(node)

	local Box = irr.aabbox3d_f32();
	local Vertices = cpgf.createObjectArray(irr.S3DVertex);
	local Material = irr.SMaterial();

	Material.Wireframe = false;
	Material.Lighting = false;

	Vertices.setItem(0, irr.S3DVertex(0,0,10, 1,1,0, irr.SColor(255,0,255,255), 0, 1));
	Vertices.setItem(1, irr.S3DVertex(10,0,-10, 1,0,0, irr.SColor(255,255,0,255), 1, 1));
	Vertices.setItem(2, irr.S3DVertex(0,20,0, 0,1,1, irr.SColor(255,255,255,0), 1, 0));
	Vertices.setItem(3, irr.S3DVertex(-10,0,-10, 0,0,1, irr.SColor(255,0,255,0), 0, 0));

	Box.reset(Vertices.getItem(0).Pos);
	for i = 1, 3 do
		Box.addInternalPoint(Vertices.getItem(i).Pos);
	end

	node.OnRegisterSceneNode = function(me)
		if me.isVisible() then
			me.getSceneManager().registerNodeForRendering(me);
		end

		me.super_OnRegisterSceneNode();
	end

	node.render = function(me)
		local indices = {	0, 2, 3, 2, 1, 3, 1, 0, 3, 2, 0, 1 };
		local driver = me.getSceneManager().getVideoDriver();
		local indicesByteArray = cpgf.createByteArray();
		for i = 1, #indices do
			indicesByteArray.writeInt16(indices[i]);
		end

		driver.setMaterial(Material);
		driver.setTransform(irr.ETS_WORLD, me.getAbsoluteTransformation());
		driver.drawVertexPrimitiveList(Vertices.getItem(0), 4, indicesByteArray.getPointer(), 4, irr.EVT_STANDARD, irr.EPT_TRIANGLES, irr.EIT_16BIT);
	end

	node.getBoundingBox = function(me)
		return Box;
	end

	node.getMaterialCount = function(me)
		return 1;
	end

	node.getMaterial = function(me, i)
		return Material;
	end
end

function start()


	local driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT then
		return 1;
	end

	local device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480), 16, false);

	if device == nil then
		return 1;
	end

	device.setWindowCaption("Custom Scene Node - cpgf Irrlicht Lua binding Demo");

	local driver = device.getVideoDriver();
	local smgr = device.getSceneManager();

	smgr.addCameraSceneNode(nil, irr.vector3df(0, -40, 0), irr.vector3df(0, 0, 0));

	local myNode = irr.ISceneNodeWrapper(smgr.getRootSceneNode(), smgr, 666);
	overrideISceneNode(myNode);
	
	local anim = smgr.createRotationAnimator(irr.vector3df(0.8, 0, 0.8));

	if anim then
		myNode.addAnimator(anim);
		anim.drop();
		anim = nil;
	end

	local frames = 0;
	while device.run() do

		driver.beginScene(true, true, irr.SColor(0, 100, 100, 100));

		smgr.drawAll();

		driver.endScene();
		frames = frames + 1;
		if frames == 100 then

			local str = "cpgf Irrlicht Lua Binding Demo [";
			str = str .. driver.getName();
			str = str .. "] FPS:";
			str = str .. driver.getFPS();

			device.setWindowCaption(str);
			frames = 0;
		end
	end
	
	device.drop();
	
	return 0;
end

start();
