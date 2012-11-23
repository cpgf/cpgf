cpgf.import(nil, "builtin.core");
cpgf.import(nil, "builtin.debug");

local Device = nil;
local StartUpModelFile = "";
local MessageText = "";
local Caption = "";
local Model = nil;
local SkyBox = nil;
local Octree=false;
local UseLight=false;

local Camera = { nil, nil };

local GUI_ID_DIALOG_ROOT_WINDOW  = 0x10000;

local GUI_ID_X_SCALE = 0x10001;
local GUI_ID_Y_SCALE = 0x10002;
local GUI_ID_Z_SCALE = 0x10003;

local GUI_ID_OPEN_MODEL = 0x10004;
local GUI_ID_SET_MODEL_ARCHIVE = 0x10005;
local GUI_ID_LOAD_AS_OCTREE = 0x10006;

local GUI_ID_SKY_BOX_VISIBLE = 0x10007;
local GUI_ID_TOGGLE_DEBUG_INFO = 0x10008;

local GUI_ID_DEBUG_OFF = 0x10009;
local GUI_ID_DEBUG_BOUNDING_BOX = 0x1000A;
local GUI_ID_DEBUG_NORMALS = 0x1000B;
local GUI_ID_DEBUG_SKELETON = 0x1000C;
local GUI_ID_DEBUG_WIRE_OVERLAY = 0x1000D;
local GUI_ID_DEBUG_HALF_TRANSPARENT = 0x1000E;
local GUI_ID_DEBUG_BUFFERS_BOUNDING_BOXES = 0x1000F;
local GUI_ID_DEBUG_ALL = 0x10010;

local GUI_ID_MODEL_MATERIAL_SOLID = 0x10011;
local GUI_ID_MODEL_MATERIAL_TRANSPARENT = 0x10012;
local GUI_ID_MODEL_MATERIAL_REFLECTION = 0x10013;

local GUI_ID_CAMERA_MAYA = 0x10014;
local GUI_ID_CAMERA_FIRST_PERSON = 0x10015;

local GUI_ID_POSITION_TEXT = 0x10016;

local GUI_ID_ABOUT = 0x10017;
local GUI_ID_QUIT = 0x10018;

local GUI_ID_TEXTUREFILTER = 0x10019;
local GUI_ID_SKIN_TRANSPARENCY = 0x1001A;
local GUI_ID_SKIN_ANIMATION_FPS = 0x1001B;

local GUI_ID_BUTTON_SET_SCALE = 0x1001C;
local GUI_ID_BUTTON_SCALE_MUL10 = 0x1001D;
local GUI_ID_BUTTON_SCALE_DIV10 = 0x1001E;
local GUI_ID_BUTTON_OPEN_MODEL = 0x1001F;
local GUI_ID_BUTTON_SHOW_ABOUT = 0x10020;
local GUI_ID_BUTTON_SHOW_TOOLBOX = 0x10021;
local GUI_ID_BUTTON_SELECT_ARCHIVE = 0x10022;

local GUI_ID_ANIMATION_INFO = 0x10023;

local MAX_FRAMERATE = 80;
local DEFAULT_FRAMERATE = 30;


function setActiveCamera(newActive)
	if nil == Device then
		return;
	end

	local active = Device.getSceneManager().getActiveCamera();
	active.setInputReceiverEnabled(false);

	newActive.setInputReceiverEnabled(true);
	Device.getSceneManager().setActiveCamera(newActive);
end

function setSkinTransparency(alpha, skin)
	for i=0, irr.EGDC_COUNT - 1 do
		local col = skin.getColor(i);
		col.setAlpha(alpha);
		skin.setColor(i, col);
	end
end

