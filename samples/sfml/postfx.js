doMain();

function doMain()
{
    // Check that the system can use post effects
    if(! sfml.PostFX.CanUsePostFX())
    {
        return sfml.EXIT_SUCCESS;
    }

    // Create the main window
    var App = new sfml.RenderWindow( new sfml.VideoMode(800, 600), "SFML PostFX -- cpgf SFML binding demo -- V8 Javascript");

    // Load a cute background image to display :)
    var BackgroundImage = new sfml.Image();
    if(!BackgroundImage.LoadFromFile("datas/post-fx/background.jpg"))
        return sfml.EXIT_FAILURE;
    var Background = new sfml.Sprite(BackgroundImage);

    // Load the text font
    var Cheeseburger = new sfml.Font();
    if(!Cheeseburger.LoadFromFile("datas/post-fx/cheeseburger.ttf"))
        return sfml.EXIT_FAILURE;

    // Load the image needed for the wave effect
    var WaveImage = new sfml.Image();
    if (!WaveImage.LoadFromFile("datas/post-fx/wave.jpg"))
        return sfml.EXIT_FAILURE;

    // Load all effects
    var Effects = {
    	"nothing" : new sfml.PostFX(),
    	"blur" : new sfml.PostFX(),
    	"colorize" : new sfml.PostFX(),
    	"fisheye" : new sfml.PostFX(),
    	"wave" : new sfml.PostFX(),
    };
    if (!Effects["nothing"].LoadFromFile("datas/post-fx/nothing.sfx"))   return sfml.EXIT_FAILURE;
    if (!Effects["blur"].LoadFromFile("datas/post-fx/blur.sfx"))         return sfml.EXIT_FAILURE;
    if (!Effects["colorize"].LoadFromFile("datas/post-fx/colorize.sfx")) return sfml.EXIT_FAILURE;
    if (!Effects["fisheye"].LoadFromFile("datas/post-fx/fisheye.sfx"))   return sfml.EXIT_FAILURE;
    if (!Effects["wave"].LoadFromFile("datas/post-fx/wave.sfx"))         return sfml.EXIT_FAILURE;

    var EffectIndexes = [
    	"nothing",
    	"blur",
    	"colorize",
    	"fisheye",
    	"wave",
    ];

	var CurrentEffect = EffectIndexes.length - 1;

    // Do specific initializations
    Effects["nothing"].SetTexture("framebuffer", null);
    Effects["blur"].SetTexture("framebuffer", null);
    Effects["blur"].SetParameter("offset", 0.0);
    Effects["colorize"].SetTexture("framebuffer", null);
    Effects["colorize"].SetParameter("color", 1.0, 1.0, 1.0);
    Effects["fisheye"].SetTexture("framebuffer", null);
    Effects["wave"].SetTexture("framebuffer", null);
    Effects["wave"].SetTexture("wave", WaveImage);

    // Define a string for displaying current effect description
    var CurFXStr = new sfml.String();
    CurFXStr.SetText("Current effect is \"" + EffectIndexes[CurrentEffect] + "\"");
    CurFXStr.SetFont(Cheeseburger);
    CurFXStr.SetPosition(20.0, 0.0);

    // Define a string for displaying help
    var InfoStr = new sfml.String();
    InfoStr.SetText("Move your mouse to change the effect parameters\nPress numpad + and - to change effect\nWarning : some effects may not work\ndepending on your graphics card");
    InfoStr.SetFont(Cheeseburger);
    InfoStr.SetPosition(20.0, 460.0);
    InfoStr.SetColor(new sfml.Color(200, 100, 150));

    // Start the game loop
    while (App.IsOpened())
    {
        // Process events
        var Event = new sfml.Event();
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sfml.Event.Closed)
                App.Close();

            if (Event.Type == sfml.Event.KeyPressed)
            {
                // Escape key : exit
                if (Event.Key.Code == sfml.Escape)
                    App.Close();

                // Add key : next effect
                if (Event.Key.Code == sfml.Add)
                {
                    CurrentEffect++;
                    if (CurrentEffect == EffectIndexes.length)
                        CurrentEffect = 0;
                    CurFXStr.SetText("Current effect is \"" + EffectIndexes[CurrentEffect] + "\"");
                }

                // Subtract key : previous effect
                if (Event.Key.Code == sfml.Subtract)
                {
                    if (CurrentEffect == 0)
                        CurrentEffect = EffectIndexes.length;
                    CurrentEffect--;
                    CurFXStr.SetText("Current effect is \"" + EffectIndexes[CurrentEffect] + "\"");
                }
            }
        }

        // Get the mouse position in the range [0, 1]
        var X = App.GetInput().GetMouseX() / App.GetWidth();
        var Y = App.GetInput().GetMouseY() / App.GetHeight();

        // Update the current effect
        if      (EffectIndexes[CurrentEffect] == "blur")     Effects[EffectIndexes[CurrentEffect]].SetParameter("offset", X * Y * 0.1);
        else if (EffectIndexes[CurrentEffect] == "colorize") Effects[EffectIndexes[CurrentEffect]].SetParameter("color", 0.3, X, Y);
        else if (EffectIndexes[CurrentEffect] == "fisheye")  Effects[EffectIndexes[CurrentEffect]].SetParameter("mouse", X, 1.0 - Y);
        else if (EffectIndexes[CurrentEffect] == "wave")     Effects[EffectIndexes[CurrentEffect]].SetParameter("offset", X, Y);

        // Clear the window
        App.Clear();

        // Draw background and apply the post-fx
        App.Draw(Background);
        App.Draw(Effects[EffectIndexes[CurrentEffect]]);

        // Draw interface strings
        App.Draw(CurFXStr);
        App.Draw(InfoStr);

        // Finally, display the rendered frame on screen
        App.Display();

    }

    return sfml.EXIT_SUCCESS;
}
