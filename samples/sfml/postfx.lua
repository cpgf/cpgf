function doMain()
    -- Check that the system can use post effects
    if not sfml.PostFX.CanUsePostFX() then
        return sfml.EXIT_SUCCESS
    end

    -- Create the main window
    local App = sfml.RenderWindow(sfml.VideoMode(800, 600), "SFML PostFX -- cpgf SFML binding demo -- Lua")


    -- Load a cute background image to display
    local BackgroundImage = sfml.Image()
    if not BackgroundImage.LoadFromFile("datas/post-fx/background.jpg") then
        return sfml.EXIT_FAILURE
    end
    local Background = sfml.Sprite(BackgroundImage)

    -- Load the text font
    local Cheeseburger = sfml.Font()
    if not Cheeseburger.LoadFromFile("datas/post-fx/cheeseburger.ttf") then
        return sfml.EXIT_FAILURE
    end

    -- Load the image needed for the wave effect
    local WaveImage = sfml.Image()
    if not WaveImage.LoadFromFile("datas/post-fx/wave.jpg") then
        return sfml.EXIT_FAILURE
    end

    -- Load all effects
    local Effects = {
    	nothing = sfml.PostFX(),
    	blur = sfml.PostFX(),
    	colorize = sfml.PostFX(),
    	fisheye = sfml.PostFX(),
    	wave = sfml.PostFX()
    }
    if not Effects["nothing"].LoadFromFile("datas/post-fx/nothing.sfx") then   return sfml.EXIT_FAILURE end
    if not Effects["blur"].LoadFromFile("datas/post-fx/blur.sfx") then         return sfml.EXIT_FAILURE end
    if not Effects["colorize"].LoadFromFile("datas/post-fx/colorize.sfx") then return sfml.EXIT_FAILURE end
    if not Effects["fisheye"].LoadFromFile("datas/post-fx/fisheye.sfx") then   return sfml.EXIT_FAILURE end
    if not Effects["wave"].LoadFromFile("datas/post-fx/wave.sfx") then         return sfml.EXIT_FAILURE end

    local EffectIndexes = {
    	"nothing",
    	"blur",
    	"colorize",
    	"fisheye",
    	"wave"
    }

	local CurrentEffect = #EffectIndexes

    -- Do specific initializations
    Effects["nothing"].SetTexture("framebuffer", nil)
    Effects["blur"].SetTexture("framebuffer", nil)
    Effects["blur"].SetParameter("offset", 0.0)
    Effects["colorize"].SetTexture("framebuffer", nil)
    Effects["colorize"].SetParameter("color", 1.0, 1.0, 1.0)
    Effects["fisheye"].SetTexture("framebuffer", nil)
    Effects["wave"].SetTexture("framebuffer", nil)
    Effects["wave"].SetTexture("wave", WaveImage)

    -- Define a string for displaying current effect description
    local CurFXStr = sfml.String()
    CurFXStr.SetText("Current effect is \"" .. EffectIndexes[CurrentEffect] .. "\"")
    CurFXStr.SetFont(Cheeseburger)
    CurFXStr.SetPosition(20.0, 0.0)

    -- Define a string for displaying help
    local InfoStr = sfml.String()
    InfoStr.SetText("Move your mouse to change the effect parameters\nPress numpad + and - to change effect\nWarning : some effects may not work\ndepending on your graphics card")
    InfoStr.SetFont(Cheeseburger)
    InfoStr.SetPosition(20.0, 460.0)
    InfoStr.SetColor(sfml.Color(200, 100, 150))

    -- Start the game loop
    while App.IsOpened() do
        -- Process events
        local Event = sfml.Event()
        while App.GetEvent(Event) do
            -- Close window : exit
            if Event.Type == sfml.Event.Closed then
                App.Close()
            end

            if Event.Type == sfml.Event.KeyPressed then
                -- Escape key : exit
                if Event.Key.Code == sfml.Escape then
                    App.Close()
                end

                -- Add key : next effect
                if Event.Key.Code == sfml.Add then
                    CurrentEffect = CurrentEffect + 1
                    if CurrentEffect > #EffectIndexes then
                        CurrentEffect = 1
                    end
                    CurFXStr.SetText("Current effect is \"" .. EffectIndexes[CurrentEffect] .. "\"");
                end

                -- Subtract key : previous effect
                if Event.Key.Code == sfml.Subtract then
                    if CurrentEffect == 1 then
                        CurrentEffect = #EffectIndexes + 1
                    end
                    CurrentEffect = CurrentEffect - 1
                    CurFXStr.SetText("Current effect is \"" .. EffectIndexes[CurrentEffect] .. "\"");
                end
            end
        end

        -- Get the mouse position in the range [0, 1]
        local X = App.GetInput().GetMouseX() / App.GetWidth()
        local Y = App.GetInput().GetMouseY() / App.GetHeight()

        -- Update the current effect
        if EffectIndexes[CurrentEffect] == "blur" then     Effects[EffectIndexes[CurrentEffect]].SetParameter("offset", X * Y * 0.1) end
        if EffectIndexes[CurrentEffect] == "colorize" then Effects[EffectIndexes[CurrentEffect]].SetParameter("color", 0.3, X, Y) end
        if EffectIndexes[CurrentEffect] == "fisheye" then  Effects[EffectIndexes[CurrentEffect]].SetParameter("mouse", X, 1.0 - Y) end
        if EffectIndexes[CurrentEffect] == "wave" then     Effects[EffectIndexes[CurrentEffect]].SetParameter("offset", X, Y) end

        -- Clear the window
        App.Clear()

        -- Draw background and apply the post-fx
        App.Draw(Background)
        App.Draw(Effects[EffectIndexes[CurrentEffect]])

        -- Draw interface strings
        App.Draw(CurFXStr)
        App.Draw(InfoStr)

        -- Finally, display the rendered frame on screen
        App.Display()

    end


    return sfml.EXIT_SUCCESS
end


doMain()