function updateScaleInfo(model)
	local toolboxWnd = Device.getGUIEnvironment().getRootGUIElement().getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
	if toolboxWnd == nil then
		return;
	end

	if model == nil then
		toolboxWnd.getElementFromId(GUI_ID_X_SCALE, true).setText( "-" );
		toolboxWnd.getElementFromId(GUI_ID_Y_SCALE, true).setText( "-" );
		toolboxWnd.getElementFromId(GUI_ID_Z_SCALE, true).setText( "-" );
	else
		local scale = model.getScale();
		toolboxWnd.getElementFromId(GUI_ID_X_SCALE, true).setText( "" .. scale.X );
		toolboxWnd.getElementFromId(GUI_ID_Y_SCALE, true).setText( "" .. scale.Y );
		toolboxWnd.getElementFromId(GUI_ID_Z_SCALE, true).setText( "" .. scale.Z );
	end
end

function showAboutText()
	Device.getGUIEnvironment().addMessageBox(Caption, MessageText);
end


function loadModel(fn)

	local filename = fn;

	local extension = string.sub(filename, string.len(filename) - 4, -1);
	extension = string.lower(extension);

	if extension == ".jpg" or extension == ".pcx" or extension == ".png" or extension == ".ppm" or extension == ".pgm" or extension == ".pbm" or extension == ".psd" or extension == ".tga" or extension == ".bmp" or extension == ".wa" or extension == ".rgb" or extension == ".rgba" then
		local texture = Device.getVideoDriver().getTexture( filename );
		if  texture ~= nil and Model ~= nil then
			Device.getVideoDriver().removeTexture(texture);
			texture = Device.getVideoDriver().getTexture( filename );

			Model.setMaterialTexture(0, texture);
		end
		return;
	elseif extension == ".pk3" or extension == ".zip" or extension == ".pak" or extension == ".npk" then
		Device.getFileSystem().addFileArchive(filename);
		return;
	end

	if Model ~= nil then
		Model.remove();
	end

	Model = nil;

	if extension ==".irr" then
		return;
	end

	local m = Device.getSceneManager().getMesh( filename );

	if m == nil then
		if StartUpModelFile ~= filename then
			Device.getGUIEnvironment().addMessageBox(Caption, "The model could not be loaded. Maybe it is not a supported file format.");
		end
		return;
	end


	if Octree then
		Model = Device.getSceneManager().addOctreeSceneNode(m.getMesh(0));
	else
		local animModel = Device.getSceneManager().addAnimatedMeshSceneNode(m);
		animModel.setAnimationSpeed(30);
		Model = animModel;
	end
	Model.setMaterialFlag(irr.EMF_LIGHTING, UseLight);
	Model.setMaterialFlag(irr.EMF_NORMALIZE_NORMALS, UseLight);
	Model.setDebugDataVisible(irr.EDS_OFF);

	local menu = cpgf.cast(Device.getGUIEnvironment().getRootGUIElement().getElementFromId(GUI_ID_TOGGLE_DEBUG_INFO, true), irr.IGUIContextMenu);
	if menu ~= nil then
		for item = 1, 5 do
			menu.setItemChecked(item, false);
		end
	end
	updateScaleInfo(Model);
end


