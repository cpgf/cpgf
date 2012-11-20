cpgf._import(None, "builtin.core");
cpgf._import(None, "builtin.debug");

Device = None;
StartUpModelFile = "";
MessageText = "";
Caption = "";
Model = None;
SkyBox = None;
Octree=False;
UseLight=False;

Camera = [ None, None ];

GUI_ID_DIALOG_ROOT_WINDOW  = 0x10000;

GUI_ID_X_SCALE = 0x10001;
GUI_ID_Y_SCALE = 0x10002;
GUI_ID_Z_SCALE = 0x10003;

GUI_ID_OPEN_MODEL = 0x10004;
GUI_ID_SET_MODEL_ARCHIVE = 0x10005;
GUI_ID_LOAD_AS_OCTREE = 0x10006;

GUI_ID_SKY_BOX_VISIBLE = 0x10007;
GUI_ID_TOGGLE_DEBUG_INFO = 0x10008;

GUI_ID_DEBUG_OFF = 0x10009;
GUI_ID_DEBUG_BOUNDING_BOX = 0x1000A;
GUI_ID_DEBUG_NORMALS = 0x1000B;
GUI_ID_DEBUG_SKELETON = 0x1000C;
GUI_ID_DEBUG_WIRE_OVERLAY = 0x1000D;
GUI_ID_DEBUG_HALF_TRANSPARENT = 0x1000E;
GUI_ID_DEBUG_BUFFERS_BOUNDING_BOXES = 0x1000F;
GUI_ID_DEBUG_ALL = 0x10010;

GUI_ID_MODEL_MATERIAL_SOLID = 0x10011;
GUI_ID_MODEL_MATERIAL_TRANSPARENT = 0x10012;
GUI_ID_MODEL_MATERIAL_REFLECTION = 0x10013;

GUI_ID_CAMERA_MAYA = 0x10014;
GUI_ID_CAMERA_FIRST_PERSON = 0x10015;

GUI_ID_POSITION_TEXT = 0x10016;

GUI_ID_ABOUT = 0x10017;
GUI_ID_QUIT = 0x10018;

GUI_ID_TEXTUREFILTER = 0x10019;
GUI_ID_SKIN_TRANSPARENCY = 0x1001A;
GUI_ID_SKIN_ANIMATION_FPS = 0x1001B;

GUI_ID_BUTTON_SET_SCALE = 0x1001C;
GUI_ID_BUTTON_SCALE_MUL10 = 0x1001D;
GUI_ID_BUTTON_SCALE_DIV10 = 0x1001E;
GUI_ID_BUTTON_OPEN_MODEL = 0x1001F;
GUI_ID_BUTTON_SHOW_ABOUT = 0x10020;
GUI_ID_BUTTON_SHOW_TOOLBOX = 0x10021;
GUI_ID_BUTTON_SELECT_ARCHIVE = 0x10022;

GUI_ID_ANIMATION_INFO = 0x10023;

MAX_FRAMERATE = 80;
DEFAULT_FRAMERATE = 30;


def setActiveCamera(newActive) :
	global Device;
	global StartUpModelFile;
	global MessageText;
	global Caption;
	global Model;
	global SkyBox;
	global Octree;
	global UseLight;
	global Camera;
	
	if None == Device :
		return;

	active = Device.getSceneManager().getActiveCamera();
	active.setInputReceiverEnabled(False);

	newActive.setInputReceiverEnabled(True);
	Device.getSceneManager().setActiveCamera(newActive);

def setSkinTransparency(alpha, skin) :
	for i in range(irr.EGDC_COUNT) :
		col = skin.getColor(i);
		col.setAlpha(alpha);
		skin.setColor(i, col);

def updateScaleInfo(model) :
	toolboxWnd = Device.getGUIEnvironment().getRootGUIElement().getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, True);
	if toolboxWnd == None :
		return;

	if model == None :
		toolboxWnd.getElementFromId(GUI_ID_X_SCALE, True).setText( "-" );
		toolboxWnd.getElementFromId(GUI_ID_Y_SCALE, True).setText( "-" );
		toolboxWnd.getElementFromId(GUI_ID_Z_SCALE, True).setText( "-" );
	else :
		scale = model.getScale();
		toolboxWnd.getElementFromId(GUI_ID_X_SCALE, True).setText( "" + str(scale.X) );
		toolboxWnd.getElementFromId(GUI_ID_Y_SCALE, True).setText( "" + str(scale.Y) );
		toolboxWnd.getElementFromId(GUI_ID_Z_SCALE, True).setText( "" + str(scale.Z) );

