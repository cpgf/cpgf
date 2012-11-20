var resourcePath = "../../media/";

// I use this ISceneNode ID to indicate a scene node that is
// not pickable by getSceneNodeAndCollisionPointFromRay()
var ID_IsNotPickable = 0;

// I use this flag in ISceneNode IDs to indicate that the
// scene node can be picked by ray selection.
var IDFlag_IsPickable = 1 << 0;

// I use this flag in ISceneNode IDs to indicate that the
// scene node can be highlighted.  In this example, the
// homonids can be highlighted, but the level mesh can't.
var IDFlag_IsHighlightable = 1 << 1;

function start()
{
	// ask user for driver
	var driverType = irr.driverChoiceConsole();
	if(driverType == irr.EDT_COUNT)
		return 1;

	// create device
	var device = irr.createDevice(driverType, new irr.dimension2d_u32(640, 480), 16, false);

	if (device == null)
		return 1; // could not create selected driver.

	var driver = device.getVideoDriver();
	var smgr = device.getSceneManager();

	device.getFileSystem().addZipFileArchive(resourcePath + "map-20kdm2.pk3");

	var q3levelmesh = smgr.getMesh("20kdm2.bsp");
	var q3node = null;

	// The Quake mesh is pickable, but doesn't get highlighted.
	if(q3levelmesh)
		q3node = smgr.addOctreeSceneNode(q3levelmesh.getMesh(0), 0, IDFlag_IsPickable);

	// So far so good, we've loaded the quake 3 level like in tutorial 2. Now,
	// here comes something different: We create a triangle selector. A
	// triangle selector is a class which can fetch the triangles from scene
	// nodes for doing different things with them, for example collision
	// detection. There are different triangle selectors, and all can be
	// created with the ISceneManager. In this example, we create an
	// OctreeTriangleSelector, which optimizes the triangle output a little
	// bit by reducing it like an octree. This is very useful for huge meshes
	// like quake 3 levels. After we created the triangle selector, we attach
	// it to the q3node. This is not necessary, but in this way, we do not
	// need to care for the selector, for example dropping it after we do not
	// need it anymore.

	var selector = null;

	if(q3node)
	{
		q3node.setPosition(new irr.vector3df(-1350,-130,-1400));

		selector = smgr.createOctreeTriangleSelector(q3node.getMesh(), q3node, 128);
		q3node.setTriangleSelector(selector);
		// We're not done with this selector yet, so don't drop it.
	}


	// We add a first person shooter camera to the scene so that we can see and
	// move in the quake 3 level like in tutorial 2. But this, time, we add a
	// special animator to the camera: A Collision Response animator. This
	// animator modifies the scene node to which it is attached to in order to
	// prevent it moving through walls, and to add gravity to it. The
	// only thing we have to tell the animator is how the world looks like,
	// how big the scene node is, how much gravity to apply and so on. After the
	// collision response animator is attached to the camera, we do not have to do
	// anything more for collision detection, anything is done automatically.
	// The rest of the collision detection code below is for picking. And please
	// note another cool feature: The collision response animator can be
	// attached also to all other scene nodes, not only to cameras. And it can
	// be mixed with other scene node animators. In this way, collision
	// detection and response in the Irrlicht engine is really easy.
	// 
	// Now we'll take a closer look on the parameters of
	// createCollisionResponseAnimator(). The first parameter is the
	// TriangleSelector, which specifies how the world, against collision
	// detection is done looks like. The second parameter is the scene node,
	// which is the object, which is affected by collision detection, in our
	// case it is the camera. The third defines how big the object is, it is
	// the radius of an ellipsoid. Try it out and change the radius to smaller
	// values, the camera will be able to move closer to walls after this. The
	// next parameter is the direction and speed of gravity.  We'll set it to
	// (0, -10, 0), which approximates to realistic gravity, assuming that our
	// units are metres. You could set it to (0,0,0) to disable gravity. And the
	// last value is just a translation: Without this, the ellipsoid with which
	// collision detection is done would be around the camera, and the camera would
	// be in the middle of the ellipsoid. But as human beings, we are used to have our
	// eyes on top of the body, with which we collide with our world, not in
	// the middle of it. So we place the scene node 50 units over the center
	// of the ellipsoid with this parameter. And that's it, collision
	// detection works now.

	// Set a jump speed of 3 units per second, which gives a fairly realistic jump
	// when used with the gravity of (0, -10, 0) in the collision response animator.
	var camera = smgr.addCameraSceneNodeFPS(0, 100.0, 0.3, ID_IsNotPickable, 0, 0, true, 3.0);
	camera.setPosition(new irr.vector3df(50,50,-60));
	camera.setTarget(new irr.vector3df(-70,30,-60));

	if(selector)
	{
		var anim = smgr.createCollisionResponseAnimator(selector, camera, new irr.vector3df(30,50,30),	new irr.vector3df(0,-10,0), new irr.vector3df(0,30,0));
		selector.drop(); // As soon as we're done with the selector, drop it.
		camera.addAnimator(anim);
		anim.drop();  // And likewise, drop the animator when we're done referring to it.
	}

	// Now I create three animated characters which we can pick, a dynamic light for
	// lighting them, and a billboard for drawing where we found an intersection.

	// First, let's get rid of the mouse cursor.  We'll use a billboard to show
	// what we're looking at.
	device.getCursorControl().setVisible(false);

	// Add the billboard.
	var bill = smgr.addBillboardSceneNode();
	bill.setMaterialType(irr.EMT_TRANSPARENT_ADD_COLOR );
	bill.setMaterialTexture(0, driver.getTexture(resourcePath + "particle.bmp"));
	bill.setMaterialFlag(irr.EMF_LIGHTING, false);
	bill.setMaterialFlag(irr.EMF_ZBUFFER, false);
	bill.setSize(new irr.dimension2d_f32(20.0, 20.0));
	bill.setID(ID_IsNotPickable); // This ensures that we don't accidentally ray-pick it

	// Add 3 animated hominids, which we can pick using a ray-triangle intersection.
	// They all animate quite slowly, to make it easier to see that accurate triangle
	// selection is being performed.
	var node = null;

	// Add an MD2 node, which uses vertex-based animation.
	node = smgr.addAnimatedMeshSceneNode(smgr.getMesh(resourcePath + "faerie.md2"),	0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	node.setPosition(new irr.vector3df(-70,-15,-120)); // Put its feet on the floor.
	node.setScale(new irr.vector3df(2, 2, 2)); // Make it appear realistically scaled
	node.setMD2Animation(irr.EMAT_POINT);
	node.setAnimationSpeed(20.0);
	var material = new irr.SMaterial();
	material.setTexture(0, driver.getTexture(resourcePath + "/faerie2.bmp"));
	material.Lighting = true;
	material.NormalizeNormals = true;
//	node.getMaterial(0) = material;
	node.getMaterial(0).setTexture(0, driver.getTexture(resourcePath + "/faerie2.bmp"));
	node.getMaterial(0).Lighting = true;
	node.getMaterial(0).NormalizeNormals = true;

	// Now create a triangle selector for it.  The selector will know that it
	// is associated with an animated node, and will update itself as necessary.
	selector = smgr.createTriangleSelector(node);
	node.setTriangleSelector(selector);
	selector.drop(); // We're done with this selector, so drop it now.

	// This X files uses skeletal animation, but without skinning.
	node = smgr.addAnimatedMeshSceneNode(smgr.getMesh(resourcePath + "dwarf.x"), 0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	node.setPosition(new irr.vector3df(-70,-66,0)); // Put its feet on the floor.
	node.setRotation(new irr.vector3df(0,-90,0)); // And turn it towards the camera.
	node.setAnimationSpeed(20.0);
	selector = smgr.createTriangleSelector(node);
	node.setTriangleSelector(selector);
	selector.drop();

	// And this B3D file uses skinned skeletal animation.
	node = smgr.addAnimatedMeshSceneNode(smgr.getMesh(resourcePath + "ninja.b3d"), 0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	node.setScale(new irr.vector3df(10, 10, 10));
	node.setPosition(new irr.vector3df(-70,-66,-60));
	node.setRotation(new irr.vector3df(0,90,0));
	node.setAnimationSpeed(10.0);
	node.getMaterial(0).NormalizeNormals = true;
	// Just do the same as we did above.
	selector = smgr.createTriangleSelector(node);
	node.setTriangleSelector(selector);
	selector.drop();

	material.setTexture(0, 0);
	material.Lighting = false;

	// Add a light, so that the unselected nodes aren't completely dark.
	var light = smgr.addLightSceneNode(0, new irr.vector3df(-60,100,400), new irr.SColorf(1.0,1.0,1.0,1.0), 600.0);
	light.setID(ID_IsNotPickable); // Make it an invalid target for selection.

	// Remember which scene node is highlighted
	var highlightedSceneNode = null;
	var collMan = smgr.getSceneCollisionManager();
	var lastFPS = -1;

	// draw the selection triangle only as wireframe
	material.Wireframe=true;

	while(device.run())
	if (device.isWindowActive())
	{
		driver.beginScene(true, true, 0);
		smgr.drawAll();

		// Unlight any currently highlighted scene node
		if (highlightedSceneNode)
		{
			highlightedSceneNode.setMaterialFlag(irr.EMF_LIGHTING, true);
			highlightedSceneNode = 0;
		}

		// All intersections in this example are done with a ray cast out from the camera to
		// a distance of 1000.  You can easily modify this to check (e.g.) a bullet
		// trajectory or a sword's position, or create a ray from a mouse click position using
		// ISceneCollisionManager::getRayFromScreenCoordinates()

		//ray.end = ray.start + (camera.getTarget() - ray.start).normalize() * 1000.0;
		var s = camera.getPosition();
		var e = s._opAdd(camera.getTarget()._opSub(s).normalize()._opMul(1000.0));
		var ray = new irr.line3d_f32(s.X, s.Y, s.Z, e.X, e.Y, e.Z);

		// Tracks the current intersection point with the level or a mesh
		var intersection = new irr.vector3df();
		// Used to show with triangle has been hit
		var hitTriangle = new irr.triangle3df();

		// This call is all you need to perform ray/triangle collision on every scene node
		// that has a triangle selector, including the Quake level mesh.  It finds the nearest
		// collision point/triangle, and returns the scene node containing that point.
		// Irrlicht provides other types of selection, including ray/triangle selector,
		// ray/box and ellipse/triangle selector, plus associated helpers.
		// See the methods of ISceneCollisionManager
		var selectedSceneNode = collMan.getSceneNodeAndCollisionPointFromRay(ray,
					intersection, // This will be the position of the collision
					hitTriangle, // This will be the triangle hit in the collision
					IDFlag_IsPickable, // This ensures that only nodes that we have
							// set up to be pickable are considered
					0); // Check the entire scene (this is actually the implicit default)

		// If the ray hit anything, move the billboard to the collision position
		// and draw the triangle that was hit.
		if(selectedSceneNode)
		{
			bill.setPosition(intersection);

			// We need to reset the transform before doing our own rendering.
			driver.setTransform(irr.ETS_WORLD, new irr.matrix4());
			driver.setMaterial(material);
			driver.draw3DTriangle(hitTriangle, new irr.SColor(0,255,0,0));

			// We can check the flags for the scene node that was hit to see if it should be
			// highlighted. The animated nodes can be highlighted, but not the Quake level mesh
			if((selectedSceneNode.getID() & IDFlag_IsHighlightable) == IDFlag_IsHighlightable)
			{
				highlightedSceneNode = selectedSceneNode;

				// Highlighting in this case means turning lighting OFF for this node,
				// which means that it will be drawn with full brightness.
				highlightedSceneNode.setMaterialFlag(irr.EMF_LIGHTING, false);
			}
		}

		// We're all done drawing, so end the scene.
		driver.endScene();

		var fps = driver.getFPS();

		if(lastFPS != fps)
		{
			var str = "Collision detection example - cpgf Irrlicht JavaScript Binding [";
			str = str + driver.getName();
			str = str + "] FPS:";
			str = str + fps;

			device.setWindowCaption(str);
			lastFPS = fps;
		}

	}
	
	device.drop();

	return 0;
}

start();
