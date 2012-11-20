cpgf.import("cpgf", "builtin.collections");
cpgf.import("cpgf", "builtin.debug");

function overrideISceneNode(node)
{
	this.Box = new irr.aabbox3d_f32();
	this.Vertices = cpgf.createObjectArray(irr.S3DVertex);
	this.Material = new irr.SMaterial();

	this.Material.Wireframe = false;
	this.Material.Lighting = false;

	this.Vertices.setItem(0, new irr.S3DVertex(0,0,10, 1,1,0, new irr.SColor(255,0,255,255), 0, 1));
	this.Vertices.setItem(1, new irr.S3DVertex(10,0,-10, 1,0,0, new irr.SColor(255,255,0,255), 1, 1));
	this.Vertices.setItem(2, new irr.S3DVertex(0,20,0, 0,1,1, new irr.SColor(255,255,255,0), 1, 0));
	this.Vertices.setItem(3, new irr.S3DVertex(-10,0,-10, 0,0,1, new irr.SColor(255,0,255,0), 0, 0));

	this.Box.reset(this.Vertices.getItem(0).Pos);
	for(var i = 1; i < 4; ++i) {
		this.Box.addInternalPoint(this.Vertices.getItem(i).Pos);
	}

	var self = this;
			
	node.OnRegisterSceneNode = function(me) {
		if(me.isVisible())
			me.getSceneManager().registerNodeForRendering(me);

		me.super_OnRegisterSceneNode();
	}		

	node.render = function(me) {
		var indices = [	0, 2, 3, 2, 1, 3, 1, 0, 3, 2, 0, 1 ];
		var driver = me.getSceneManager().getVideoDriver();
		var indicesByteArray = cpgf.createByteArray();
		for(var i = 0; i < indices.length; ++i) indicesByteArray.writeInt16(indices[i]);

		driver.setMaterial(Material);
		driver.setTransform(irr.ETS_WORLD, me.getAbsoluteTransformation());
		driver.drawVertexPrimitiveList(self.Vertices.getItem(0), 4, indicesByteArray.getPointer(), 4, irr.EVT_STANDARD, irr.EPT_TRIANGLES, irr.EIT_16BIT);
	}

	node.getBoundingBox = function(me) {
		return self.Box;
	}

	node.getMaterialCount = function(me) {
		return 1;
	}

	node.getMaterial = function(me, i) {
		return self.Material;
	}	
}

function start()
{
	// ask user for driver
	var driverType = irr.driverChoiceConsole();
	if(driverType == irr.EDT_COUNT) {
		return 1;
	}

	// create device
	var device = irr.createDevice(driverType, new irr.dimension2d_u32(640, 480), 16, false);

	if (device == null) {
		return 1; // could not create selected driver.
	}


	// create engine and camera

	device.setWindowCaption("Custom Scene Node - cpgf Irrlicht JavaScript binding Demo");

	var driver = device.getVideoDriver();
	var smgr = device.getSceneManager();

	smgr.addCameraSceneNode(0, new irr.vector3df(0, -40, 0), new irr.vector3df(0, 0, 0));

	/*
	Create our scene node. I don't check the result of calling new, as it
	should throw an exception rather than returning 0 on failure. Because
	the new node will create itself with a reference count of 1, and then
	will have another reference added by its parent scene node when it is
	added to the scene, I need to drop my reference to it. Best practice is
	to drop it only *after* I have finished using it, regardless of what
	the reference count of the object is after creation.
	*/
	var myNode = new irr.ISceneNodeWrapper(smgr.getRootSceneNode(), smgr, 666);
	overrideISceneNode(myNode);
	
	/*
	To animate something in this boring scene consisting only of one
	tetraeder, and to show that you now can use your scene node like any
	other scene node in the engine, we add an animator to the scene node,
	which rotates the node a little bit.
	irr::scene::ISceneManager::createRotationAnimator() could return 0, so
	should be checked.
	*/
	var anim = smgr.createRotationAnimator(new irr.vector3df(0.8, 0, 0.8));

	if(anim)
	{
		myNode.addAnimator(anim);
		
		/*
		I'm done referring to anim, so must
		irr::IReferenceCounted::drop() this reference now because it
		was produced by a createFoo() function. As I shouldn't refer to
		it again, ensure that I can't by setting to 0.
		*/
		anim.drop();
		anim = null;
	}

	/*
	I'm done with my CSampleSceneNode object, and so must drop my reference.
	This won't delete the object, yet, because it is still attached to the
	scene graph, which prevents the deletion until the graph is deleted or the
	custom scene node is removed from it.
	*/
	// Don't drop myNode because it's created by new an object.
	// Script engine will garbage collect it.
//	myNode.drop();
	// Also don't set myNode to null, otherwise, it may be GCed.
//	myNode = null; // As I shouldn't refer to it again, ensure that I can't

	/*
	Now draw everything and finish.
	*/
	var frames = 0;
	while(device.run())
	{
		driver.beginScene(true, true, new irr.SColor(0, 100, 100, 100));

		smgr.drawAll();

		driver.endScene();
		if(++frames == 100)
		{
			var str = "cpgf Irrlicht JavaScript Binding Demo [";
			str = str + driver.getName();
			str = str + "] FPS:";
			str = str + driver.getFPS();

			device.setWindowCaption(str);
			frames = 0;
		}
	}

	device.drop();
	
	return 0;
}

start();
