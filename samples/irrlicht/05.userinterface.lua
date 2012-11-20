cpgf.import(nil, "builtin.debug");
cpgf.import(nil, "builtin.core");

Context = {
	device = nil,
	counter = 0,
	listbox = nil
}

local GUI_ID_QUIT_BUTTON = 101;
local GUI_ID_NEW_WINDOW_BUTTON = 102;
local GUI_ID_FILE_OPEN_BUTTON = 103;
local GUI_ID_TRANSPARENCY_SCROLL_BAR = 104;

function makeMyEventReceiver(receiver)
	receiver.OnEvent = function(me, event)
		if event.EventType == irr.EET_GUI_EVENT then
			local id = event.GUIEvent.Caller.getID();
			local env = Context.device.getGUIEnvironment();

			if event.GUIEvent.EventType == irr.EGET_SCROLL_BAR_CHANGED then
				if id == GUI_ID_TRANSPARENCY_SCROLL_BAR then

					local pos = cpgf.cast(event.GUIEvent.Caller, irr.IGUIScrollBar).getPos();
					local skin = env.getSkin();
					
					for i = 0, irr.EGDC_COUNT - 1 do
						local col = skin.getColor(i);
						col.setAlpha(pos);
						skin.setColor(i, col);
					end
					
				end

			elseif event.GUIEvent.EventType == irr.EGET_BUTTON_CLICKED then
				if id == GUI_ID_QUIT_BUTTON then
					Context.device.closeDevice();
					return true;
					
				elseif id == GUI_ID_NEW_WINDOW_BUTTON then
					Context.listbox.addItem("Window created");
					Context.counter = Context.counter + 30;
					if Context.counter > 200 then
						Context.counter = 0;
					end
					
					local window = env.addWindow(irr.rect_s32(100 + Context.counter, 100 + Context.counter, 300 + Context.counter, 200 + Context.counter), false, "Test window");
					env.addStaticText("Please close me", irr.rect_s32(35,35,140,50), true, false, window);
					
					return true;

				elseif id == GUI_ID_FILE_OPEN_BUTTON then
					Context.listbox.addItem("File open");
					env.addFileOpenDialog("Please choose a file.");
					return true;
				end
			end
		end

		return false;
	end
end

function start()
	local driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT then
		return 1;
	end

	local device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480));

	if device == nil then
		return 1;
	end

	device.setWindowCaption("cpgf Irrlicht Lua Binding - User Interface Demo");
	device.setResizable(true);

	local driver = device.getVideoDriver();
	local env = device.getGUIEnvironment();

	local skin = env.getSkin();
	local font = env.getFont("../../media/fonthaettenschweiler.bmp");
	if font then
		skin.setFont(font);
	end

	skin.setFont(env.getBuiltInFont(), irr.EGDF_TOOLTIP);

	env.addButton(irr.rect_s32(10,240,110,240 + 32), nil, GUI_ID_QUIT_BUTTON, "Quit", "Exits Program");
	env.addButton(irr.rect_s32(10,280,110,280 + 32), nil, GUI_ID_NEW_WINDOW_BUTTON, "New Window", "Launches a Window");
	env.addButton(irr.rect_s32(10,320,110,320 + 32), nil, GUI_ID_FILE_OPEN_BUTTON, "File Open", "Opens a file");

	env.addStaticText("Transparent Control:", irr.rect_s32(150,20,350,40), true);
	scrollbar = env.addScrollBar(true, irr.rect_s32(150, 45, 350, 60), nil, GUI_ID_TRANSPARENCY_SCROLL_BAR);
	scrollbar.setMax(255);

	scrollbar.setPos(env.getSkin().getColor(irr.EGDC_WINDOW).getAlpha());

	env.addStaticText("Logging ListBox:", irr.rect_s32(50,110,250,130), true);
	local listbox = env.addListBox(irr.rect_s32(50, 140, 250, 210));
	env.addEditBox("Editable Text", irr.rect_s32(350, 80, 550, 100));

	Context.device = device;
	Context.counter = 0;
	Context.listbox = listbox;

	local MyEventReceiver = cpgf.cloneClass(irr.IEventReceiverWrapper);
	makeMyEventReceiver(MyEventReceiver);
	local receiver = MyEventReceiver();

	device.setEventReceiver(receiver);

	env.addImage(driver.getTexture("../../media/irrlichtlogo2.png"), irr.position2d_s32(10,10));

	while device.run() and driver do
		if device.isWindowActive() then
			driver.beginScene(true, true, irr.SColor(0,200,200,200));
			env.drawAll();
			driver.endScene();
		end
	end

	device.drop();
	
	return 0;
end

start();