def showAboutText() :
	Device.getGUIEnvironment().addMessageBox(Caption, MessageText);


def loadModel(fn) :
	global Device;
	global StartUpModelFile;
	global MessageText;
	global Caption;
	global Model;
	global SkyBox;
	global Octree;
	global UseLight;
	global Camera;
	

	filename = fn;

	extension = filename[len(filename) - 4 : len(filename)];
	extension = extension.lower();

	if extension == ".jpg" or extension == ".pcx" or extension == ".png" or extension == ".ppm" or extension == ".pgm" or extension == ".pbm" or extension == ".psd" or extension == ".tga" or extension == ".bmp" or extension == ".wa" or extension == ".rgb" or extension == ".rgba" :
		texture = Device.getVideoDriver().getTexture( filename );
		if  texture != None and Model != None :
			Device.getVideoDriver().removeTexture(texture);
			texture = Device.getVideoDriver().getTexture( filename );

			Model.setMaterialTexture(0, texture);
		return;
	elif extension == ".pk3" or extension == ".zip" or extension == ".pak" or extension == ".npk" :
		Device.getFileSystem().addFileArchive(filename);
		return;

	if Model != None :
		Model.remove();

	Model = None;

	if extension ==".irr" :
		return;

	m = Device.getSceneManager().getMesh( filename );

	if m == None :
		if StartUpModelFile != filename :
			Device.getGUIEnvironment().addMessageBox(Caption, "The model could not be loaded. Maybe it is not a supported file format.");
		return;


	if Octree :
		Model = Device.getSceneManager().addOctreeSceneNode(m.getMesh(0));
	else :
		animModel = Device.getSceneManager().addAnimatedMeshSceneNode(m);
		animModel.setAnimationSpeed(30);
		Model = animModel;
	Model.setMaterialFlag(irr.EMF_LIGHTING, UseLight);
	Model.setMaterialFlag(irr.EMF_NORMALIZE_NORMALS, UseLight);
	Model.setDebugDataVisible(irr.EDS_OFF);

	menu = cpgf.cast(Device.getGUIEnvironment().getRootGUIElement().getElementFromId(GUI_ID_TOGGLE_DEBUG_INFO, True), irr.IGUIContextMenu);
	if menu != None :
		for item in range(5) :
			menu.setItemChecked(item, False);
	updateScaleInfo(Model);


