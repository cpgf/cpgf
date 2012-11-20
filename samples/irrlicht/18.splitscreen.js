var ResX=800;
var ResY=600;
var fullScreen=false;

//Use SplitScreen?
var SplitScreen=true;

/*
Now we need four pointers to our cameras which are created later:
*/
//cameras
var camera= [ null, null, null, null ];
/*
In our event-receiver we switch the SplitScreen-variable,
whenever the user press the S-key. All other events are sent
to the FPS camera.
*/

function overrideEventReceiver(receiver)
{
	receiver.OnEvent = function(me, event) {
		//Key S enables/disables SplitScreen
		if (event.EventType == irr.EET_KEY_INPUT_EVENT &&
			event.KeyInput.Key == irr.KEY_KEY_S && event.KeyInput.PressedDown)
		{
			SplitScreen = !SplitScreen;
			return true;
		}
		//Send all other events to camera4
		if (camera[3])
			return camera[3].OnEvent(event);
		return false;
	}
}

/*
Ok, now the main-function:
First, we initialize the device, get the SourceManager and
VideoDriver, load an animated mesh from .md2 and a map from
.pk3. Because that's old stuff, I won't explain every step.
Just take care of the maps position.
*/
function start()
{
	// ask user for driver
	var driverType=irr.driverChoiceConsole();
	if (driverType==irr.EDT_COUNT)
		return 1;

	//Instance of the EventReceiver
	var receiver = new irr.IEventReceiverWrapper();
	overrideEventReceiver(receiver);

	//Initialise the engine
	var device = irr.createDevice(driverType, new irr.dimension2du(ResX,ResY), 32, fullScreen, false, false, receiver);
	if (!device)
		return 1;

	var smgr = device.getSceneManager();
	var driver = device.getVideoDriver();

	//Load model
	var model = smgr.getMesh("../../media/sydney.md2");
	if (!model)
		return 1;
	var model_node = smgr.addAnimatedMeshSceneNode(model);
	//Load texture
	if (model_node)
	{
		var texture = driver.getTexture("../../media/sydney.bmp");
		model_node.setMaterialTexture(0,texture);
		model_node.setMD2Animation(irr.EMAT_RUN);
		//Disable lighting (we've got no light)
		model_node.setMaterialFlag(irr.EMF_LIGHTING,false);
	}

	//Load map
	device.getFileSystem().addZipFileArchive("../../media/map-20kdm2.pk3");
	var map = smgr.getMesh("20kdm2.bsp");
	if (map)
	{
		var map_node = smgr.addOctreeSceneNode(map.getMesh(0));
		//Set position
		map_node.setPosition(new irr.vector3df(-850,-220,-850));
	}

/*
Now we create our four cameras. One is looking at the model
from the front, one from the top and one from the side. In
addition there's a FPS-camera which can be controlled by the
user.
*/
	// Create 3 fixed and one user-controlled cameras
	//Front
	camera[0] = smgr.addCameraSceneNode(0, new irr.vector3df(50,0,0), new irr.vector3df(0,0,0));
	//Top
	camera[1] = smgr.addCameraSceneNode(0, new irr.vector3df(0,50,0), new irr.vector3df(0,0,0));
	//Left
	camera[2] = smgr.addCameraSceneNode(0, new irr.vector3df(0,0,50), new irr.vector3df(0,0,0));
	//User-controlled
	camera[3] = smgr.addCameraSceneNodeFPS();
	// don't start at sydney's position
	if (camera[3])
		camera[3].setPosition(new irr.vector3df(-50,0,-50));

/*
Create a variable for counting the fps and hide the mouse:
*/
	//Hide mouse
	device.getCursorControl().setVisible(false);
	//We want to count the fps
	var lastFPS = -1;

/*
There wasn't much new stuff - till now!
Only by defining four cameras, the game won't be splitscreen.
To do this you need several steps:
  - Set the viewport to the whole screen
  - Begin a new scene (Clear screen)

  - The following 3 steps are repeated for every viewport in the splitscreen
    - Set the viewport to the area you wish
    - Activate the camera which should be "linked" with the viewport
    - Render all objects

  - If you have a GUI:
    - Set the viewport the whole screen
    - Display the GUI
  - End scene

Sounds a little complicated, but you'll see it isn't:
*/

	while(device.run())
	{
		//Set the viewpoint to the whole screen and begin scene
		driver.setViewPort(new irr.rect_s32(0,0,ResX,ResY));
		driver.beginScene(true,true,new irr.SColor(255,100,100,100));
		//If SplitScreen is used
		if (SplitScreen)
		{
			//Activate camera1
			smgr.setActiveCamera(camera[0]);
			//Set viewpoint to the first quarter (left top)
			driver.setViewPort(new irr.rect_s32(0,0,ResX/2,ResY/2));
			//Draw scene
			smgr.drawAll();
			//Activate camera2
			smgr.setActiveCamera(camera[1]);
			//Set viewpoint to the second quarter (right top)
			driver.setViewPort(new irr.rect_s32(ResX/2,0,ResX,ResY/2));
			//Draw scene
			smgr.drawAll();
			//Activate camera3
			smgr.setActiveCamera(camera[2]);
			//Set viewpoint to the third quarter (left bottom)
			driver.setViewPort(new irr.rect_s32(0,ResY/2,ResX/2,ResY));
			//Draw scene
			smgr.drawAll();
			//Set viewport the last quarter (right bottom)
			driver.setViewPort(new irr.rect_s32(ResX/2,ResY/2,ResX,ResY));
		}
		//Activate camera4
		smgr.setActiveCamera(camera[3]);
		//Draw scene
		smgr.drawAll();
		driver.endScene();

		/*
		As you can probably see, the image is rendered for every
		viewport seperately. That means, that you'll loose much performance.
		Ok, if you're aksing "How do I have to set the viewport
		to get this or that screen?", don't panic. It's really
		easy: In the rect-function you define 4 coordinates:
		- X-coordinate of the corner left top
		- Y-coordinate of the corner left top
		- X-coordinate of the corner right bottom
		- Y-coordinate of the corner right bottom

		That means, if you want to split the screen into 2 viewports
		you would give the following coordinates:
		- 1st viewport: 0,0,ResX/2,ResY
		- 2nd viewport: ResX/2,0,ResX,ResY

		If you didn't fully understand, just play arround with the example
		to check out what happens.

		Now we just view the current fps and shut down the engine,
		when the user wants to:
		*/
		//Get and show fps
		if (driver.getFPS() != lastFPS)
		{
			lastFPS = driver.getFPS();
			var tmp = "cpgf Irrlicht JavaScript Binding SplitScreen-Example (FPS: ";
			tmp = tmp + lastFPS;
			tmp = tmp + ")";
			device.setWindowCaption(tmp);
		}
	}
	//Delete device
	device.drop();
	return 0;
}

start();
