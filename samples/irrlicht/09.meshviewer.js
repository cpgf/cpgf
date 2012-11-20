cpgf.import(null, "builtin.core");
cpgf.import(null, "builtin.debug");

var Device = null;
var StartUpModelFile = "";
var MessageText = "";
var Caption = "";
var Model = null;
var SkyBox = null;
var Octree=false;
var UseLight=false;

var Camera = [ null, null ];

// Values used to identify individual GUI elements
var GUI_ID_DIALOG_ROOT_WINDOW  = 0x10000;

var GUI_ID_X_SCALE = 0x10001;
var GUI_ID_Y_SCALE = 0x10002;
var GUI_ID_Z_SCALE = 0x10003;

var GUI_ID_OPEN_MODEL = 0x10004;
var GUI_ID_SET_MODEL_ARCHIVE = 0x10005;
var GUI_ID_LOAD_AS_OCTREE = 0x10006;

var GUI_ID_SKY_BOX_VISIBLE = 0x10007;
var GUI_ID_TOGGLE_DEBUG_INFO = 0x10008;

var GUI_ID_DEBUG_OFF = 0x10009;
var GUI_ID_DEBUG_BOUNDING_BOX = 0x1000A;
var GUI_ID_DEBUG_NORMALS = 0x1000B;
var GUI_ID_DEBUG_SKELETON = 0x1000C;
var GUI_ID_DEBUG_WIRE_OVERLAY = 0x1000D;
var GUI_ID_DEBUG_HALF_TRANSPARENT = 0x1000E;
var GUI_ID_DEBUG_BUFFERS_BOUNDING_BOXES = 0x1000F;
var GUI_ID_DEBUG_ALL = 0x10010;

var GUI_ID_MODEL_MATERIAL_SOLID = 0x10011;
var GUI_ID_MODEL_MATERIAL_TRANSPARENT = 0x10012;
var GUI_ID_MODEL_MATERIAL_REFLECTION = 0x10013;

var GUI_ID_CAMERA_MAYA = 0x10014;
var GUI_ID_CAMERA_FIRST_PERSON = 0x10015;

var GUI_ID_POSITION_TEXT = 0x10016;

var GUI_ID_ABOUT = 0x10017;
var GUI_ID_QUIT = 0x10018;

var GUI_ID_TEXTUREFILTER = 0x10019;
var GUI_ID_SKIN_TRANSPARENCY = 0x1001A;
var GUI_ID_SKIN_ANIMATION_FPS = 0x1001B;

var GUI_ID_BUTTON_SET_SCALE = 0x1001C;
var GUI_ID_BUTTON_SCALE_MUL10 = 0x1001D;
var GUI_ID_BUTTON_SCALE_DIV10 = 0x1001E;
var GUI_ID_BUTTON_OPEN_MODEL = 0x1001F;
var GUI_ID_BUTTON_SHOW_ABOUT = 0x10020;
var GUI_ID_BUTTON_SHOW_TOOLBOX = 0x10021;
var GUI_ID_BUTTON_SELECT_ARCHIVE = 0x10022;

var GUI_ID_ANIMATION_INFO = 0x10023;

// And some magic numbers
var MAX_FRAMERATE = 80;
var DEFAULT_FRAMERATE = 30;


/*
Toggle between various cameras
*/
function setActiveCamera(newActive)
{
	if (null == Device)
		return;

	var active = Device.getSceneManager().getActiveCamera();
	active.setInputReceiverEnabled(false);

	newActive.setInputReceiverEnabled(true);
	Device.getSceneManager().setActiveCamera(newActive);
}

/*
	Set the skin transparency by changing the alpha values of all skin-colors
*/
function setSkinTransparency(alpha, skin)
{
	for (var i=0; i<irr.EGDC_COUNT ; ++i)
	{
		var col = skin.getColor(i);
		col.setAlpha(alpha);
		skin.setColor(i, col);
	}
}

/*
  Update the display of the model scaling
*/
function updateScaleInfo(model)
{
	var toolboxWnd = Device.getGUIEnvironment().getRootGUIElement().getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
	if (!toolboxWnd)
		return;
	if (!model)
	{
		toolboxWnd.getElementFromId(GUI_ID_X_SCALE, true).setText( "-" );
		toolboxWnd.getElementFromId(GUI_ID_Y_SCALE, true).setText( "-" );
		toolboxWnd.getElementFromId(GUI_ID_Z_SCALE, true).setText( "-" );
	}
	else
	{
		var scale = model.getScale();
		toolboxWnd.getElementFromId(GUI_ID_X_SCALE, true).setText( "" + scale.X );
		toolboxWnd.getElementFromId(GUI_ID_Y_SCALE, true).setText( "" + scale.Y );
		toolboxWnd.getElementFromId(GUI_ID_Z_SCALE, true).setText( "" + scale.Z );
	}
}