function createToolBox()
	local env = Device.getGUIEnvironment();
	local root = env.getRootGUIElement();
	local e = root.getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
	if e ~= nil then
		e.remove();
	end

	local wnd = env.addWindow(irr.rect_s32(600,45,800,480),	false, "Toolset", nil, GUI_ID_DIALOG_ROOT_WINDOW);

	local tab = env.addTabControl(irr.rect_s32(2,20,800-602,480-7), wnd, true, true);

	local t1 = tab.addTab("Config");

	env.addStaticText("Scale:", irr.rect_s32(10,20,60,45), false, false, t1);
	env.addStaticText("X:", irr.rect_s32(22,48,40,66), false, false, t1);
	env.addEditBox("1.0", irr.rect_s32(40,46,130,66), true, t1, GUI_ID_X_SCALE);
	env.addStaticText("Y:", irr.rect_s32(22,82,40,96), false, false, t1);
	env.addEditBox("1.0", irr.rect_s32(40,76,130,96), true, t1, GUI_ID_Y_SCALE);
	env.addStaticText("Z:", irr.rect_s32(22,108,40,126), false, false, t1);
	env.addEditBox("1.0", irr.rect_s32(40,106,130,126), true, t1, GUI_ID_Z_SCALE);

	env.addButton(irr.rect_s32(10,134,85,165), t1, GUI_ID_BUTTON_SET_SCALE, "Set");

	env.addButton(irr.rect_s32(65,20,95,40), t1, GUI_ID_BUTTON_SCALE_MUL10, "* 10");
	env.addButton(irr.rect_s32(100,20,130,40), t1, GUI_ID_BUTTON_SCALE_DIV10, "* 0.1");

	updateScaleInfo(Model);

	env.addStaticText("GUI Transparency Control:", irr.rect_s32(10,200,150,225), true, false, t1);
	local scrollbar = env.addScrollBar(true, irr.rect_s32(10,225,150,240), t1, GUI_ID_SKIN_TRANSPARENCY);
	scrollbar.setMax(255);
	scrollbar.setPos(255);

	env.addStaticText(":", irr.rect_s32(10,240,150,265), true, false, t1);
	env.addStaticText("Framerate:", irr.rect_s32(12,240,75,265), false, false, t1);
	env.addStaticText("", irr.rect_s32(75,240,200,265), false, false, t1, GUI_ID_ANIMATION_INFO);
	scrollbar = env.addScrollBar(true, irr.rect_s32(10,265,150,280), t1, GUI_ID_SKIN_ANIMATION_FPS);
	scrollbar.setMax(MAX_FRAMERATE);
	scrollbar.setMin(-MAX_FRAMERATE);
	scrollbar.setPos(DEFAULT_FRAMERATE);
	scrollbar.setSmallStep(1);
end

function updateToolBox()
	local env = Device.getGUIEnvironment();
	local root = env.getRootGUIElement();
	local dlg = root.getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
	if dlg == nil  then
		return;
	end

	local aniInfo = cpgf.cast(dlg.getElementFromId(GUI_ID_ANIMATION_INFO, true), irr.IGUIStaticText);
	if aniInfo ~= nil then
		if  Model ~= nil and irr.ESNT_ANIMATED_MESH == Model.getType()  then
			local animatedModel = cpgf.cast(Model, irr.IAnimatedMeshSceneNode);

			local str = "" .. animatedModel.getAnimationSpeed();
			str = str .. " Frame: ";
			str = str .. animatedModel.getFrameNr();
			aniInfo.setText(str);
		else
			aniInfo.setText("");
		end
	end
end

function hasModalDialog()
	local env = Device.getGUIEnvironment();
	local focused = env.getFocus();
	while  focused ~= nil  do
		if  focused.isVisible() and focused.hasType(irr.EGUIET_MODAL_SCREEN)  then
			return true;
		end
		focused = focused.getParent();
	end
	return false;
end

