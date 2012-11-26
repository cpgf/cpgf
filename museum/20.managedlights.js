var NO_MANAGEMENT = 0;
var LIGHTS_NEAREST_NODE = 1;
var LIGHTS_IN_ZONE = 2;

// These two variable should be in a shared object.
var RequestedMode = 0;
var SceneManager = null;
var lightManager = null;

function overrideIEventReceiver(receiver)
{
	receiver.OnEvent = function(me, event)
	{
		var handled = false;

		if (event.EventType == irr.EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown)
		{
			handled = true;
			switch(event.KeyInput.Key)
			{
			case irr.KEY_KEY_1:
				RequestedMode = NO_MANAGEMENT;
				break;
			case irr.KEY_KEY_2:
				RequestedMode = LIGHTS_NEAREST_NODE;
				break;
			case irr.KEY_KEY_3:
				RequestedMode = LIGHTS_IN_ZONE;
				break;
			default:
				handled = false;
				break;
			}

			if(NO_MANAGEMENT == RequestedMode)
				SceneManager.setLightManager(null); // Show that it's safe to register the light manager
			else
				SceneManager.setLightManager(lightManager);
		}

		return handled;
	}

}

function overrideILightManager(lm, sManager)
{
	SceneManager = sManager;
	this.Mode = 0;

	this.SceneLightList = null;
	this.CurrentRenderPass = Irr.ESNRP_NONE;
	this.CurrentSceneNode = null;

	var self = this;

	// The input receiver interface, which just switches light management strategy

	// This is called before the first scene node is rendered.
	lm.OnPreRender = function(me, lightList)
	{
		// Update the mode; changing it here ensures that it's consistent throughout a render
		self.Mode = RequestedMode;

		// Store the light list. I am free to alter this list until the end of OnPostRender().
		self.SceneLightList = &lightList;
	}

	// Called after the last scene node is rendered.
	lm.OnPostRender = function(me)
	{
		// Since light management might be switched off in the event handler, we'll turn all
		// lights on to ensure that they are in a consistent state. You wouldn't normally have
		// to do this when using a light manager, since you'd continue to do light management
		// yourself.
		for(i = 0; i < self.SceneLightList.size(); i++) {
			SceneLightList._opArrayGet(i).setVisible(true);
		}
	}

	lm.OnRenderPassPreRender = function(me, renderPass)
	{
		// I don't have to do anything here except remember which render pass I am in.
		self.CurrentRenderPass = renderPass;
	}

	lm.OnRenderPassPostRender = function(me, renderPass)
	{
		// I only want solid nodes to be lit, so after the solid pass, turn all lights off.
		if(Irr.ESNRP_SOLID == renderPass)
		{
			for(i = 0; i < self.SceneLightList.size(); i++) {
				SceneLightList._opArrayGet(i).setVisible(false);
			}
		}
	}

	// This is called before the specified scene node is rendered
	lm.OnNodePreRender = function(me, node)
	{
		self.CurrentSceneNode = node;

		// This light manager only considers solid objects, but you are free to manipulate
		// lights during any phase, depending on your requirements.
		if (Irr.ESNRP_SOLID != self.CurrentRenderPass)
			return;

		// And in fact for this example, I only want to consider lighting for cube scene
		// nodes.  You will probably want to deal with lighting for (at least) mesh /
		// animated mesh scene nodes as well.
		if (node.getType() != Irr.ESNT_CUBE) {
			return;
		}

		if (LIGHTS_NEAREST_NODE == self.Mode)
		{
			// This is a naive implementation that prioritises every light in the scene
			// by its proximity to the node being rendered.  This produces some flickering
			// when lights orbit closer to a cube than its 'zone' lights.
			var nodePosition = new irr.vector3df(node.getAbsolutePosition());

			// Sort the light list by prioritising them based on their distance from the node
			// that's about to be rendered.

			var nodeArray = new Array();
			var distanceArray = new Array();

			for(i = 0; i < self.SceneLightList.size(); ++i)
			{
				var lightNode = self.SceneLightList._opArrayGet(i);
				var distance = lightNode.getAbsolutePosition().getDistanceFromSQ(nodePosition);
				nodeArray[i] = lightNode;
				distanceArray = distance;
			}

//			sortingArray.sort();

			// The list is now sorted by proximity to the node.
			// Turn on the three nearest lights, and turn the others off.
			for(i = 0; i < self.SceneLightList.size(); ++i) {
				nodeArray[i].node.setVisible(i < 3);
			}

		}
		else if(LIGHTS_IN_ZONE == Mode)
		{
			// Empty scene nodes are used to represent 'zones'.  For each solid mesh that
			// is being rendered, turn off all lights, then find its 'zone' parent, and turn
			// on all lights that are found under that node in the scene graph.
			// This is a general purpose algorithm that doesn't use any special
			// knowledge of how this particular scene graph is organised.
			for (var i = 0; i < self.SceneLightList.size(); ++i)
			{
				var lightNode = self.SceneLightList._opArrayGet(i);
				var lightData = lightNode.getLightData();

				if(Irr.ELT_DIRECTIONAL != lightData.Type) {
					lightNode.setVisible(false);
				}
			}

			var parentZone = self.findZone(node);
			if (parentZone) {
				self.turnOnZoneLights(parentZone);
			}
		}
	}

	// Called after the specified scene node is rendered
	lm.OnNodePostRender = function(me, node)
	{
		// I don't need to do any light management after individual node rendering.
	}

	// Find the empty scene node that is the parent of the specified node
	this.findZone = function(node)
	{
		if(node == null) {
			return 0;
		}

		if(node.getType() == irr.ESNT_EMPTY) {
			return node;
		}

		return self.findZone(node.getParent());
	}

	// Turn on all lights that are children (directly or indirectly) of the
	// specified scene node.
	this.turnOnZoneLights = function(node)
	{
		var children = node.getChildren();
		for (core::list<scene::ISceneNode*>::ConstIterator child = children.begin();
			child != children.end();
			++child)
		{
			if((*child).getType() == scene::ESNT_LIGHT)
				static_cast<scene::ILightSceneNode*>(*child).setVisible(true);
			else // Assume that lights don't have any children that are also lights
				turnOnZoneLights(*child);
		}
	}


	// A utility class to aid in sorting scene nodes into a distance order
	class LightDistanceElement
	{
	public:
		LightDistanceElement() {};

		LightDistanceElement(scene::ILightSceneNode* n, f64 d)
			: node(n), distance(d) { }

		scene::ILightSceneNode* node;
		f64 distance;

		// Lower distance elements are sorted to the start of the array
		bool operator < (const LightDistanceElement& other) const
		{
			return (distance < other.distance);
		}
	};
}


