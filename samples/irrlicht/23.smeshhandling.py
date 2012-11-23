import math

cpgf._import("cpgf", "builtin.core");
cpgf._import("cpgf", "builtin.debug");

def grey(x, y, z) :
	n = 255.0 * z;
	return irr.SColor(255, n, n, n);

def yellow(x, y, z) :
	return irr.SColor(255, 128 + 127.0 * x, 128 + 127.0 * y, 255);

def white(x, y, z) :
	return irr.SColor(255, 255, 255, 255);

def eggbox(x, y, s) :
	r = 4.0 * math.sqrt(x*x + y*y)/s;
	z = math.exp(-r * 2) * (math.cos(0.2 * x) + math.cos(0.2 * y));
	return 0.25 + 0.25 * z;

def moresine(x, y, s) :
	xx = 0.3 * x / s;
	yy = 12 * y / s;
	z = math.sin(xx*xx+yy) * math.sin(xx+yy*yy);
	return 0.25 + 0.25 * z;

def justexp(x, y, s) :
	xx=6*x/s;
	yy=6*y/s;
	z = (xx*xx+yy*yy);
	return 0.3 * z * math.cos(xx*yy);

class HeightMap :
	Width = 0;
	Height = 0;
	s = 0;
	data = None;

	def generate(self, f) :
		i=0;
		for y in range(self.Height) :
			for x in range(self.Width) :
				self.setAt(i, self.calc(f, x, y));
				i = i + 1;

	def height(self) :
		return self.Height;
		
	def width(self) :
		return self.Width;

	def calc(self, f, x, y) :
		xx = x - self.Width * 0.5;
		yy = y - self.Height * 0.5;
		return f(xx, yy, self.s);

	def setAt(self, i, z) :
		self.data._opArraySet(i, z);

	def get(self, x, y) :
		return self.data._opArrayGet(y * self.Width + x);

	def getnormal(self, x, y, s) :
		zc = self.get(x, y);
		zl = 0;
		zr = 0;
		zu = 0;
		zd = 0;

		if x == 0 :
			zr = self.get(x + 1, y);
			zl = zc + zc - zr;
		elif (x == self.Width - 1) :
			zl = self.get(x - 1, y);
			zr = zc + zc - zl;
		else :
			zr = self.get(x + 1, y);
			zl = self.get(x - 1, y);

		if y == 0 :
			zd = self.get(x, y + 1);
			zu = zc + zc - zd;
		elif (y == self.Height - 1) :
			zu = self.get(x, y - 1);
			zd = zc + zc - zu;
		else :
			zd = self.get(x, y + 1);
			zu = self.get(x, y - 1);

		return irr.vector3df(s * 2 * (zl - zr), 4, s * 2 * (zd - zu)).normalize();

def createHeightMap(w, h) :
	self = HeightMap();
	self.Width = w;
	self.Height = h;
	self.s = math.sqrt(self.Width * self.Width + self.Height * self.Height);
	self.data = irr.array_f32();
	self.data.set_used(self.Width * self.Height);
	return self;

class TMesh :
	Width = 0;
	Height = 0;
	Scale = 1.0;
	Mesh = irr.SMesh();


	def init(self, hm, scale, cf, driver) :
		self.Scale = scale;

		mp = driver.getMaximalPrimitiveCount();
		self.Width = hm.width();
		self.Height = hm.height();

		sw = mp / (6 * self.Height);

		i=0;
		for y0 in range(1) :
			y1 = y0 + sw;
			if y1 >= self.Height :
				y1 = self.Height - 1;
			self.addstrip(hm, cf, y0, y1, i);
			i = i + 1;
			
		if i<self.Mesh.getMeshBufferCount() :
			self.Mesh.MeshBuffers.erase(i,self.Mesh.getMeshBufferCount()-i);

		self.Mesh.recalculateBoundingBox();


	def addstrip(self, hm, cf, y0, y1, bufNum) :
		buf = None;
		if bufNum< self.Mesh.getMeshBufferCount() :
			buf = cpgf.cast(self.Mesh.getMeshBuffer(bufNum), irr.SMeshBuffer);
		else :
			buf = irr.SMeshBuffer();
			self.Mesh.addMeshBuffer(buf);

		buf.Vertices.set_used((1 + y1 - y0) * self.Width);

		i=0;
		for y in range(y0, y1 + 1) :
			for x in range(self.Width) :
				z = hm.get(x, y);
				xx = x/self.Width;
				yy = y/self.Height;

				v = buf.Vertices._opArrayGet(i);
				i = i + 1;
				v.Pos.set(x, self.Scale * z, y);
				v.Normal.set(hm.getnormal(x, y, self.Scale));
				v.Color.color = cf(xx, yy, z).color;
				v.TCoords.set(xx, yy);

		buf.Indices.set_used(6 * (self.Width - 1) * (y1 - y0));
		i=0;
		for y in range(y0, y1) :
			for x in range(self.Width - 1) :
				n = (y-y0) * self.Width + x;
				buf.Indices._opArraySet(i, n);
				i = i + 1;
				buf.Indices._opArraySet(i, n + self.Height);
				i = i + 1;
				buf.Indices._opArraySet(i, n + self.Height + 1);
				i = i + 1;
				buf.Indices._opArraySet(i, n + self.Height + 1);
				i = i + 1;
				buf.Indices._opArraySet(i, n + 1);
				i = i + 1;
				buf.Indices._opArraySet(i, n);
				i = i + 1;

		buf.recalculateBoundingBox();

