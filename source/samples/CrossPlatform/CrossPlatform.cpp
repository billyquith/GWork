/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Gwork.h>
#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>
#include <Gwork/Renderers/OpenGL.h>
#include <Gwork/Controls/WindowCanvas.h>

//
// These are the renderers that work with WindowCanvas
// (you need to include the relevant libs to use them)
//
#include <Gwork/Renderers/OpenGL_DebugFont.h>
// #include <Gwork/Renderers/GDIPlus.h>
// #include <Gwork/Renderers/Direct2D.h>
// #include <Gwork/Renderers/DirectX9.h>


//
// Program starts here
//
int main()
{
    //
    // Note: Order is important here. you need to create the WindowCanvas before
    // you setup the skin - because the renderer won't be properly set up
    // if you try to do it before (The window canvas initializes the renderer)
    //
    // Create the skin and renderer
    //
    // Gwk::Renderer::GDIPlusBuffered		renderer;
    // Gwk::Renderer::Direct2D				renderer;
    // Gwk::Renderer::DirectX9				renderer;
    Gwk::Renderer::OpenGL_DebugFont renderer;
    Gwk::Skin::TexturedBase skin(&renderer);
    //
    // The window canvas is a cross between a window and a canvas
    // It's cool because it takes care of creating an OS specific
    // window - so we don't have to bother with all that crap.
    //
    //
    Gwk::Controls::WindowCanvas window_canvas(-1, -1, 700, 500, &skin,
                                               "Gwork's Cross Platform Example");
    //
    // Now it's safe to set up the skin
    //
    skin.Init("DefaultSkin.png");
    //
    // Create our unittest control
    //
    UnitTest* unit = new UnitTest(&window_canvas);
    unit->SetPos(10, 10);

    while (!window_canvas.WantsQuit())
    {
        window_canvas.DoThink();
    }

    // Everything should be automatically released
    // unit is a child of Canvas - which releases all of its children
}