/*
Function showAboutText() displays a messagebox with a caption and
a message text. The texts will be stored in the MessageText and Caption
variables at startup.
*/
function showAboutText()
{
	// create modal message box with the text
	// loaded from the xml file.
	Device.getGUIEnvironment().addMessageBox(Caption, MessageText);
}


/*
Function loadModel() loads a model and displays it using an
addAnimatedMeshSceneNode and the scene manager. Nothing difficult. It also
displays a short message box, if the model could not be loaded.
*/
function loadModel(fn)
{
	// modify the name if it a .pk3 file

	var filename = fn;

	var extension = filename.substr(filename.length - 4, 4);
	extension = extension.toLowerCase();

	// if a texture is loaded apply it to the current model..
	if (extension == ".jpg" || extension == ".pcx" ||
		extension == ".png" || extension == ".ppm" ||
		extension == ".pgm" || extension == ".pbm" ||
		extension == ".psd" || extension == ".tga" ||
		extension == ".bmp" || extension == ".wa" ||
		extension == ".rgb" || extension == ".rgba")
	{
		var texture = Device.getVideoDriver().getTexture( filename );
		if ( texture && Model )
		{
			// always reload texture
			Device.getVideoDriver().removeTexture(texture);
			texture = Device.getVideoDriver().getTexture( filename );

			Model.setMaterialTexture(0, texture);
		}
		return;
	}
	// if a archive is loaded add it to the FileArchive..
	else if (extension == ".pk3" || extension == ".zip" || extension == ".pak" || extension == ".npk")
	{
		Device.getFileSystem().addFileArchive(filename);
		return;
	}

	// load a model into the engine

	if (Model)
		Model.remove();

	Model = 0;

	if (extension==".irr")
	{
//		core::array<irr.ISceneNode*> outNodes;
//		Device.getSceneManager().loadScene(filename);
//		Device.getSceneManager().getSceneNodesFromType(irr.ESNT_ANIMATED_MESH, outNodes);
//		if (outNodes.size())
//			Model = outNodes[0];
		return;
	}

	var m = Device.getSceneManager().getMesh( filename );

	if (!m)
	{
		// model could not be loaded

		if (StartUpModelFile != filename)
			Device.getGUIEnvironment().addMessageBox(Caption, "The model could not be loaded. Maybe it is not a supported file format.");
		return;
	}

	// set default material properties

	if (Octree)
		Model = Device.getSceneManager().addOctreeSceneNode(m.getMesh(0));
	else
	{
		var animModel = Device.getSceneManager().addAnimatedMeshSceneNode(m);
		animModel.setAnimationSpeed(30);
		Model = animModel;
	}
	Model.setMaterialFlag(irr.EMF_LIGHTING, UseLight);
	Model.setMaterialFlag(irr.EMF_NORMALIZE_NORMALS, UseLight);
//	Model.setMaterialFlag(irr.EMF_BACK_FACE_CULLING, false);
	Model.setDebugDataVisible(irr.EDS_OFF);

	// we need to uncheck the menu entries. would be cool to fake a menu event, but
	// that's not so simple. so we do it brute force
	var menu = cpgf.cast(Device.getGUIEnvironment().getRootGUIElement().getElementFromId(GUI_ID_TOGGLE_DEBUG_INFO, true), irr.IGUIContextMenu);
	if (menu)
		for(var item = 1; item < 6; ++item)
			menu.setItemChecked(item, false);
	updateScaleInfo(Model);
}


