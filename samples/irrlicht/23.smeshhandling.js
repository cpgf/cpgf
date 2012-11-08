cpgf.import(null, "builtin.core");
cpgf.import(null, "builtin.debug");

/* Here comes a set of functions which can be used for coloring the nodes while
creating the mesh. */

// Greyscale, based on the height.
function grey(x, y, z)
{
	var n = 255.0 * z;
	return new irr.SColor(255, n, n, n);
}

// Interpolation between blue and white, with red added in one
// direction and green in the other.
function yellow(x, y, z)
{
	return new irr.SColor(255, 128 + (127.0 * x), 128 + (127.0 * y), 255);
}

// Pure white.
function white(x, y, z) { return new irr.SColor(255, 255, 255, 255); }

/* The type of the functions which generate the heightmap. x and y
range between -0.5 and 0.5, and s is the scale of the heightmap. */

// An interesting sample function :-)
function eggbox(x, y, s)
{
	var r = 4.0 * Math.sqrt(x*x + y*y)/s;
	var z = Math.exp(-r * 2) * (Math.cos(0.2 * x) + Math.cos(0.2 * y));
	return 0.25 + 0.25 * z;
}

// A rather dumb sine function :-/
function moresine(x, y, s)
{
	var xx= 0.3 * x / s;
	var yy=12*y/s;
	var z = Math.sin(xx*xx+yy)*Math.sin(xx+yy*yy);
	return 0.25 + 0.25 * z;
}

// A simple function
function justexp(x, y, s)
{
	var xx=6*x/s;
	var yy=6*y/s;
	var z = (xx*xx+yy*yy);
	return 0.3 * z * Math.cos(xx*yy);
}

/* A simple class for representing heightmaps. Most of this should be obvious. */

function HeightMap(_w, _h)
{
	this.Width = _w;
	this.Height = _h;
	this.s = 0.0;
	this.data = [];

	this.s = Math.sqrt(this.Width * this.Width + this.Height * this.Height);

	// The height at (x, y) is at position y * Width + x.

	this.set = function(x, y, z) { this.data[y * this.Width + x] = z; }
	this.setAt = function(i, z) { this.data[i] = z; }
	this.get = function(x, y) { return this.data[y * this.Width + x]; }

	// Fill the heightmap with values generated from f.
	this.generate = function(f)
	{
		var i=0;
		for(var y = 0; y < this.Height; ++y)
			for(var x = 0; x < this.Width; ++x)
				this.set(i++, this.calc(f, x, y));
	}

	this.height = function() { return this.Height; }
	this.width = function() { return this.Width; }

	this.calc = function(f, x, y)
	{
		var xx = x - this.Width*0.5;
		var yy = y - this.Height*0.5;
		return f(xx, yy, this.s);
	}

	/* The only difficult part. This considers the normal at (x, y) to
	be the cross product of the vectors between the adjacent points
	in the horizontal and vertical directions.

	s is a scaling factor, which is necessary if the height units are
	different from the coordinate units; for example, if your map has
	heights in metres and the coordinates are in units of a
	kilometer. */

	this.getnormal = function(x, y, s)
	{
		var zc = this.get(x, y);
		var zl, zr, zu, zd;

		if (x == 0)
		{
			zr = this.get(x + 1, y);
			zl = zc + zc - zr;
		}
		else if (x == this.Width - 1)
		{
			zl = this.get(x - 1, y);
			zr = zc + zc - zl;
		}
		else
		{
			zr = this.get(x + 1, y);
			zl = this.get(x - 1, y);
		}

		if (y == 0)
		{
			zd = this.get(x, y + 1);
			zu = zc + zc - zd;
		}
		else if (y == this.Height - 1)
		{
			zu = this.get(x, y - 1);
			zd = zc + zc - zu;
		}
		else
		{
			zd = this.get(x, y + 1);
			zu = this.get(x, y - 1);
		}

		return new irr.vector3df(s * 2 * (zl - zr), 4, s * 2 * (zd - zu)).normalize();
	}
}

