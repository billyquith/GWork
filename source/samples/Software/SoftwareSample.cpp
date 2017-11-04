/*
 *  Gwork
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Renderers/Software.h>
#include <Gwork/Platform.h>
#include <Gwork/Test/TestAPI.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Gwork/External/stb_image_write.h>

#define VIEWER 1    // turn on for interactive viewer

#if VIEWER
#   include <Gwork/Input/SDL2.h>
#endif

int main(int argc, char** argv)
{
    // Setup our window and renderer
    const Gwk::Point screenSize(1024, 768);

#if VIEWER
    // We'll use SDL2 for the window and rendering management
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
        return EXIT_FAILURE;
    
    SDL_Window *window = SDL_CreateWindow("Gwork Software Sample", 100, 100,
                                          screenSize.x, screenSize.y, SDL_WINDOW_SHOWN);
    if (!window)
        return EXIT_FAILURE;
    
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
#endif

    Gwk::Platform::RelativeToExecutablePaths paths(GWORK_RESOURCE_DIR);
    Gwk::Renderer::SoftwareResourceLoader loader(paths);
    
    Gwk::Renderer::PixelBuffer pixbuff;
    pixbuff.Init(screenSize);

    Gwk::Renderer::Software *renderer = new Gwk::Renderer::Software(loader, pixbuff);

    // Create a Gwork skin
    Gwk::Skin::TexturedBase skin(renderer);
    skin.SetRender(renderer);
    skin.Init("DefaultSkin.png");

    // Note, you can get fonts that cover many languages/locales to do Chinese,
    //       Arabic, Korean, etc. e.g. "Arial Unicode" (but it's 23MB!).
    skin.SetDefaultFont("OpenSans.ttf", 11);

    // Create a Canvas (it's root, on which all other Gwork panels are created)
    Gwk::Controls::Canvas* canvas = new Gwk::Controls::Canvas(&skin);
    canvas->SetSize(screenSize);
    canvas->SetDrawBackground(true);
    canvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));

    // Create our unittest control (which is a Window with controls in it)
    TestAPI* unit = new TestAPI(canvas);
    unit->SetPos(10, 10);
    
#if VIEWER
    Gwk::Input::SDL2 input;
    input.Initialize(canvas);
    auto *texture = SDL_CreateTexture(sdlRenderer,
                                      SDL_PIXELFORMAT_ABGR8888,
                                      SDL_TEXTUREACCESS_STREAMING,
                                      screenSize.x, screenSize.y);
    bool haveQuit = false;
    while (!haveQuit)
    {
        SDL_Event evt;
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT)
                haveQuit = true;
            
            input.ProcessEvent(&evt);
        }
        
        // render the GUI to our buffer
        renderer->BeginContext(nullptr);
        canvas->RenderCanvas();
        renderer->PresentContext(nullptr);
        renderer->EndContext(nullptr);
        
        // show the software rendered GUI on the screen
        SDL_UpdateTexture(texture, NULL, &pixbuff.At(0,0), screenSize.x*4);
        SDL_RenderClear(sdlRenderer);
        SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(5);
    }
#else
    // render GUI to buffer
    renderer->BeginContext(nullptr);
    canvas->RenderCanvas();
    renderer->PresentContext(nullptr);
    renderer->EndContext(nullptr);
    
    // write screen shot
    stbi_write_png("sw.png", screenSize.x, screenSize.y, 4, &pixbuff.At(0,0), 4*screenSize.x);
#endif
    
//    skin.ReleaseFont(skin.GetDefaultFont());
//    delete unit;
//    delete renderer;
    
#if VIEWER
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
#endif
    
    return EXIT_SUCCESS;
}
