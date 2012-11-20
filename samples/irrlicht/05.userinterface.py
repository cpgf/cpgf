cpgf._import(None, "builtin.debug");
cpgf._import(None, "builtin.core");

class SAppContext:
	device = None,
	counter = 0,
	listbox = None

Context = SAppContext();	
	
GUI_ID_QUIT_BUTTON = 101;
GUI_ID_NEW_WINDOW_BUTTON = 102;
GUI_ID_FILE_OPEN_BUTTON = 103;
GUI_ID_TRANSPARENCY_SCROLL_BAR = 104;

def makeMyEventReceiver(receiver) :
	def OnEvent(me, event) :
		if event.EventType == irr.EET_GUI_EVENT :
			id = event.GUIEvent.Caller.getID();
			env = Context.device.getGUIEnvironment();

			if event.GUIEvent.EventType == irr.EGET_SCROLL_BAR_CHANGED :
				if id == GUI_ID_TRANSPARENCY_SCROLL_BAR :

					pos = cpgf.cast(event.GUIEvent.Caller, irr.IGUIScrollBar).getPos();
					skin = env.getSkin();
					
					for i in range(irr.EGDC_COUNT) :
						col = skin.getColor(i);
						col.setAlpha(pos);
						skin.setColor(i, col);

			elif event.GUIEvent.EventType == irr.EGET_BUTTON_CLICKED :
				if id == GUI_ID_QUIT_BUTTON :
					Context.device.closeDevice();
					return True;
					
				elif id == GUI_ID_NEW_WINDOW_BUTTON :
					Context.listbox.addItem("Window created");
					Context.counter = Context.counter + 30;
					if Context.counter > 200 :
						Context.counter = 0;
					
					window = env.addWindow(irr.rect_s32(100 + Context.counter, 100 + Context.counter, 300 + Context.counter, 200 + Context.counter), False, "Test window");
					env.addStaticText("Please close me", irr.rect_s32(35,35,140,50), True, False, window);
					
					return True;

				elif id == GUI_ID_FILE_OPEN_BUTTON :
					Context.listbox.addItem("File open");
					env.addFileOpenDialog("Please choose a file.");
					return True;


		return False;

	receiver.OnEvent = OnEvent;

def start() :
	driverType = irr.driverChoiceConsole();
	if driverType == irr.EDT_COUNT :
		return 1;


	device = irr.createDevice(driverType, irr.dimension2d_u32(640, 480));

	if device == None :
		return 1;


	device.setWindowCaption("cpgf Irrlicht Python Binding - User Interface Demo");
	device.setResizable(True);

	driver = device.getVideoDriver();
	env = device.getGUIEnvironment();

	skin = env.getSkin();
	font = env.getFont("../../media/fonthaettenschweiler.bmp");
	if font :
		skin.setFont(font);


	skin.setFont(env.getBuiltInFont(), irr.EGDF_TOOLTIP);

	env.addButton(irr.rect_s32(10,240,110,240 + 32), None, GUI_ID_QUIT_BUTTON, "Quit", "Exits Program");
	env.addButton(irr.rect_s32(10,280,110,280 + 32), None, GUI_ID_NEW_WINDOW_BUTTON, "New Window", "Launches a Window");
	env.addButton(irr.rect_s32(10,320,110,320 + 32), None, GUI_ID_FILE_OPEN_BUTTON, "File Open", "Opens a file");

	env.addStaticText("Transparent Control:", irr.rect_s32(150,20,350,40), True);
	scrollbar = env.addScrollBar(True, irr.rect_s32(150, 45, 350, 60), None, GUI_ID_TRANSPARENCY_SCROLL_BAR);
	scrollbar.setMax(255);

	scrollbar.setPos(env.getSkin().getColor(irr.EGDC_WINDOW).getAlpha());

	env.addStaticText("Logging ListBox:", irr.rect_s32(50,110,250,130), True);
	listbox = env.addListBox(irr.rect_s32(50, 140, 250, 210));
	env.addEditBox("Editable Text", irr.rect_s32(350, 80, 550, 100));

	Context.device = device;
	Context.counter = 0;
	Context.listbox = listbox;

	MyEventReceiver = cpgf.cloneClass(irr.IEventReceiverWrapper);
	makeMyEventReceiver(MyEventReceiver);
	receiver = MyEventReceiver();

	device.setEventReceiver(receiver);

	env.addImage(driver.getTexture("../../media/irrlichtlogo2.png"), irr.position2d_s32(10,10));

	while device.run() and driver :
		if device.isWindowActive() :
			driver.beginScene(True, True, irr.SColor(0,200,200,200));
			env.drawAll();
			driver.endScene();


	device.drop();

	return 0;


start();