/*
Function createToolBox() creates a toolbox window. In this simple mesh
viewer, this toolbox only contains a tab control with three edit boxes for
changing the scale of the displayed model.
*/
function createToolBox()
{
	// remove tool box if already there
	var env = Device.getGUIEnvironment();
	var root = env.getRootGUIElement();
	var e = root.getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
	if (e)
		e.remove();

	// create the toolbox window
	var wnd = env.addWindow(new irr.rect_s32(600,45,800,480),
		false, "Toolset", 0, GUI_ID_DIALOG_ROOT_WINDOW);

	// create tab control and tabs
	var tab = env.addTabControl(new irr.rect_s32(2,20,800-602,480-7), wnd, true, true);

	var t1 = tab.addTab("Config");

	// add some edit boxes and a button to tab one
	env.addStaticText("Scale:", new irr.rect_s32(10,20,60,45), false, false, t1);
	env.addStaticText("X:", new irr.rect_s32(22,48,40,66), false, false, t1);
	env.addEditBox("1.0", new irr.rect_s32(40,46,130,66), true, t1, GUI_ID_X_SCALE);
	env.addStaticText("Y:", new irr.rect_s32(22,82,40,96), false, false, t1);
	env.addEditBox("1.0", new irr.rect_s32(40,76,130,96), true, t1, GUI_ID_Y_SCALE);
	env.addStaticText("Z:", new irr.rect_s32(22,108,40,126), false, false, t1);
	env.addEditBox("1.0", new irr.rect_s32(40,106,130,126), true, t1, GUI_ID_Z_SCALE);

	env.addButton(new irr.rect_s32(10,134,85,165), t1, GUI_ID_BUTTON_SET_SCALE, "Set");

	// quick scale buttons
	env.addButton(new irr.rect_s32(65,20,95,40), t1, GUI_ID_BUTTON_SCALE_MUL10, "* 10");
	env.addButton(new irr.rect_s32(100,20,130,40), t1, GUI_ID_BUTTON_SCALE_DIV10, "* 0.1");

	updateScaleInfo(Model);

	// add transparency control
	env.addStaticText("GUI Transparency Control:", new irr.rect_s32(10,200,150,225), true, false, t1);
	var scrollbar = env.addScrollBar(true, new irr.rect_s32(10,225,150,240), t1, GUI_ID_SKIN_TRANSPARENCY);
	scrollbar.setMax(255);
	scrollbar.setPos(255);

	// add framerate control
	env.addStaticText(":", new irr.rect_s32(10,240,150,265), true, false, t1);
	env.addStaticText("Framerate:", new irr.rect_s32(12,240,75,265), false, false, t1);
	env.addStaticText("", new irr.rect_s32(75,240,200,265), false, false, t1, GUI_ID_ANIMATION_INFO);
	scrollbar = env.addScrollBar(true, new irr.rect_s32(10,265,150,280), t1, GUI_ID_SKIN_ANIMATION_FPS);
	scrollbar.setMax(MAX_FRAMERATE);
	scrollbar.setMin(-MAX_FRAMERATE);
	scrollbar.setPos(DEFAULT_FRAMERATE);
	scrollbar.setSmallStep(1);
}

/*
Function updateToolBox() is called each frame to update dynamic information in 
the toolbox.
*/
function updateToolBox()
{
	var env = Device.getGUIEnvironment();
	var root = env.getRootGUIElement();
	var dlg = root.getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
	if (!dlg )
		return;

	// update the info we have about the animation of the model
	var  aniInfo = cpgf.cast(dlg.getElementFromId(GUI_ID_ANIMATION_INFO, true), irr.IGUIStaticText);
	if (aniInfo)
	{
		if ( Model && irr.ESNT_ANIMATED_MESH == Model.getType() )
		{
			var animatedModel = cpgf.cast(Model, irr.IAnimatedMeshSceneNode);

			var str = "" + animatedModel.getAnimationSpeed();
			str = str + " Frame: ";
			str = str + animatedModel.getFrameNr();
			aniInfo.setText(str);
		}
		else
			aniInfo.setText("");
	}
}

/*
Function hasModalDialog() checks if we currently have a modal dialog open.
*/
function hasModalDialog()
{
	var env = Device.getGUIEnvironment();
	var focused = env.getFocus();
	while ( focused )
	{
		if ( focused.isVisible() && focused.hasType(irr.EGUIET_MODAL_SCREEN) )
			return true;
		focused = focused.getParent();
	}
	return false;
}

