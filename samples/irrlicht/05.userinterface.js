cpgf.import(null, "builtin.debug");
cpgf.import(null, "builtin.core");

function SAppContext()
{
	this.device = null;
	this.counter = 0;
	this.listbox = null;
}

var GUI_ID_QUIT_BUTTON = 101;
var GUI_ID_NEW_WINDOW_BUTTON = 102;
var GUI_ID_FILE_OPEN_BUTTON = 103;
var GUI_ID_TRANSPARENCY_SCROLL_BAR = 104;

/*
The Event Receiver is not only capable of getting keyboard and
mouse input events, but also events of the graphical user interface
(gui). There are events for almost everything: Button click,
Listbox selection change, events that say that a element was hovered
and so on. To be able to react to some of these events, we create
an event receiver.
We only react to gui events, and if it's such an event, we get the
id of the caller (the gui element which caused the event) and get
the pointer to the gui environment.
*/
function makeMyEventReceiver(receiver, context)
{
	this.Context = context;
	var self = this;

	receiver.OnEvent = function(me, event) {
		if (event.EventType == irr.EET_GUI_EVENT)
		{
			var id = event.GUIEvent.Caller.getID();
			var env = self.Context.device.getGUIEnvironment();

			switch(event.GUIEvent.EventType)
			{

			/*
			If a scrollbar changed its scroll position, and it is
			'our' scrollbar (the one with id GUI_ID_TRANSPARENCY_SCROLL_BAR), then we change
			the transparency of all gui elements. This is a very
			easy task: There is a skin object, in which all color
			settings are stored. We simply go through all colors
			stored in the skin and change their alpha value.
			*/
			case irr.EGET_SCROLL_BAR_CHANGED:
				if (id == GUI_ID_TRANSPARENCY_SCROLL_BAR)
				{
					var pos = cpgf.cast(event.GUIEvent.Caller, irr.IGUIScrollBar).getPos();
					var skin = env.getSkin();
					
					for (var i=0; i<irr.EGDC_COUNT ; ++i)
					{
						var col = skin.getColor(i);
						col.setAlpha(pos);
						skin.setColor(i, col);
					}
					
				}
				break;

			/*
			If a button was clicked, it could be one of 'our'
			three buttons. If it is the first, we shut down the engine.
			If it is the second, we create a little window with some
			text on it. We also add a string to the list box to log
			what happened. And if it is the third button, we create
			a file open dialog, and add also this as string to the list box.
			That's all for the event receiver.
			*/
			case irr.EGET_BUTTON_CLICKED:
				switch(id)
				{
				case GUI_ID_QUIT_BUTTON:
					self.Context.device.closeDevice();
					return true;

				case GUI_ID_NEW_WINDOW_BUTTON:
					{
					self.Context.listbox.addItem("Window created");
					self.Context.counter += 30;
					if (self.Context.counter > 200)
						self.Context.counter = 0;

					var window = env.addWindow(new irr.rect_s32(100 + self.Context.counter, 100 + self.Context.counter, 300 + self.Context.counter, 200 + self.Context.counter), false, "Test window");

					env.addStaticText("Please close me", new irr.rect_s32(35,35,140,50), true, false, window);
					}
					return true;

				case GUI_ID_FILE_OPEN_BUTTON:
					self.Context.listbox.addItem("File open");
					env.addFileOpenDialog("Please choose a file.");
					return true;

				default:
					return false;
				}
				break;

			default:
				break;
			}
		}

		return false;
	}
}

function start()
{
	// ask user for driver
	var driverType = irr.driverChoiceConsole();
	if(driverType == irr.EDT_COUNT)
		return 1;


	// create device and exit if creation failed

	var device = irr.createDevice(driverType, new irr.dimension2d_u32(640, 480));

	if (device == null)
		return 1; // could not create selected driver.

	/* The creation was successful, now we set the event receiver and
		store pointers to the driver and to the gui environment. */

	device.setWindowCaption("cpgf Irrlicht JavaScript Binding - User Interface Demo");
	device.setResizable(true);

	var driver = device.getVideoDriver();
	var env = device.getGUIEnvironment();

	/*
	To make the font a little bit nicer, we load an external font
	and set it as the new default font in the skin.
	To keep the standard font for tool tip text, we set it to
	the built-in font.
	*/

	var skin = env.getSkin();
	var font = env.getFont("../../media/fonthaettenschweiler.bmp");
	if (font)
		skin.setFont(font);

	skin.setFont(env.getBuiltInFont(), irr.EGDF_TOOLTIP);

	/*
	We add three buttons. The first one closes the engine. The second
	creates a window and the third opens a file open dialog. The third
	parameter is the id of the button, with which we can easily identify
	the button in the event receiver.
	*/	

	env.addButton(new irr.rect_s32(10,240,110,240 + 32), 0, GUI_ID_QUIT_BUTTON, "Quit", "Exits Program");
	env.addButton(new irr.rect_s32(10,280,110,280 + 32), 0, GUI_ID_NEW_WINDOW_BUTTON, "New Window", "Launches a new Window");
	env.addButton(new irr.rect_s32(10,320,110,320 + 32), 0, GUI_ID_FILE_OPEN_BUTTON, "File Open", "Opens a file");

	/*
	Now, we add a static text and a scrollbar, which modifies the
	transparency of all gui elements. We set the maximum value of
	the scrollbar to 255, because that's the maximal value for
	a color value.
	Then we create an other static text and a list box.
	*/

	env.addStaticText("Transparent Control:", new irr.rect_s32(150,20,350,40), true);
	scrollbar = env.addScrollBar(true, new irr.rect_s32(150, 45, 350, 60), 0, GUI_ID_TRANSPARENCY_SCROLL_BAR);
	scrollbar.setMax(255);

	// set scrollbar position to alpha value of an arbitrary element
	scrollbar.setPos(env.getSkin().getColor(irr.EGDC_WINDOW).getAlpha());

	env.addStaticText("Logging ListBox:", new irr.rect_s32(50,110,250,130), true);
	var listbox = env.addListBox(new irr.rect_s32(50, 140, 250, 210));
	env.addEditBox("Editable Text", new irr.rect_s32(350, 80, 550, 100));

	// Store the appropriate data in a context structure.
	var context = new SAppContext();
	context.device = device;
	context.counter = 0;
	context.listbox = listbox;

	// Then create the event receiver, giving it that context structure.
	var MyEventReceiver = cpgf.cloneClass(irr.IEventReceiverWrapper);
	makeMyEventReceiver(MyEventReceiver, context);
	var receiver = new MyEventReceiver();

	// And tell the device to use our custom event receiver.
	device.setEventReceiver(receiver);


	/*
	And at last, we create a nice Irrlicht Engine logo in the top left corner. 
	*/
	env.addImage(driver.getTexture("../../media/irrlichtlogo2.png"), new irr.position2d_s32(10,10));


	/*
	That's all, we only have to draw everything.
	*/

	while(device.run() && driver)
	if (device.isWindowActive())
	{
		driver.beginScene(true, true, new irr.SColor(0,200,200,200));

		env.drawAll();
	
		driver.endScene();
	}

	device.drop();

	return 0;
}

start();