function overrideEventReceiver(receiver)
	local OnKeyUp = function(keyCode)
		if  hasModalDialog()  then
			return false;
		end

		if keyCode == irr.KEY_ESCAPE then
			if Device ~= nil then
				local camera = Device.getSceneManager().getActiveCamera();
				if camera then
					camera.setInputReceiverEnabled( not camera.isInputReceiverEnabled() );
				end
				return true;
			end
		elseif keyCode == irr.KEY_F1 then
			if Device ~= nil then
				local elem = Device.getGUIEnvironment().getRootGUIElement().getElementFromId(GUI_ID_POSITION_TEXT);
				if elem then
					elem.setVisible(not elem.isVisible());
				end
			end
		elseif keyCode == irr.KEY_KEY_M then
			if Device ~= nil then
				Device.minimizeWindow();
			end
		elseif keyCode == irr.KEY_KEY_L then
			UseLight= not UseLight;
			if Model ~= nil then
				Model.setMaterialFlag(irr.EMF_LIGHTING, UseLight);
				Model.setMaterialFlag(irr.EMF_NORMALIZE_NORMALS, UseLight);
			end
		end
		return false;
	end

	local simpleXor = function(a, b)
		if a == b then
			return 0;
		else
			return b;
		end
	end

	local OnMenuItemSelected = function(menu)
		local id = menu.getItemCommandId(menu.getSelectedItem());
		local env = Device.getGUIEnvironment();

		if id == GUI_ID_OPEN_MODEL then
			env.addFileOpenDialog("Please select a model file to open");
			
		elseif id == GUI_ID_SET_MODEL_ARCHIVE then
			env.addFileOpenDialog("Please select your game archive/directory");
			
		elseif id == GUI_ID_LOAD_AS_OCTREE then
			Octree = not Octree;
			menu.setItemChecked(menu.getSelectedItem(), Octree);
			
		elseif id == GUI_ID_QUIT then
			Device.closeDevice();
			
		elseif id == GUI_ID_SKY_BOX_VISIBLE then
			menu.setItemChecked(menu.getSelectedItem(), not menu.isItemChecked(menu.getSelectedItem()));
			SkyBox.setVisible(not SkyBox.isVisible());
			
		elseif id == GUI_ID_DEBUG_OFF then
			menu.setItemChecked(menu.getSelectedItem()+1, false);
			menu.setItemChecked(menu.getSelectedItem()+2, false);
			menu.setItemChecked(menu.getSelectedItem()+3, false);
			menu.setItemChecked(menu.getSelectedItem()+4, false);
			menu.setItemChecked(menu.getSelectedItem()+5, false);
			menu.setItemChecked(menu.getSelectedItem()+6, false);
			if Model ~= nil then
				Model.setDebugDataVisible(irr.EDS_OFF);
			end
			
		elseif id == GUI_ID_DEBUG_BOUNDING_BOX then
			menu.setItemChecked(menu.getSelectedItem(), not menu.isItemChecked(menu.getSelectedItem()));
			if Model ~= nil then
				Model.setDebugDataVisible(simpleXor(Model.isDebugDataVisible(), irr.EDS_BBOX));
			end
			
		elseif id == GUI_ID_DEBUG_NORMALS then
			menu.setItemChecked(menu.getSelectedItem(), not menu.isItemChecked(menu.getSelectedItem()));
			if Model ~= nil then
				Model.setDebugDataVisible(simpleXor(Model.isDebugDataVisible(), irr.EDS_NORMALS));
			end
			
		elseif id == GUI_ID_DEBUG_SKELETON then
			menu.setItemChecked(menu.getSelectedItem(), not menu.isItemChecked(menu.getSelectedItem()));
			if Model ~= nil then
				Model.setDebugDataVisible(simpleXor(Model.isDebugDataVisible(), irr.EDS_SKELETON));
			end
			
		elseif id == GUI_ID_DEBUG_WIRE_OVERLAY then
			menu.setItemChecked(menu.getSelectedItem(), not menu.isItemChecked(menu.getSelectedItem()));
			if Model ~= nil then
				Model.setDebugDataVisible(simpleXor(Model.isDebugDataVisible(), irr.EDS_MESH_WIRE_OVERLAY));
			end
		elseif id == GUI_ID_DEBUG_HALF_TRANSPARENT then
			menu.setItemChecked(menu.getSelectedItem(), not menu.isItemChecked(menu.getSelectedItem()));
			if Model ~= nil then
				Model.setDebugDataVisible(simpleXor(Model.isDebugDataVisible(), irr.EDS_HALF_TRANSPARENCY));
			end
		elseif id == GUI_ID_DEBUG_BUFFERS_BOUNDING_BOXES then
			menu.setItemChecked(menu.getSelectedItem(), not menu.isItemChecked(menu.getSelectedItem()));
			if Model ~= nil then
				Model.setDebugDataVisible(simpleXor(Model.isDebugDataVisible(), irr.EDS_BBOX_BUFFERS));
			end
		elseif id == GUI_ID_DEBUG_ALL then
			menu.setItemChecked(menu.getSelectedItem()-1, true);
			menu.setItemChecked(menu.getSelectedItem()-2, true);
			menu.setItemChecked(menu.getSelectedItem()-3, true);
			menu.setItemChecked(menu.getSelectedItem()-4, true);
			menu.setItemChecked(menu.getSelectedItem()-5, true);
			menu.setItemChecked(menu.getSelectedItem()-6, true);
			if Model ~= nil then
				Model.setDebugDataVisible(irr.EDS_FULL);
			end
		elseif id == GUI_ID_ABOUT then
			showAboutText();
		elseif id == GUI_ID_MODEL_MATERIAL_SOLID then
			if Model ~= nil then
				Model.setMaterialType(irr.EMT_SOLID);
			end
		elseif id == GUI_ID_MODEL_MATERIAL_TRANSPARENT then
			if Model ~= nil then
				Model.setMaterialType(irr.EMT_TRANSPARENT_ADD_COLOR);
			end
		elseif id == GUI_ID_MODEL_MATERIAL_REFLECTION then
			if Model ~= nil then
				Model.setMaterialType(irr.EMT_SPHERE_MAP);
			end

		elseif id == GUI_ID_CAMERA_MAYA then
			setActiveCamera(Camera[0]);
		elseif id == GUI_ID_CAMERA_FIRST_PERSON then
			setActiveCamera(Camera[1]);
		end
	end

	local OnTextureFilterSelected = function(combo)
		local pos = combo.getSelected();
		if pos == 0 then
			if Model ~= nil then
				Model.setMaterialFlag(irr.EMF_BILINEAR_FILTER, false);
				Model.setMaterialFlag(irr.EMF_TRILINEAR_FILTER, false);
				Model.setMaterialFlag(irr.EMF_ANISOTROPIC_FILTER, false);
			end
		elseif pos == 1 then
			if Model ~= nil then
				Model.setMaterialFlag(irr.EMF_BILINEAR_FILTER, true);
				Model.setMaterialFlag(irr.EMF_TRILINEAR_FILTER, false);
			end
		elseif pos == 2 then
			if Model ~= nil then
				Model.setMaterialFlag(irr.EMF_BILINEAR_FILTER, false);
				Model.setMaterialFlag(irr.EMF_TRILINEAR_FILTER, true);
			end
		elseif pos == 3 then
			if Model ~= nil then
				Model.setMaterialFlag(irr.EMF_ANISOTROPIC_FILTER, true);
			end
		elseif pos == 4 then
			if Model ~= nil then
				Model.setMaterialFlag(irr.EMF_ANISOTROPIC_FILTER, false);
			end
		end
	end

	receiver.OnEvent = function(me, event)
		if event.EventType == irr.EET_KEY_INPUT_EVENT and event.KeyInput.PressedDown == false then
			if  OnKeyUp(event.KeyInput.Key)  then
				return true;
			end
		end

		if event.EventType == irr.EET_GUI_EVENT then
			local id = event.GUIEvent.Caller.getID();
			local env = Device.getGUIEnvironment();

			if event.GUIEvent.EventType == irr.EGET_MENU_ITEM_SELECTED then
				OnMenuItemSelected( cpgf.cast(event.GUIEvent.Caller, irr.IGUIContextMenu) );

			elseif event.GUIEvent.EventType == irr.EGET_FILE_SELECTED then
				local dialog = cpgf.cast(event.GUIEvent.Caller, irr.IGUIFileOpenDialog);
				loadModel(dialog.getFileName());

			elseif event.GUIEvent.EventType == irr.EGET_SCROLL_BAR_CHANGED then
				if id == GUI_ID_SKIN_TRANSPARENCY then
					local pos = cpgf.cast(event.GUIEvent.Caller, irr.IGUIScrollBar).getPos();
					setSkinTransparency(pos, env.getSkin());
				elseif id == GUI_ID_SKIN_ANIMATION_FPS then
					local pos = cpgf.cast(event.GUIEvent.Caller, irr.IGUIScrollBar).getPos();
					if Model ~= nil and irr.ESNT_ANIMATED_MESH == Model.getType() then
						cpgf.cast(Model, irr.IAnimatedMeshSceneNode).setAnimationSpeed(pos);
					end
				end

			elseif event.GUIEvent.EventType == irr.EGET_COMBO_BOX_CHANGED then
				if id == GUI_ID_TEXTUREFILTER then
					OnTextureFilterSelected( cpgf.cast(event.GUIEvent.Caller, irr.IGUIComboBox) );
				end

			elseif event.GUIEvent.EventType == irr.EGET_BUTTON_CLICKED then
				if id == GUI_ID_BUTTON_SET_SCALE then
					local root = env.getRootGUIElement();
					local scale = irr.vector3df();
					local s;

					s = root.getElementFromId(GUI_ID_X_SCALE, true).getText();
					scale.X = tonumber(s);
					s = root.getElementFromId(GUI_ID_Y_SCALE, true).getText();
					scale.Y = tonumber(s);
					s = root.getElementFromId(GUI_ID_Z_SCALE, true).getText();
					scale.Z = tonumber(s);

					if Model ~= nil then
						Model.setScale(scale);
					end
					updateScaleInfo(Model);
				elseif id == GUI_ID_BUTTON_SCALE_MUL10 then
					if Model ~= nil then
						local scale = irr.vector3df(Model.getScale());
						scale.X = scale.X * 10;
						scale.Y = scale.Y * 10;
						scale.Z = scale.Z * 10;
						Model.setScale(scale);
					end
					updateScaleInfo(Model);

				elseif id == GUI_ID_BUTTON_SCALE_DIV10 then
					if Model ~= nil then
						local scale = irr.vector3df(Model.getScale());
						scale.X = scale.X * 0.1;
						scale.Y = scale.Y * 0.1;
						scale.Z = scale.Z * 0.1;
						Model.setScale(scale);
					end
					updateScaleInfo(Model);

				elseif id == GUI_ID_BUTTON_OPEN_MODEL then
					env.addFileOpenDialog("Please select a model file to open");

				elseif id == GUI_ID_BUTTON_SHOW_ABOUT then
					showAboutText();

				elseif id == GUI_ID_BUTTON_SHOW_TOOLBOX then
					createToolBox();

				elseif id == GUI_ID_BUTTON_SELECT_ARCHIVE then
					env.addFileOpenDialog("Please select your game archive/directory");
				end
			end
		end

		return false;
	end