/* A class which generates a mesh from a heightmap. */
function TMesh()
{
	this.Width = 0;
	this.Height = 0;
	this.Scale = 1.0;
	this.Mesh = new irr.SMesh();

	// Unless the heightmap is small, it won't all fit into a single
	// SMeshBuffer. This function chops it into pieces and generates a
	// buffer from each one.

	this.init = function(hm, scale, cf, driver)
	{
		this.Scale = scale;

		var mp = driver.getMaximalPrimitiveCount();
		this.Width = hm.width();
		this.Height = hm.height();
		
		var sw = mp / (6 * this.Height);

		var i=0;
		for(var y0 = 0; y0 < this.Height; y0 += sw)
		{
			var y1 = y0 + sw;
			if (y1 >= this.Height)
				y1 = this.Height - 1; // the last one might be narrower
			this.addstrip(hm, cf, y0, y1, i);
			++i;
		}
		if (i<this.Mesh.getMeshBufferCount())
		{
			// clear the rest
//			for (var j=i; j<this.Mesh.getMeshBufferCount(); ++j)
//			{
//				this.Mesh.getMeshBuffer(j).drop();
//			}
			this.Mesh.MeshBuffers.erase(i,this.Mesh.getMeshBufferCount()-i);
		}

		this.Mesh.recalculateBoundingBox();
	}

	// Generate a SMeshBuffer which represents all the vertices and
	// indices for values of y between y0 and y1, and add it to the
	// mesh.

	this.addstrip = function(hm, cf, y0, y1, bufNum)
	{
/*
		var buf = null;
		if (bufNum<Mesh.getMeshBufferCount())
		{
			buf = cpgf.cast(this.Mesh.getMeshBuffer(bufNum), irr.SMeshBuffer);
		}
		else
		{
			// create new buffer
			buf = new irr.SMeshBuffer();
			Mesh.addMeshBuffer(buf);
			// to simplify things we drop here but continue using buf
//			buf.drop();
		}
		buf.Vertices.set_used((1 + y1 - y0) * Width);

		var i=0;
		for (var y = y0; y <= y1; ++y)
		{
			for (var x = 0; x < this.Width; ++x)
			{
				var z = hm.get(x, y);
				var xx = x/this.Width;
				var yy = y/this.Height;

				S3DVertex& v = buf.Vertices[i++];
				v.Pos.set(x, Scale * z, y);
				v.Normal.set(hm.getnormal(x, y, Scale));
				v.Color=cf(xx, yy, z);
				v.TCoords.set(xx, yy);
			}
		}

		buf.Indices.set_used(6 * (Width - 1) * (y1 - y0));
		i=0;
		for(u16 y = y0; y < y1; ++y)
		{
			for(u16 x = 0; x < Width - 1; ++x)
			{
				const u16 n = (y-y0) * Width + x;
				buf.Indices[i]=n;
				buf.Indices[++i]=n + Height;
				buf.Indices[++i]=n + Height + 1;
				buf.Indices[++i]=n + Height + 1;
				buf.Indices[++i]=n + 1;
				buf.Indices[++i]=n;
				++i;
			}
		}

		buf.recalculateBoundingBox();
*/
	}
}

/*
Our event receiver implementation, taken from tutorial 4.
*/
var KeyIsDown = [];
function IsKeyDown(keyCode)
{
	return KeyIsDown[keyCode];
}

function overrideEventReceiver(receiver)
{
	for (var i=0; i<irr.KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
	// This is the one method that we have to implement
	receiver.OnEvent = function(me, event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr.EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}
};

/*
Much of this is code taken from some of the examples. We merely set
up a mesh from a heightmap, light it with a moving light, and allow
the user to navigate around it.
*/

function start()
{
	// ask user for driver
	var driverType=irr.driverChoiceConsole();
	if (driverType==irr.EDT_COUNT)
		return 1;

	var receiver = new irr.IEventReceiverWrapper();
	overrideEventReceiver(receiver);
	var device = irr.createDevice(driverType, new irr.dimension2du(800, 600), 32, false, false, false, receiver);

	if(device == null)
		return 1;
 
	var driver = device.getVideoDriver();
	var smgr = device.getSceneManager();
	device.setWindowCaption("Irrlicht Example for SMesh usage.");

	/*
	Create the custom mesh and initialize with a heightmap
	*/
	var mesh = new TMesh();
	var hm = new HeightMap(255, 255);
	hm.generate(eggbox);
	mesh.init(hm, 50.0, grey, driver);

	// Add the mesh to the scene graph
	var meshnode = smgr.addMeshSceneNode(mesh.Mesh);
	meshnode.setMaterialFlag(irr.EMF_BACK_FACE_CULLING, false);

	// light is just for nice effects
	var node = smgr.addLightSceneNode(0, new irr.vector3df(0,100,0), new irr.SColorf(1.0, 0.6, 0.7, 1.0), 500.0);
	if (node)
	{
		node.getLightData().Attenuation.set(0.0, 1.0/500.0, 0.0);
		var anim = smgr.createFlyCircleAnimator(new irr.vector3df(0,150,0),250.0);
		if (anim)
		{
			node.addAnimator(anim);
//			anim.drop();
		}
	}

	var camera = smgr.addCameraSceneNodeFPS();
	if (camera)
	{
		camera.setPosition(new irr.vector3df(-20.0, 150.0, -20.0));
		camera.setTarget(new irr.vector3df(200.0, -80.0, 150.0));
		camera.setFarValue(20000.0);
	}

	/*
	Just a usual render loop with event handling. The custom mesh is
	a usual part of the scene graph which gets rendered by drawAll.
	*/
	while(device.run())
	{
		if(!device.isWindowActive())
		{
			device.sleep(100);
			continue;
		}

		if(IsKeyDown(irr.KEY_KEY_W))
		{
			meshnode.setMaterialFlag(irr.EMF_WIREFRAME, !meshnode.getMaterial(0).Wireframe);
		}
		else if(IsKeyDown(irr.KEY_KEY_1))
		{
			hm.generate(eggbox);
			mesh.init(hm, 50.0, grey, driver);
		}
		else if(IsKeyDown(irr.KEY_KEY_2))
		{
			hm.generate(moresine);
			mesh.init(hm, 50.0, yellow, driver);
		}
		else if(IsKeyDown(irr.KEY_KEY_3))
		{
			hm.generate(justexp);
			mesh.init(hm, 50.0, yellow, driver);
		}

		driver.beginScene(true, true, new irr.SColor(0xff000000));
		smgr.drawAll();
		driver.endScene();
	}

//	device.drop();

	return 0;
}

cpgf.traceLine("Sorry, this example won't work because the function addstrip can't be implemented due to it using irr::array.");
cpgf.traceLine("You will only see black screen if you continue running it.");
//start();
