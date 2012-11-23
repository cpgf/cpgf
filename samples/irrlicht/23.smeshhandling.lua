cpgf.import("cpgf", "builtin.core");
cpgf.import("cpgf", "builtin.debug");

function grey(x, y, z)
	local n = 255.0 * z;
	return irr.SColor(255, n, n, n);
end

function yellow(x, y, z)
	return irr.SColor(255, 128 + 127.0 * x, 128 + 127.0 * y, 255);
end

function white(x, y, z)
	return irr.SColor(255, 255, 255, 255);
end

function eggbox(x, y, s)
	local r = 4.0 * math.sqrt(x*x + y*y)/s;
	local z = math.exp(-r * 2) * (math.cos(0.2 * x) + math.cos(0.2 * y));
	return 0.25 + 0.25 * z;
end

function moresine(x, y, s)
	local xx = 0.3 * x / s;
	local yy = 12 * y / s;
	local z = math.sin(xx*xx+yy) * math.sin(xx+yy*yy);
	return 0.25 + 0.25 * z;
end

function justexp(x, y, s)
	local xx=6*x/s;
	local yy=6*y/s;
	local z = (xx*xx+yy*yy);
	return 0.3 * z * math.cos(xx*yy);
end

local HeightMap = {
	Width = 0,
	Height = 0,
	s = 0,
	data = nil
}

function createHeightMap(w, h)
	HeightMap.Width = w;
	HeightMap.Height = h;
	HeightMap.s = math.sqrt(HeightMap.Width * HeightMap.Width + HeightMap.Height * HeightMap.Height);
	HeightMap.data = irr.array_f32();
	HeightMap.data.set_used(HeightMap.Width * HeightMap.Height);

	return HeightMap;
end

function generate(hm, f)
	local i=0;
	for y = 0, hm.Height - 1 do
		for x = 0, hm.Width - 1 do
			setAt(hm, i, calc(hm, f, x, y));
			i = i + 1;
		end
	end
end

function height(hm)
	return hm.Height;
end

function width(hm)
	return hm.Width;
end

function calc(hm, f, x, y)
	local xx = x - hm.Width * 0.5;
	local yy = y - hm.Height * 0.5;
	return f(xx, yy, hm.s);
end

function setAt(hm, i, z)
	hm.data._opArraySet(i, z);
end

function get(hm, x, y)
	return hm.data._opArrayGet(y * hm.Width + x);
end

function getnormal(hm, x, y, s)
	local zc = get(hm, x, y);
	local zl = 0;
	local zr = 0;
	local zu = 0;
	local zd = 0;

	if x == 0 then
		zr = get(hm, x + 1, y);
		zl = zc + zc - zr;
	elseif x == hm.Width - 1 then
		zl = get(hm, x - 1, y);
		zr = zc + zc - zl;
	else
		zr = get(hm, x + 1, y);
		zl = get(hm, x - 1, y);
	end

	if y == 0 then
		zd = get(hm, x, y + 1);
		zu = zc + zc - zd;
	elseif y == hm.Height - 1 then
		zu = get(hm, x, y - 1);
		zd = zc + zc - zu;
	else
		zd = get(hm, x, y + 1);
		zu = get(hm, x, y - 1);
	end
	
	return irr.vector3df(s * 2 * (zl - zr), 4, s * 2 * (zd - zu)).normalize();
end


local TMesh = {
	Width = 0,
	Height = 0,
	Scale = 1.0,
	Mesh = irr.SMesh()
}

function init(tm, hm, scale, cf, driver)
	tm.Scale = scale;

	local mp = driver.getMaximalPrimitiveCount();
	tm.Width = width(hm);
	tm.Height = height(hm);

	local sw = mp / (6 * tm.Height);

	local i=0;
	for y0 = 0, 1 do
		local y1 = y0 + sw;
		if y1 >= tm.Height then
			y1 = tm.Height - 1;
		end
		addstrip(tm, hm, cf, y0, y1, i);
		i = i + 1;
	end
	if i<tm.Mesh.getMeshBufferCount() then
		tm.Mesh.MeshBuffers.erase(i,tm.Mesh.getMeshBufferCount()-i);
	end

	tm.Mesh.recalculateBoundingBox();
end