/*
*/
int main(int argumentCount, char * argumentValues[])
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;

	IrrlichtDevice *device = createDevice(driverType,
			dimension2d<u32>(640, 480), 32);

	if(!device)
		return -1;

	f32 const lightRadius = 60.f; // Enough to reach the far side of each 'zone'

	video::IVideoDriver* driver = device.getVideoDriver();
	scene::ISceneManager* smgr = device.getSceneManager();
	gui::IGUIEnvironment* guienv = device.getGUIEnvironment();

	gui::IGUISkin* skin = guienv.getSkin();
	if (skin)
	{
		skin.setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255, 255, 255, 255));
		gui::IGUIFont* font = guienv.getFont("../../media/fontlucida.png");
		if(font)
			skin.setFont(font);
	}

	guienv.addStaticText(L"1 - No light management", core::rect<s32>(10,10,200,30));
	guienv.addStaticText(L"2 - Closest 3 lights", core::rect<s32>(10,30,200,50));
	guienv.addStaticText(L"3 - Lights in zone", core::rect<s32>(10,50,200,70));

/*
Add several "zones".  You could use this technique to light individual rooms, for example.
*/
	for(f32 zoneX = -100.f; zoneX <= 100.f; zoneX += 50.f)
		for(f32 zoneY = -60.f; zoneY <= 60.f; zoneY += 60.f)
		{
			// Start with an empty scene node, which we will use to represent a zone.
			scene::ISceneNode * zoneRoot = smgr.addEmptySceneNode();
			zoneRoot.setPosition(vector3df(zoneX, zoneY, 0));

			// Each zone contains a rotating cube
			scene::IMeshSceneNode * node = smgr.addCubeSceneNode(15, zoneRoot);
			scene::ISceneNodeAnimator * rotation = smgr.createRotationAnimator(vector3df(0.25f, 0.5f, 0.75f));
			node.addAnimator(rotation);
			rotation.drop();

			// And each cube has three lights attached to it.  The lights are attached to billboards so
			// that we can see where they are.  The billboards are attached to the cube, so that the
			// lights are indirect descendents of the same empty scene node as the cube.
			scene::IBillboardSceneNode * billboard = smgr.addBillboardSceneNode(node);
			billboard.setPosition(vector3df(0, -14, 30));
			billboard.setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
			billboard.setMaterialTexture(0, driver.getTexture("../../media/particle.bmp"));
			billboard.setMaterialFlag(video::EMF_LIGHTING, false);
			scene::ILightSceneNode * light = smgr.addLightSceneNode(billboard, vector3df(0, 0, 0), video::SColorf(1, 0, 0), lightRadius);

			billboard = smgr.addBillboardSceneNode(node);
			billboard.setPosition(vector3df(-21, -14, -21));
			billboard.setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
			billboard.setMaterialTexture(0, driver.getTexture("../../media/particle.bmp"));
			billboard.setMaterialFlag(video::EMF_LIGHTING, false);
			light = smgr.addLightSceneNode(billboard, vector3df(0, 0, 0), video::SColorf(0, 1, 0), lightRadius);

			billboard = smgr.addBillboardSceneNode(node);
			billboard.setPosition(vector3df(21, -14, -21));
			billboard.setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
			billboard.setMaterialTexture(0, driver.getTexture("../../media/particle.bmp"));
			billboard.setMaterialFlag(video::EMF_LIGHTING, false);
			light = smgr.addLightSceneNode(billboard, vector3df(0, 0, 0), video::SColorf(0, 0, 1), lightRadius);

			// Each cube also has a smaller cube rotating around it, to show that the cubes are being
			// lit by the lights in their 'zone', not just lights that are their direct children.
			node = smgr.addCubeSceneNode(5, node);
			node.setPosition(vector3df(0, 21, 0));
		}

	smgr.addCameraSceneNode(0, vector3df(0,0,-130), vector3df(0,0,0));

	CMyLightManager * myLightManager = new CMyLightManager(smgr);
	smgr.setLightManager(0); // This is the default: we won't do light management until told to do it.
	device.setEventReceiver(myLightManager);

	int lastFps = -1;

	while(device.run())
	{
		driver.beginScene(true, true, video::SColor(255,100,101,140));
		smgr.drawAll();
		guienv.drawAll();
		driver.endScene();

		int fps = driver.getFPS();
		if(fps != lastFps)
		{
			lastFps = fps;
			core::stringw str = L"Managed Lights [";
			str += driver.getName();
			str += "] FPS:";
			str += fps;
			device.setWindowCaption(str.c_str());
		}
	}

	myLightManager.drop(); // Drop my implicit reference
	device.drop();
	return 0;
}

/*
**/