end


function start()
	local driverType=irr.driverChoiceConsole();
	if driverType==irr.EDT_COUNT then
		return 1;
	end

	local receiver = irr.IEventReceiverWrapper();
	overrideEventReceiver(receiver);
	Device = irr.createDevice(driverType, irr.dimension2d_u32(800, 600), 16, false, false, false, receiver);

	if Device == 0 then
		return 1;
	end

	Device.setResizable(true);

	Device.setWindowCaption("cpgf Irrlicht Lua Binding - Loading...");

	local driver = Device.getVideoDriver();
	local env = Device.getGUIEnvironment();
	local smgr = Device.getSceneManager();
	smgr.getParameters().setAttribute(irr.COLLADA_CREATE_SCENE_INSTANCES, true);

	driver.setTextureCreationFlag(irr.ETCF_ALWAYS_32_BIT, true);

	smgr.addLightSceneNode(nil, irr.vector3df(200,200,200), irr.SColorf(1.0,1.0,1.0),2000);
	smgr.setAmbientLight(irr.SColorf(0.3,0.3,0.3));
	Device.getFileSystem().addFolderFileArchive("../../media/");

	local xml = Device.getFileSystem().createXMLReader( "config.xml");

	while xml ~= nil and xml.read() do
		local t = xml.getNodeType()
		if t == irr.EXN_TEXT then
			MessageText = xml.getNodeData();
			
		elseif t == irr.EXN_ELEMENT then
			if "startUpModel" == xml.getNodeName() then
				StartUpModelFile = xml.getAttributeValue("file");
			elseif "messageText" == xml.getNodeName() then
				Caption = xml.getAttributeValue("caption");
			end
		end
	end

	if xml then
		xml.drop();
	end

	local skin = env.getSkin();
	local font = env.getFont("fonthaettenschweiler.bmp");
	if font ~= nil then
		skin.setFont(font);
	end

	local menu = env.addMenu();
	menu.addItem("File", -1, true, true);
	menu.addItem("View", -1, true, true);
	menu.addItem("Camera", -1, true, true);
	menu.addItem("Help", -1, true, true);

	local submenu;
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

	local bar = env.addToolBar();

	local image = driver.getTexture("open.png");
	bar.addButton(GUI_ID_BUTTON_OPEN_MODEL, nil, "Open a mode",image, nil, false, true);

	image = driver.getTexture("tools.png");
	bar.addButton(GUI_ID_BUTTON_SHOW_TOOLBOX, nil, "Open Toolset",image, nil, false, true);

	image = driver.getTexture("zip.png");
	bar.addButton(GUI_ID_BUTTON_SELECT_ARCHIVE, nil, "Set Model Archive",image, nil, false, true);

	image = driver.getTexture("help.png");
	bar.addButton(GUI_ID_BUTTON_SHOW_ABOUT, nil, "Open Help", image, nil, false, true);


	local box = env.addComboBox(irr.rect_s32(250,4,350,23), bar, GUI_ID_TEXTUREFILTER);
	box.addItem("No filtering");
	box.addItem("Bilinear");
	box.addItem("Trilinear");
	box.addItem("Anisotropic");
	box.addItem("Isotropic");

	for i=0, irr.EGDC_COUNT - 1 do
		local col = env.getSkin().getColor(i);
		col.setAlpha(255);
		env.getSkin().setColor(i, col);
	end


	createToolBox();

	local fpstext = env.addStaticText("",	irr.rect_s32(400,4,570,23), true, false, bar);

	local postext = env.addStaticText("",	irr.rect_s32(10,50,470,80),false, false, nil, GUI_ID_POSITION_TEXT);
	postext.setVisible(false);


	Caption = Caption .. " - [";
	Caption = Caption .. driver.getName();
	Caption = Caption .. "]";
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

	local img = env.addImage(driver.getTexture("irrlichtlogo2.png"), irr.position2d_s32(10, driver.getScreenSize().Height - 128));

	img.setAlignment(irr.EGUIA_UPPERLEFT, irr.EGUIA_UPPERLEFT, irr.EGUIA_LOWERRIGHT, irr.EGUIA_LOWERRIGHT);


	while Device.run() and driver do
		if Device.isWindowActive() then
			driver.beginScene(true, true, irr.SColor(150,50,50,50));

			smgr.drawAll();
			env.drawAll();

			driver.endScene();

			local str = "FPS: ";
			str = str .. ("" .. driver.getFPS());
			str = str .. " Tris: ";
			str = str .. ("" .. driver.getPrimitiveCountDrawn());
			fpstext.setText(str);

			local cam = Device.getSceneManager().getActiveCamera();
			str = "Pos: ";
			str = str .. cam.getPosition().X;
			str = str .. " ";
			str = str .. cam.getPosition().Y;
			str = str .. " ";
			str = str .. cam.getPosition().Z;
			str = str .. " Tgt: ";
			str = str .. cam.getTarget().X;
			str = str .. " ";
			str = str .. cam.getTarget().Y;
			str = str .. " ";
			str = str .. cam.getTarget().Z;
			postext.setText(str);

			updateToolBox();
		end
	end

	Device.drop();
	return 0;
end

start();