def createToolBox() :
	global Device;
	global StartUpModelFile;
	global MessageText;
	global Caption;
	global Model;
	global SkyBox;
	global Octree;
	global UseLight;
	global Camera;
	
	env = Device.getGUIEnvironment();
	root = env.getRootGUIElement();
	e = root.getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, True);
	if e != None :
		e.remove();

	wnd = env.addWindow(irr.rect_s32(600,45,800,480),	False, "Toolset", None, GUI_ID_DIALOG_ROOT_WINDOW);

	tab = env.addTabControl(irr.rect_s32(2,20,800-602,480-7), wnd, True, True);

	t1 = tab.addTab("Config");

	env.addStaticText("Scale:", irr.rect_s32(10,20,60,45), False, False, t1);
	env.addStaticText("X:", irr.rect_s32(22,48,40,66), False, False, t1);
	env.addEditBox("1.0", irr.rect_s32(40,46,130,66), True, t1, GUI_ID_X_SCALE);
	env.addStaticText("Y:", irr.rect_s32(22,82,40,96), False, False, t1);
	env.addEditBox("1.0", irr.rect_s32(40,76,130,96), True, t1, GUI_ID_Y_SCALE);
	env.addStaticText("Z:", irr.rect_s32(22,108,40,126), False, False, t1);
	env.addEditBox("1.0", irr.rect_s32(40,106,130,126), True, t1, GUI_ID_Z_SCALE);

	env.addButton(irr.rect_s32(10,134,85,165), t1, GUI_ID_BUTTON_SET_SCALE, "Set");

	env.addButton(irr.rect_s32(65,20,95,40), t1, GUI_ID_BUTTON_SCALE_MUL10, "* 10");
	env.addButton(irr.rect_s32(100,20,130,40), t1, GUI_ID_BUTTON_SCALE_DIV10, "* 0.1");

	updateScaleInfo(Model);

	env.addStaticText("GUI Transparency Control:", irr.rect_s32(10,200,150,225), True, False, t1);
	scrollbar = env.addScrollBar(True, irr.rect_s32(10,225,150,240), t1, GUI_ID_SKIN_TRANSPARENCY);
	scrollbar.setMax(255);
	scrollbar.setPos(255);

	env.addStaticText(":", irr.rect_s32(10,240,150,265), True, False, t1);
	env.addStaticText("Framerate:", irr.rect_s32(12,240,75,265), False, False, t1);
	env.addStaticText("", irr.rect_s32(75,240,200,265), False, False, t1, GUI_ID_ANIMATION_INFO);
	scrollbar = env.addScrollBar(True, irr.rect_s32(10,265,150,280), t1, GUI_ID_SKIN_ANIMATION_FPS);
	scrollbar.setMax(MAX_FRAMERATE);
	scrollbar.setMin(-MAX_FRAMERATE);
	scrollbar.setPos(DEFAULT_FRAMERATE);
	scrollbar.setSmallStep(1);

def updateToolBox() :
	global Device;
	global StartUpModelFile;
	global MessageText;
	global Caption;
	global Model;
	global SkyBox;
	global Octree;
	global UseLight;
	global Camera;
	
	env = Device.getGUIEnvironment();
	root = env.getRootGUIElement();
	dlg = root.getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, True);
	if dlg == None  :
		return;

	aniInfo = cpgf.cast(dlg.getElementFromId(GUI_ID_ANIMATION_INFO, True), irr.IGUIStaticText);
	if aniInfo != None :
		if  Model != None and irr.ESNT_ANIMATED_MESH == Model.getType()  :
			animatedModel = cpgf.cast(Model, irr.IAnimatedMeshSceneNode);

			tmp = "" + str(animatedModel.getAnimationSpeed());
			tmp = tmp + " Frame: ";
			tmp = tmp + str(animatedModel.getFrameNr());
			aniInfo.setText(tmp);
		else :
			aniInfo.setText("");

def hasModalDialog() :
	env = Device.getGUIEnvironment();
	focused = env.getFocus();
	while  focused != None  :
		if  focused.isVisible() and focused.hasType(irr.EGUIET_MODAL_SCREEN)  :
			return True;
		focused = focused.getParent();
	return False;