/*
To get all the events sent by the GUI Elements, we need to create an event
receiver. This one is really simple. If an event occurs, it checks the id of
the caller and the event type, and starts an action based on these values. For
example, if a menu item with id GUI_ID_OPEN_MODEL was selected, it opens a file-open-dialog.
*/
function overrideEventReceiver(receiver)
{
	/*
		Handle key-up events
	*/
	this.OnKeyUp = function(keyCode)
	{
		// Don't handle keys if we have a modal dialog open as it would lead 
		// to unexpected application behaviour for the user.
		if ( hasModalDialog() )
			return false;

		if (keyCode == irr.KEY_ESCAPE)
		{
			if (Device)
			{
				var camera = Device.getSceneManager().getActiveCamera();
				if (camera)
				{
					camera.setInputReceiverEnabled( !camera.isInputReceiverEnabled() );
				}
				return true;
			}
		}
		else if (keyCode == irr.KEY_F1)
		{
			if (Device)
			{
				var elem = Device.getGUIEnvironment().getRootGUIElement().getElementFromId(GUI_ID_POSITION_TEXT);
				if (elem)
					elem.setVisible(!elem.isVisible());
			}
		}
		else if (keyCode == irr.KEY_KEY_M)
		{
			if (Device)
				Device.minimizeWindow();
		}
		else if (keyCode == irr.KEY_KEY_L)
		{
			UseLight=!UseLight;
			if (Model)
			{
				Model.setMaterialFlag(irr.EMF_LIGHTING, UseLight);
				Model.setMaterialFlag(irr.EMF_NORMALIZE_NORMALS, UseLight);
			}
		}
		return false;
	}


	/*
		Handle "menu item clicked" events.
	*/
	this.OnMenuItemSelected = function(menu)
	{
		var id = menu.getItemCommandId(menu.getSelectedItem());
		var env = Device.getGUIEnvironment();

		switch(id)
		{
		case GUI_ID_OPEN_MODEL: // FilOnButtonSetScalinge . Open Model
			env.addFileOpenDialog("Please select a model file to open");
			break;
		case GUI_ID_SET_MODEL_ARCHIVE: // File . Set Model Archive
			env.addFileOpenDialog("Please select your game archive/directory");
			break;
		case GUI_ID_LOAD_AS_OCTREE: // File . LoadAsOctree
			Octree = !Octree;
			menu.setItemChecked(menu.getSelectedItem(), Octree);
			break;
		case GUI_ID_QUIT: // File . Quit
			Device.closeDevice();
			break;
		case GUI_ID_SKY_BOX_VISIBLE: // View . Skybox
			menu.setItemChecked(menu.getSelectedItem(), !menu.isItemChecked(menu.getSelectedItem()));
			SkyBox.setVisible(!SkyBox.isVisible());
			break;
		case GUI_ID_DEBUG_OFF: // View . Debug Information
			menu.setItemChecked(menu.getSelectedItem()+1, false);
			menu.setItemChecked(menu.getSelectedItem()+2, false);
			menu.setItemChecked(menu.getSelectedItem()+3, false);
			menu.setItemChecked(menu.getSelectedItem()+4, false);
			menu.setItemChecked(menu.getSelectedItem()+5, false);
			menu.setItemChecked(menu.getSelectedItem()+6, false);
			if (Model)
				Model.setDebugDataVisible(irr.EDS_OFF);
			break;
		case GUI_ID_DEBUG_BOUNDING_BOX: // View . Debug Information
			menu.setItemChecked(menu.getSelectedItem(), !menu.isItemChecked(menu.getSelectedItem()));
			if (Model)
				Model.setDebugDataVisible(Model.isDebugDataVisible()^irr.EDS_BBOX);
			break;
		case GUI_ID_DEBUG_NORMALS: // View . Debug Information
			menu.setItemChecked(menu.getSelectedItem(), !menu.isItemChecked(menu.getSelectedItem()));
			if (Model)
				Model.setDebugDataVisible(Model.isDebugDataVisible()^irr.EDS_NORMALS);
			break;
		case GUI_ID_DEBUG_SKELETON: // View . Debug Information
			menu.setItemChecked(menu.getSelectedItem(), !menu.isItemChecked(menu.getSelectedItem()));
			if (Model)
				Model.setDebugDataVisible(Model.isDebugDataVisible()^irr.EDS_SKELETON);
			break;
		case GUI_ID_DEBUG_WIRE_OVERLAY: // View . Debug Information
			menu.setItemChecked(menu.getSelectedItem(), !menu.isItemChecked(menu.getSelectedItem()));
			if (Model)
				Model.setDebugDataVisible(Model.isDebugDataVisible()^irr.EDS_MESH_WIRE_OVERLAY);
			break;
		case GUI_ID_DEBUG_HALF_TRANSPARENT: // View . Debug Information
			menu.setItemChecked(menu.getSelectedItem(), !menu.isItemChecked(menu.getSelectedItem()));
			if (Model)
				Model.setDebugDataVisible(Model.isDebugDataVisible()^irr.EDS_HALF_TRANSPARENCY);
			break;
		case GUI_ID_DEBUG_BUFFERS_BOUNDING_BOXES: // View . Debug Information
			menu.setItemChecked(menu.getSelectedItem(), !menu.isItemChecked(menu.getSelectedItem()));
			if (Model)
				Model.setDebugDataVisible(Model.isDebugDataVisible()^irr.EDS_BBOX_BUFFERS);
			break;
		case GUI_ID_DEBUG_ALL: // View . Debug Information
			menu.setItemChecked(menu.getSelectedItem()-1, true);
			menu.setItemChecked(menu.getSelectedItem()-2, true);
			menu.setItemChecked(menu.getSelectedItem()-3, true);
			menu.setItemChecked(menu.getSelectedItem()-4, true);
			menu.setItemChecked(menu.getSelectedItem()-5, true);
			menu.setItemChecked(menu.getSelectedItem()-6, true);
			if (Model)
				Model.setDebugDataVisible(irr.EDS_FULL);
			break;
		case GUI_ID_ABOUT: // Help.About
			showAboutText();
			break;
		case GUI_ID_MODEL_MATERIAL_SOLID: // View . Material . Solid
			if (Model)
				Model.setMaterialType(irr.EMT_SOLID);
			break;
		case GUI_ID_MODEL_MATERIAL_TRANSPARENT: // View . Material . Transparent
			if (Model)
				Model.setMaterialType(irr.EMT_TRANSPARENT_ADD_COLOR);
			break;
		case GUI_ID_MODEL_MATERIAL_REFLECTION: // View . Material . Reflection
			if (Model)
				Model.setMaterialType(irr.EMT_SPHERE_MAP);
			break;

		case GUI_ID_CAMERA_MAYA:
			setActiveCamera(Camera[0]);
			break;
		case GUI_ID_CAMERA_FIRST_PERSON:
			setActiveCamera(Camera[1]);
			break;
		}
	}

	/*
		Handle the event that one of the texture-filters was selected in the corresponding combobox.
	*/
	this.OnTextureFilterSelected = function(combo)
	{
		var pos = combo.getSelected();
		switch (pos)
		{
			case 0:
			if (Model)
			{
				Model.setMaterialFlag(irr.EMF_BILINEAR_FILTER, false);
				Model.setMaterialFlag(irr.EMF_TRILINEAR_FILTER, false);
				Model.setMaterialFlag(irr.EMF_ANISOTROPIC_FILTER, false);
			}
			break;
			case 1:
			if (Model)
			{
				Model.setMaterialFlag(irr.EMF_BILINEAR_FILTER, true);
				Model.setMaterialFlag(irr.EMF_TRILINEAR_FILTER, false);
			}
			break;
			case 2:
			if (Model)
			{
				Model.setMaterialFlag(irr.EMF_BILINEAR_FILTER, false);
				Model.setMaterialFlag(irr.EMF_TRILINEAR_FILTER, true);
			}
			break;
			case 3:
			if (Model)
			{
				Model.setMaterialFlag(irr.EMF_ANISOTROPIC_FILTER, true);
			}
			break;
			case 4:
			if (Model)
			{
				Model.setMaterialFlag(irr.EMF_ANISOTROPIC_FILTER, false);
			}
			break;
		}
	}

	var self = this;
	receiver.OnEvent = function(me, event)
	{
		// Escape swaps Camera Input
		if (event.EventType == irr.EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown == false)
		{
			if ( self.OnKeyUp(event.KeyInput.Key) )
				return true;
		}

		if (event.EventType == irr.EET_GUI_EVENT)
		{
			var id = event.GUIEvent.Caller.getID();
			var env = Device.getGUIEnvironment();

			switch(event.GUIEvent.EventType)
			{
			case irr.EGET_MENU_ITEM_SELECTED:
					// a menu item was clicked
					self.OnMenuItemSelected( cpgf.cast(event.GUIEvent.Caller, irr.IGUIContextMenu) );
				break;

			case irr.EGET_FILE_SELECTED:
				{
					// load the model file, selected in the file open dialog
					var dialog = cpgf.cast(event.GUIEvent.Caller, irr.IGUIFileOpenDialog);
					loadModel(dialog.getFileName());
				}
				break;

			case irr.EGET_SCROLL_BAR_CHANGED:

				// control skin transparency
				if (id == GUI_ID_SKIN_TRANSPARENCY)
				{
					var pos = cpgf.cast(event.GUIEvent.Caller, irr.IGUIScrollBar).getPos();
					setSkinTransparency(pos, env.getSkin());
				}
				// control animation speed
				else if (id == GUI_ID_SKIN_ANIMATION_FPS)
				{
					var pos = cpgf.cast(event.GUIEvent.Caller, irr.IGUIScrollBar).getPos();
					if (Model && irr.ESNT_ANIMATED_MESH == Model.getType())
						cpgf.cast(Model, irr.IAnimatedMeshSceneNode).setAnimationSpeed(pos);
				}
				break;

			case irr.EGET_COMBO_BOX_CHANGED:

				// control anti-aliasing/filtering
				if (id == GUI_ID_TEXTUREFILTER)
				{
					self.OnTextureFilterSelected( cpgf.cast(event.GUIEvent.Caller, irr.IGUIComboBox) );
				}
				break;

			case irr.EGET_BUTTON_CLICKED:
				switch(id)
				{
				case GUI_ID_BUTTON_SET_SCALE:
					{
						// set scale
						var root = env.getRootGUIElement();
						var scale = new irr.vector3df();
						var s;

						s = root.getElementFromId(GUI_ID_X_SCALE, true).getText();
						scale.X = parseFloat(s);
						s = root.getElementFromId(GUI_ID_Y_SCALE, true).getText();
						scale.Y = parseFloat(s);
						s = root.getElementFromId(GUI_ID_Z_SCALE, true).getText();
						scale.Z = parseFloat(s);

						if (Model)
							Model.setScale(scale);
						updateScaleInfo(Model);
					}
					break;
				case GUI_ID_BUTTON_SCALE_MUL10:
					if (Model) {
						var scale = new irr.vector3df(Model.getScale());
						scale.X = scale.X * 10;
						scale.Y = scale.Y * 10;
						scale.Z = scale.Z * 10;
						Model.setScale(scale);
					}
					updateScaleInfo(Model);
					break;
				case GUI_ID_BUTTON_SCALE_DIV10:
					if (Model) {
						var scale = new irr.vector3df(Model.getScale());
						scale.X = scale.X * 0.1;
						scale.Y = scale.Y * 0.1;
						scale.Z = scale.Z * 0.1;
						Model.setScale(scale);
					}
					updateScaleInfo(Model);
					break;
				case GUI_ID_BUTTON_OPEN_MODEL:
					env.addFileOpenDialog("Please select a model file to open");
					break;
				case GUI_ID_BUTTON_SHOW_ABOUT:
					showAboutText();
					break;
				case GUI_ID_BUTTON_SHOW_TOOLBOX:
					createToolBox();
					break;
				case GUI_ID_BUTTON_SELECT_ARCHIVE:
					env.addFileOpenDialog("Please select your game archive/directory");
					break;
				}

				break;
			default:
				break;
			}
		}

		return false;
	}


}