def createTMesh() :
	self = TMesh();
	self.Width = 0;
	self.Height = 0;
	self.Scale = 1.0;
	self.Mesh = irr.SMesh();
	return self;
	
KeyIsDown = [];

def makeMyEventReceiver(receiver) :
	for i in range(irr.KEY_KEY_CODES_COUNT) :
		KeyIsDown.append(False);

	def OnEvent(me, event) :
		if event.EventType == irr.EET_KEY_INPUT_EVENT :
			KeyIsDown[event.KeyInput.Key + 1] = event.KeyInput.PressedDown;
		return False;

	receiver.OnEvent = OnEvent;


def IsKeyDown(keyCode) :
	return KeyIsDown[keyCode + 1];


def start() :
	driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT :
		return 1;

	MyEventReceiver = cpgf.cloneClass(irr.IEventReceiverWrapper);
	makeMyEventReceiver(MyEventReceiver);
	receiver = MyEventReceiver();

	device = irr.createDevice(driverType, irr.dimension2d_u32(800, 600), 32, False, False, False, receiver);

	if device == None :
		return 1;

	driver = device.getVideoDriver();
	smgr = device.getSceneManager();
	device.setWindowCaption("cpgf Irrlicht Python binding Example for SMesh usage.");

	mesh = createTMesh();
	hm = createHeightMap(10, 10);
	hm.generate(eggbox);
	mesh.init(hm, 50.0, grey, driver);

	meshnode = smgr.addMeshSceneNode(mesh.Mesh);
	meshnode.setMaterialFlag(irr.EMF_BACK_FACE_CULLING, False);

	node = smgr.addLightSceneNode(None, irr.vector3df(0,100,0), irr.SColorf(1.0, 0.6, 0.7, 1.0), 500.0);
	if node :
		node.getLightData().Attenuation.set(0.0, 1.0/500.0, 0.0);
		anim = smgr.createFlyCircleAnimator(irr.vector3df(0,150,0),250.0);
		if anim :
			node.addAnimator(anim);
			anim.drop();

	camera = smgr.addCameraSceneNodeFPS();
	if camera :
		camera.setPosition(irr.vector3df(-20.0, 150.0, -20.0));
		camera.setTarget(irr.vector3df(200.0, -80.0, 150.0));
		camera.setFarValue(20000.00);

	while device.run() :
		if not device.isWindowActive() :
			device.sleep(100);
			continue;

		if IsKeyDown(irr.KEY_KEY_W) :
			meshnode.setMaterialFlag(irr.EMF_WIREFRAME, not meshnode.getMaterial(0).Wireframe);
		elif(IsKeyDown(irr.KEY_KEY_1)) :
			hm.generate(eggbox);
			mesh.init(hm, 50.0, grey, driver);
		elif(IsKeyDown(irr.KEY_KEY_2)) :
			hm.generate(moresine);
			mesh.init(hm, 50.0, yellow, driver);
		elif(IsKeyDown(irr.KEY_KEY_3)) :
			hm.generate(justexp);
			mesh.init(hm, 50.0, yellow, driver);

		driver.beginScene(True, True, irr.SColor(255, 0, 0, 0));
		smgr.drawAll();
		driver.endScene();

	device.drop();

	return 0;

start();