def overrideEventReceiver(receiver) :
	global Device;
	global StartUpModelFile;
	global MessageText;
	global Caption;
	global Model;
	global SkyBox;
	global Octree;
	global UseLight;
	global Camera;
	
	def OnKeyUp(keyCode) :
		if  hasModalDialog()  :
			return False;

		if keyCode == irr.KEY_ESCAPE :
			if Device != None :
				camera = Device.getSceneManager().getActiveCamera();
				if camera :
					camera.setInputReceiverEnabled( not camera.isInputReceiverEnabled() );
				return True;
		elif keyCode == irr.KEY_F1 :
			if Device != None :
				elem = Device.getGUIEnvironment().getRootGUIElement().getElementFromId(GUI_ID_POSITION_TEXT);
				if elem :
					elem.setVisible(not elem.isVisible());
		elif keyCode == irr.KEY_KEY_M :
			if Device != None :
				Device.minimizeWindow();
		elif keyCode == irr.KEY_KEY_L :
			UseLight= not UseLight;
			if Model != None :
				Model.setMaterialFlag(irr.EMF_LIGHTING, UseLight);
				Model.setMaterialFlag(irr.EMF_NORMALIZE_NORMALS, UseLight);
		return False;

	def OnMenuItemSelected(menu) :
		id = menu.getItemCommandId(menu.getSelectedItem());
		env = Device.getGUIEnvironment();

		if id == GUI_ID_OPEN_MODEL :
			env.addFileOpenDialog("Please select a model file to open");
			
		elif id == GUI_ID_SET_MODEL_ARCHIVE :
			env.addFileOpenDialog("Please select your game archive/directory");
			
		elif id == GUI_ID_LOAD_AS_OCTREE :
			Octree = not Octree;
			menu.setItemChecked(menu.getSelectedItem(), Octree);
			
		elif id == GUI_ID_QUIT :
			Device.closeDevice();
			
		elif id == GUI_ID_SKY_BOX_VISIBLE :
			menu.setItemChecked(menu.getSelectedItem(), not menu.isItemChecked(menu.getSelectedItem()));
			SkyBox.setVisible(not SkyBox.isVisible());
			
		elif id == GUI_ID_DEBUG_OFF :
			menu.setItemChecked(menu.getSelectedItem()+1, False);
			menu.setItemChecked(menu.getSelectedItem()+2, False);
			menu.setItemChecked(menu.getSelectedItem()+3, False);
			menu.setItemChecked(menu.getSelectedItem()+4, False);
			menu.setItemChecked(menu.getSelectedItem()+5, False);
			menu.setItemChecked(menu.getSelectedItem()+6, False);
			if Model != None :
				Model.setDebugDataVisible(irr.EDS_OFF);
			
		elif id == GUI_ID_DEBUG_BOUNDING_BOX :
			menu.setItemChecked(menu.getSelectedItem(), not menu.isItemChecked(menu.getSelectedItem()));
			if Model != None :
				Model.setDebugDataVisible(Model.isDebugDataVisible() ^ irr.EDS_BBOX);
			
		elif id == GUI_ID_DEBUG_NORMALS :
			menu.setItemChecked(menu.getSelectedItem(), not menu.isItemChecked(menu.getSelectedItem()));
			if Model != None :
				Model.setDebugDataVisible(Model.isDebugDataVisible() ^ irr.EDS_NORMALS);
			
		elif id == GUI_ID_DEBUG_SKELETON :
			menu.setItemChecked(menu.getSelectedItem(), not menu.isItemChecked(menu.getSelectedItem()));
			if Model != None :
				Model.setDebugDataVisible(Model.isDebugDataVisible() ^ irr.EDS_SKELETON);
			
		elif id == GUI_ID_DEBUG_WIRE_OVERLAY :
			menu.setItemChecked(menu.getSelectedItem(), not menu.isItemChecked(menu.getSelectedItem()));
			if Model != None :
				Model.setDebugDataVisible(Model.isDebugDataVisible() ^ irr.EDS_MESH_WIRE_OVERLAY);
		elif id == GUI_ID_DEBUG_HALF_TRANSPARENT :
			menu.setItemChecked(menu.getSelectedItem(), not menu.isItemChecked(menu.getSelectedItem()));
			if Model != None :
				Model.setDebugDataVisible(Model.isDebugDataVisible() ^ irr.EDS_HALF_TRANSPARENCY);
		elif id == GUI_ID_DEBUG_BUFFERS_BOUNDING_BOXES :
			menu.setItemChecked(menu.getSelectedItem(), not menu.isItemChecked(menu.getSelectedItem()));
			if Model != None :
				Model.setDebugDataVisible(Model.isDebugDataVisible() ^ irr.EDS_BBOX_BUFFERS);
		elif id == GUI_ID_DEBUG_ALL :
			menu.setItemChecked(menu.getSelectedItem()-1, True);
			menu.setItemChecked(menu.getSelectedItem()-2, True);
			menu.setItemChecked(menu.getSelectedItem()-3, True);
			menu.setItemChecked(menu.getSelectedItem()-4, True);
			menu.setItemChecked(menu.getSelectedItem()-5, True);
			menu.setItemChecked(menu.getSelectedItem()-6, True);
			if Model != None :
				Model.setDebugDataVisible(irr.EDS_FULL);
		elif id == GUI_ID_ABOUT :
			showAboutText();
		elif id == GUI_ID_MODEL_MATERIAL_SOLID :
			if Model != None :
				Model.setMaterialType(irr.EMT_SOLID);
		elif id == GUI_ID_MODEL_MATERIAL_TRANSPARENT :
			if Model != None :
				Model.setMaterialType(irr.EMT_TRANSPARENT_ADD_COLOR);
		elif id == GUI_ID_MODEL_MATERIAL_REFLECTION :
			if Model != None :
				Model.setMaterialType(irr.EMT_SPHERE_MAP);

		elif id == GUI_ID_CAMERA_MAYA :
			setActiveCamera(Camera[0]);
		elif id == GUI_ID_CAMERA_FIRST_PERSON :
			setActiveCamera(Camera[1]);

	def OnTextureFilterSelected(combo) :
		pos = combo.getSelected();
		if pos == 0 :
			if Model != None :
				Model.setMaterialFlag(irr.EMF_BILINEAR_FILTER, False);
				Model.setMaterialFlag(irr.EMF_TRILINEAR_FILTER, False);
				Model.setMaterialFlag(irr.EMF_ANISOTROPIC_FILTER, False);
		elif pos == 1 :
			if Model != None :
				Model.setMaterialFlag(irr.EMF_BILINEAR_FILTER, True);
				Model.setMaterialFlag(irr.EMF_TRILINEAR_FILTER, False);
		elif pos == 2 :
			if Model != None :
				Model.setMaterialFlag(irr.EMF_BILINEAR_FILTER, False);
				Model.setMaterialFlag(irr.EMF_TRILINEAR_FILTER, True);
		elif pos == 3 :
			if Model != None :
				Model.setMaterialFlag(irr.EMF_ANISOTROPIC_FILTER, True);
		elif pos == 4 :
			if Model != None :
				Model.setMaterialFlag(irr.EMF_ANISOTROPIC_FILTER, False);

	def OnEvent(me, event) :
		if event.EventType == irr.EET_KEY_INPUT_EVENT and event.KeyInput.PressedDown == False :
			if  OnKeyUp(event.KeyInput.Key)  :
				return True;

		if event.EventType == irr.EET_GUI_EVENT :
			id = event.GUIEvent.Caller.getID();
			env = Device.getGUIEnvironment();

			if event.GUIEvent.EventType == irr.EGET_MENU_ITEM_SELECTED :
				OnMenuItemSelected( cpgf.cast(event.GUIEvent.Caller, irr.IGUIContextMenu) );

			elif event.GUIEvent.EventType == irr.EGET_FILE_SELECTED :
				dialog = cpgf.cast(event.GUIEvent.Caller, irr.IGUIFileOpenDialog);
				loadModel(dialog.getFileName());

			elif event.GUIEvent.EventType == irr.EGET_SCROLL_BAR_CHANGED :
				if id == GUI_ID_SKIN_TRANSPARENCY :
					pos = cpgf.cast(event.GUIEvent.Caller, irr.IGUIScrollBar).getPos();
					setSkinTransparency(pos, env.getSkin());
				elif id == GUI_ID_SKIN_ANIMATION_FPS :
					pos = cpgf.cast(event.GUIEvent.Caller, irr.IGUIScrollBar).getPos();
					if Model != None and irr.ESNT_ANIMATED_MESH == Model.getType() :
						cpgf.cast(Model, irr.IAnimatedMeshSceneNode).setAnimationSpeed(pos);

			elif event.GUIEvent.EventType == irr.EGET_COMBO_BOX_CHANGED :
				if id == GUI_ID_TEXTUREFILTER :
					OnTextureFilterSelected( cpgf.cast(event.GUIEvent.Caller, irr.IGUIComboBox) );

			elif event.GUIEvent.EventType == irr.EGET_BUTTON_CLICKED :
				if id == GUI_ID_BUTTON_SET_SCALE :
					root = env.getRootGUIElement();
					scale = irr.vector3df();
					s;

					s = root.getElementFromId(GUI_ID_X_SCALE, True).getText();
					scale.X = tonumber(s);
					s = root.getElementFromId(GUI_ID_Y_SCALE, True).getText();
					scale.Y = tonumber(s);
					s = root.getElementFromId(GUI_ID_Z_SCALE, True).getText();
					scale.Z = tonumber(s);

					if Model != None :
						Model.setScale(scale);
					updateScaleInfo(Model);
				elif id == GUI_ID_BUTTON_SCALE_MUL10 :
					if Model != None :
						scale = irr.vector3df(Model.getScale());
						scale.X = scale.X * 10;
						scale.Y = scale.Y * 10;
						scale.Z = scale.Z * 10;
						Model.setScale(scale);
					updateScaleInfo(Model);

				elif id == GUI_ID_BUTTON_SCALE_DIV10 :
					if Model != None :
						scale = irr.vector3df(Model.getScale());
						scale.X = scale.X * 0.1;
						scale.Y = scale.Y * 0.1;
						scale.Z = scale.Z * 0.1;
						Model.setScale(scale);
					updateScaleInfo(Model);

				elif id == GUI_ID_BUTTON_OPEN_MODEL :
					env.addFileOpenDialog("Please select a model file to open");

				elif id == GUI_ID_BUTTON_SHOW_ABOUT :
					showAboutText();

				elif id == GUI_ID_BUTTON_SHOW_TOOLBOX :
					createToolBox();

				elif id == GUI_ID_BUTTON_SELECT_ARCHIVE :
					env.addFileOpenDialog("Please select your game archive/directory");

		return False;

	receiver.OnEvent = OnEvent;