/*
Most of the hard work is done. We only need to create the Irrlicht Engine
device and all the buttons, menus and toolbars. We start up the engine as
usual, using createDevice(). To make our application catch events, we set our
eventreceiver as parameter. As you can see, there is also a call to
IrrlichtDevice::setResizeable(). This makes the render window resizeable, which
is quite useful for a mesh viewer.
*/
function start()
{
	// ask user for driver
	var driverType=irr.driverChoiceConsole();
	if (driverType==irr.EDT_COUNT)
		return 1;

	// create device and exit if creation failed
	var receiver = new irr.IEventReceiverWrapper();
	overrideEventReceiver(receiver);
	Device = irr.createDevice(driverType, new irr.dimension2d_u32(800, 600), 16, false, false, false, receiver);

	if (Device == 0)
		return 1; // could not create selected driver.

	Device.setResizable(true);

	Device.setWindowCaption("cpgf Irrlicht JavaScript Binding - Loading...");

	var driver = Device.getVideoDriver();
	var env = Device.getGUIEnvironment();
	var smgr = Device.getSceneManager();
	smgr.getParameters().setAttribute(irr.COLLADA_CREATE_SCENE_INSTANCES, true);

	driver.setTextureCreationFlag(irr.ETCF_ALWAYS_32_BIT, true);

	smgr.addLightSceneNode(0, new irr.vector3df(200,200,200), new irr.SColorf(1.0,1.0,1.0),2000);
	smgr.setAmbientLight(new irr.SColorf(0.3,0.3,0.3));
	// add our media directory as "search path"
	Device.getFileSystem().addFolderFileArchive("../../media/");

	/*
	The next step is to read the configuration file. It is stored in the xml
	format and looks a little bit like this:

	@verbatim
	<?xml version="1.0"?>
	<config>
		<startUpModel file="some filename" />
		<messageText caption="Irrlicht Engine Mesh Viewer">
			Hello!
		</messageText>
	</config>
	@endverbatim

	We need the data stored in there to be written into the global variables
	StartUpModelFile, MessageText and Caption. This is now done using the
	Irrlicht Engine integrated XML parser:
	*/

	// read configuration from xml file

	var xml = Device.getFileSystem().createXMLReader( "config.xml");

	while(xml && xml.read())
	{
		switch(xml.getNodeType())
		{
		case irr.EXN_TEXT:
			// in this xml file, the only text which occurs is the
			// messageText
			MessageText = xml.getNodeData();
			break;
		case irr.EXN_ELEMENT:
			{
				if ("startUpModel" == xml.getNodeName())
					StartUpModelFile = xml.getAttributeValue("file");
				else
				if ("messageText" == xml.getNodeName())
					Caption = xml.getAttributeValue("caption");
			}
			break;
		default:
			break;
		}
	}

	if (xml)
		xml.drop(); // don't forget to delete the xml reader

	/*
	That wasn't difficult. Now we'll set a nicer font and create the Menu.
	It is possible to create submenus for every menu item. The call
	menu.addItem("File", -1, true, true); for example adds a new menu
	Item with the name "File" and the id -1. The following parameter says
	that the menu item should be enabled, and the last one says, that there
	should be a submenu. The submenu can now be accessed with
	menu.getSubMenu(0), because the "File" entry is the menu item with
	index 0.
	*/

	// set a nicer font

	var skin = env.getSkin();
	var font = env.getFont("fonthaettenschweiler.bmp");
	if (font)
		skin.setFont(font);

	// create menu
	var menu = env.addMenu();
	menu.addItem("File", -1, true, true);
	menu.addItem("View", -1, true, true);
	menu.addItem("Camera", -1, true, true);
	menu.addItem("Help", -1, true, true);

	var submenu;
	submenu = menu.getSubMenu(0);
	submenu.addItem("Open Model File & Texture...", GUI_ID_OPEN_MODEL);
	submenu.addItem("Set Model Archive...", GUI_ID_SET_MODEL_ARCHIVE);
	submenu.addItem("Load as Octree", GUI_ID_LOAD_AS_OCTREE);
	submenu.addSeparator();
	submenu.addItem("Quit", GUI_ID_QUIT);

	submenu = menu.getSubMenu(1);
	submenu.addItem("sky box visible", GUI_ID_SKY_BOX_VISIBLE, true, false, true);
	submenu.addItem("toggle model debug information", GUI_ID_TOGGLE_DEBUG_INFO, true, true);
	submenu.addItem("model materia", -1, true, true );

	submenu = submenu.getSubMenu(1);
	submenu.addItem("Off", GUI_ID_DEBUG_OFF);
	submenu.addItem("Bounding Box", GUI_ID_DEBUG_BOUNDING_BOX);
	submenu.addItem("Normals", GUI_ID_DEBUG_NORMALS);
	submenu.addItem("Skeleton", GUI_ID_DEBUG_SKELETON);
	submenu.addItem("Wire overlay", GUI_ID_DEBUG_WIRE_OVERLAY);
	submenu.addItem("Half-Transparent", GUI_ID_DEBUG_HALF_TRANSPARENT);
	submenu.addItem("Buffers bounding boxes", GUI_ID_DEBUG_BUFFERS_BOUNDING_BOXES);
	submenu.addItem("Al", GUI_ID_DEBUG_ALL);

	submenu = menu.getSubMenu(1).getSubMenu(2);
	submenu.addItem("Solid", GUI_ID_MODEL_MATERIAL_SOLID);
	submenu.addItem("Transparent", GUI_ID_MODEL_MATERIAL_TRANSPARENT);
	submenu.addItem("Reflection", GUI_ID_MODEL_MATERIAL_REFLECTION);

	submenu = menu.getSubMenu(2);
	submenu.addItem("Maya Style", GUI_ID_CAMERA_MAYA);
	submenu.addItem("First Person", GUI_ID_CAMERA_FIRST_PERSON);

	submenu = menu.getSubMenu(3);
	submenu.addItem("About", GUI_ID_ABOUT);

	/*
	Below the menu we want a toolbar, onto which we can place colored
	buttons and important looking stuff like a senseless combobox.
	*/

	// create toolbar

	var bar = env.addToolBar();

	var image = driver.getTexture("open.png");
	bar.addButton(GUI_ID_BUTTON_OPEN_MODEL, 0, "Open a mode",image, 0, false, true);

	image = driver.getTexture("tools.png");
	bar.addButton(GUI_ID_BUTTON_SHOW_TOOLBOX, 0, "Open Toolset",image, 0, false, true);

	image = driver.getTexture("zip.png");
	bar.addButton(GUI_ID_BUTTON_SELECT_ARCHIVE, 0, "Set Model Archive",image, 0, false, true);

	image = driver.getTexture("help.png");
	bar.addButton(GUI_ID_BUTTON_SHOW_ABOUT, 0, "Open Help", image, 0, false, true);

	// create a combobox for texture filters

	var box = env.addComboBox(new irr.rect_s32(250,4,350,23), bar, GUI_ID_TEXTUREFILTER);
	box.addItem("No filtering");
	box.addItem("Bilinear");
	box.addItem("Trilinear");
	box.addItem("Anisotropic");
	box.addItem("Isotropic");

	/*
	To make the editor look a little bit better, we disable transparent gui
	elements, and add an Irrlicht Engine logo. In addition, a text showing
	the current frames per second value is created and the window caption is
	changed.
	*/

	// disable alpha

	for (var i=0; i<irr.EGDC_COUNT ; ++i)
	{
		var col = env.getSkin().getColor(i);
		col.setAlpha(255);
		env.getSkin().setColor(i, col);
	}

	// add a tabcontrol

	createToolBox();

	// create fps text

	var fpstext = env.addStaticText("",	new irr.rect_s32(400,4,570,23), true, false, bar);

	var postext = env.addStaticText("",	new irr.rect_s32(10,50,470,80),false, false, 0, GUI_ID_POSITION_TEXT);
	postext.setVisible(false);

	// set window caption

	Caption = Caption + " - [";
	Caption = Caption + driver.getName();
	Caption = Caption + "]";
	Device.setWindowCaption(Caption);

	/*
	That's nearly the whole application. We simply show the about message
	box at start up, and load the first model. To make everything look
	better, a skybox is created and a user controled camera, to make the
	application a little bit more interactive. Finally, everything is drawn
	in a standard drawing loop.
	*/

	// show about message box and load default model
	showAboutText();
	loadModel(StartUpModelFile);

	// add skybox

	SkyBox = smgr.addSkyBoxSceneNode(driver.getTexture("irrlicht2_up.jpg"), driver.getTexture("irrlicht2_dn.jpg"), driver.getTexture("irrlicht2_lf.jpg"), driver.getTexture("irrlicht2_rt.jpg"), driver.getTexture("irrlicht2_ft.jpg"), driver.getTexture("irrlicht2_bk.jpg"));

	// add a camera scene node
	Camera[0] = smgr.addCameraSceneNodeMaya();
	Camera[0].setFarValue(20000.0);
	// Maya cameras reposition themselves relative to their target, so target the location
	// where the mesh scene node is placed.
	Camera[0].setTarget(new irr.vector3df(0,30,0));

	Camera[1] = smgr.addCameraSceneNodeFPS();
	Camera[1].setFarValue(20000.0);
	Camera[1].setPosition(new irr.vector3df(0,0,-70));
	Camera[1].setTarget(new irr.vector3df(0,30,0));

	setActiveCamera(Camera[0]);

	// load the irrlicht engine logo
	var img = env.addImage(driver.getTexture("irrlichtlogo2.png"), new irr.position2d_s32(10, driver.getScreenSize().Height - 128));

	// lock the logo's edges to the bottom left corner of the screen
	img.setAlignment(irr.EGUIA_UPPERLEFT, irr.EGUIA_UPPERLEFT, irr.EGUIA_LOWERRIGHT, irr.EGUIA_LOWERRIGHT);

	// draw everything

	while(Device.run() && driver)
	{
		if (Device.isWindowActive())
		{
			driver.beginScene(true, true, new irr.SColor(150,50,50,50));

			smgr.drawAll();
			env.drawAll();

			driver.endScene();

			// update information about current frame-rate
			var str = "FPS: ";
			str = str + ("" + driver.getFPS());
			str = str + " Tris: ";
			str = str + ("" + driver.getPrimitiveCountDrawn());
			fpstext.setText(str);

			// update information about the active camera
			var cam = Device.getSceneManager().getActiveCamera();
			str = "Pos: ";
			str = str + cam.getPosition().X;
			str = str + " ";
			str = str + cam.getPosition().Y;
			str = str + " ";
			str = str + cam.getPosition().Z;
			str = str + " Tgt: ";
			str = str + cam.getTarget().X;
			str = str + " ";
			str = str + cam.getTarget().Y;
			str = str + " ";
			str = str + cam.getTarget().Z;
			postext.setText(str);

			// update the tool dialog
			updateToolBox();
		}
		else
			Device.yield();
	}

	Device.drop();
	return 0;
}

start();