function addstrip(tm, hm, cf, y0, y1, bufNum)
	local buf = nil;
	if bufNum< tm.Mesh.getMeshBufferCount() then
		buf = cpgf.cast(tm.Mesh.getMeshBuffer(bufNum), irr.SMeshBuffer);
	else
		buf = irr.SMeshBuffer();
		tm.Mesh.addMeshBuffer(buf);
	end
	buf.Vertices.set_used((1 + y1 - y0) * tm.Width);

	local i=0;
	for y = y0, y1 do
		for x = 0, tm.Width - 1 do
			local z = get(hm, x, y);
			local xx = x/tm.Width;
			local yy = y/tm.Height;

			local v = buf.Vertices._opArrayGet(i);
			i = i + 1;
			v.Pos.set(x, tm.Scale * z, y);
			v.Normal.set(getnormal(hm, x, y, tm.Scale));
			v.Color.color = cf(xx, yy, z).color;
			v.TCoords.set(xx, yy);
		end
	end

	buf.Indices.set_used(6 * (tm.Width - 1) * (y1 - y0));
	i=0;
	for y = y0, y1 - 1 do
		for x = 0, tm.Width - 2 do
			local n = (y-y0) * tm.Width + x;
			buf.Indices._opArraySet(i, n);
			i = i + 1;
			buf.Indices._opArraySet(i, n + tm.Height);
			i = i + 1;
			buf.Indices._opArraySet(i, n + tm.Height + 1);
			i = i + 1;
			buf.Indices._opArraySet(i, n + tm.Height + 1);
			i = i + 1;
			buf.Indices._opArraySet(i, n + 1);
			i = i + 1;
			buf.Indices._opArraySet(i, n);
			i = i + 1;
		end
	end

	buf.recalculateBoundingBox();
end

KeyIsDown = {};

function makeMyEventReceiver(receiver)
	for i = 1, irr.KEY_KEY_CODES_COUNT do
		KeyIsDown[i] = false;
	end

	receiver.OnEvent = function(me, event)
		if event.EventType == irr.EET_KEY_INPUT_EVENT then
			KeyIsDown[event.KeyInput.Key + 1] = event.KeyInput.PressedDown;
		end

		return false;
	end

end


function IsKeyDown(keyCode)
	return KeyIsDown[keyCode + 1];
end

function start()
	local driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT then
		return 1;
	end

	local MyEventReceiver = cpgf.cloneClass(irr.IEventReceiverWrapper);
	makeMyEventReceiver(MyEventReceiver);
	local receiver = MyEventReceiver();

	local device = irr.createDevice(driverType, irr.dimension2d_u32(800, 600), 32, false, false, false, receiver);

	if device == nil then
		return 1;
	end

	local driver = device.getVideoDriver();
	local smgr = device.getSceneManager();
	device.setWindowCaption("cpgf Irrlicht Lua binding Example for SMesh usage.");

	local mesh = TMesh;
	local hm = createHeightMap(10, 10);
	generate(hm, eggbox);
	init(mesh, hm, 50.0, grey, driver);

	local meshnode = smgr.addMeshSceneNode(mesh.Mesh);
	meshnode.setMaterialFlag(irr.EMF_BACK_FACE_CULLING, false);

	local node = smgr.addLightSceneNode(nil, irr.vector3df(0,100,0), irr.SColorf(1.0, 0.6, 0.7, 1.0), 500.0);
	if node then
		node.getLightData().Attenuation.set(0.0, 1.0/500.0, 0.0);
		local anim = smgr.createFlyCircleAnimator(irr.vector3df(0,150,0),250.0);
		if anim then
			node.addAnimator(anim);
			anim.drop();
		end
	end

	local camera = smgr.addCameraSceneNodeFPS();
	if camera then
		camera.setPosition(irr.vector3df(-20.0, 150.0, -20.0));
		camera.setTarget(irr.vector3df(200.0, -80.0, 150.0));
		camera.setFarValue(20000.00);
	end

	while device.run() do
		if not device.isWindowActive() then
			device.sleep(100);
--			continue;
		end

		if IsKeyDown(irr.KEY_KEY_W) then
			meshnode.setMaterialFlag(irr.EMF_WIREFRAME, not meshnode.getMaterial(0).Wireframe);
		elseif(IsKeyDown(irr.KEY_KEY_1)) then
			generate(hm, eggbox);
			init(mesh, hm, 50.0, grey, driver);
		elseif(IsKeyDown(irr.KEY_KEY_2)) then
			generate(hm, moresine);
			init(mesh, hm, 50.0, yellow, driver);
		elseif(IsKeyDown(irr.KEY_KEY_3)) then
			generate(hm, justexp);
			init(mesh, hm, 50.0, yellow, driver);
		end

		driver.beginScene(true, true, irr.SColor(0xff000000));
		smgr.drawAll();
		driver.endScene();
	end

	device.drop();

	return 0;
end

start();