def start() :
	global Device;
	global StartUpModelFile;
	global MessageText;
	global Caption;
	global Model;
	global SkyBox;
	global Octree;
	global UseLight;
	global Camera;
	
	driverType=irr.driverChoiceConsole();
	if driverType==irr.EDT_COUNT :
		return 1;

	receiver = irr.IEventReceiverWrapper();
	overrideEventReceiver(receiver);
	Device = irr.createDevice(driverType, irr.dimension2d_u32(800, 600), 16, False, False, False, receiver);

	if Device == 0 :
		return 1;

	Device.setResizable(True);

	Device.setWindowCaption("cpgf Irrlicht Python Binding - Loading...");

	driver = Device.getVideoDriver();
	env = Device.getGUIEnvironment();
	smgr = Device.getSceneManager();
	smgr.getParameters().setAttribute(irr.COLLADA_CREATE_SCENE_INSTANCES, True);

	driver.setTextureCreationFlag(irr.ETCF_ALWAYS_32_BIT, True);

	smgr.addLightSceneNode(None, irr.vector3df(200,200,200), irr.SColorf(1.0,1.0,1.0),2000);
	smgr.setAmbientLight(irr.SColorf(0.3,0.3,0.3));
	Device.getFileSystem().addFolderFileArchive("../../media/");

	xml = Device.getFileSystem().createXMLReader( "config.xml");

	while xml != None and xml.read() :
		t = xml.getNodeType()
		if t == irr.EXN_TEXT :
			MessageText = xml.getNodeData();
			
		elif t == irr.EXN_ELEMENT :
			if "startUpModel" == xml.getNodeName() :
				StartUpModelFile = xml.getAttributeValue("file");
			elif "messageText" == xml.getNodeName() :
				Caption = xml.getAttributeValue("caption");

	if xml :
		xml.drop();

	skin = env.getSkin();
	font = env.getFont("fonthaettenschweiler.bmp");
	if font != None :
		skin.setFont(font);

	menu = env.addMenu();
	menu.addItem("File", -1, True, True);
	menu.addItem("View", -1, True, True);
	menu.addItem("Camera", -1, True, True);
	menu.addItem("Help", -1, True, True);

	submenu = menu.getSubMenu(0);
	submenu.addItem("Open Model File & Texture...", GUI_ID_OPEN_MODEL);
	submenu.addItem("Set Model Archive...", GUI_ID_SET_MODEL_ARCHIVE);
	submenu.addItem("Load as Octree", GUI_ID_LOAD_AS_OCTREE);
	submenu.addSeparator();
	submenu.addItem("Quit", GUI_ID_QUIT);

	submenu = menu.getSubMenu(1);
	submenu.addItem("sky box visible", GUI_ID_SKY_BOX_VISIBLE, True, False, True);
	submenu.addItem("toggle model debug information", GUI_ID_TOGGLE_DEBUG_INFO, True, True);
	submenu.addItem("model materia", -1, True, True );

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

	bar = env.addToolBar();

	image = driver.getTexture("open.png");
	bar.addButton(GUI_ID_BUTTON_OPEN_MODEL, None, "Open a mode",image, None, False, True);

	image = driver.getTexture("tools.png");
	bar.addButton(GUI_ID_BUTTON_SHOW_TOOLBOX, None, "Open Toolset",image, None, False, True);

	image = driver.getTexture("zip.png");
	bar.addButton(GUI_ID_BUTTON_SELECT_ARCHIVE, None, "Set Model Archive",image, None, False, True);

	image = driver.getTexture("help.png");
	bar.addButton(GUI_ID_BUTTON_SHOW_ABOUT, None, "Open Help", image, None, False, True);


	box = env.addComboBox(irr.rect_s32(250,4,350,23), bar, GUI_ID_TEXTUREFILTER);
	box.addItem("No filtering");
	box.addItem("Bilinear");
	box.addItem("Trilinear");
	box.addItem("Anisotropic");
	box.addItem("Isotropic");

	for i in range(irr.EGDC_COUNT) :
		col = env.getSkin().getColor(i);
		col.setAlpha(255);
		env.getSkin().setColor(i, col);


	createToolBox();

	fpstext = env.addStaticText("",	irr.rect_s32(400,4,570,23), True, False, bar);

	postext = env.addStaticText("",	irr.rect_s32(10,50,470,80),False, False, None, GUI_ID_POSITION_TEXT);
	postext.setVisible(False);


	Caption = Caption + " - [";
	Caption = Caption + driver.getName();
	Caption = Caption + "]";
	Device.setWindowCaption(Caption);

	showAboutText();
	loadModel(StartUpModelFile);


	SkyBox = smgr.addSkyBoxSceneNode(driver.getTexture("irrlicht2_up.jpg"), driver.getTexture("irrlicht2_dn.jpg"), driver.getTexture("irrlicht2_lf.jpg"), driver.getTexture("irrlicht2_rt.jpg"), driver.getTexture("irrlicht2_ft.jpg"), driver.getTexture("irrlicht2_bk.jpg"));

	Camera[0] = smgr.addCameraSceneNodeMaya();
	Camera[0].setFarValue(20000.0);
	Camera[0].setTarget(irr.vector3df(0,30,0));

	Camera[1] = smgr.addCameraSceneNodeFPS();
	Camera[1].setFarValue(20000.0);
	Camera[1].setPosition(irr.vector3df(0,0,-70));
	Camera[1].setTarget(irr.vector3df(0,30,0));

	setActiveCamera(Camera[0]);

	img = env.addImage(driver.getTexture("irrlichtlogo2.png"), irr.position2d_s32(10, driver.getScreenSize().Height - 128));

	img.setAlignment(irr.EGUIA_UPPERLEFT, irr.EGUIA_UPPERLEFT, irr.EGUIA_LOWERRIGHT, irr.EGUIA_LOWERRIGHT);


	while Device.run() and driver :
		if Device.isWindowActive() :
			driver.beginScene(True, True, irr.SColor(150,50,50,50));

			smgr.drawAll();
			env.drawAll();

			driver.endScene();

			tmp = "FPS: ";
			tmp = tmp + str(driver.getFPS());
			tmp = tmp + " Tris: ";
			tmp = tmp + str(driver.getPrimitiveCountDrawn());
			fpstext.setText(tmp);

			cam = Device.getSceneManager().getActiveCamera();
			tmp = "Pos: ";
			tmp = tmp + str(cam.getPosition().X);
			tmp = tmp + " ";
			tmp = tmp + str(cam.getPosition().Y);
			tmp = tmp + " ";
			tmp = tmp + str(cam.getPosition().Z);
			tmp = tmp + " Tgt: ";
			tmp = tmp + str(cam.getTarget().X);
			tmp = tmp + " ";
			tmp = tmp + str(cam.getTarget().Y);
			tmp = tmp + " ";
			tmp = tmp + str(cam.getTarget().Z);
			postext.setText(tmp);

			updateToolBox();

	Device.drop();
	return 0;

start();
