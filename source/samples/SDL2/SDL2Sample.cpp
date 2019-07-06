/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Gwork.h>
#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>
#include <Gwork/Input/SDL2.h>
#include <Gwork/Renderers/SDL2.h>
#include <SDL.h>
#include <SDL_ttf.h>


int main(int argc, char** argv)
{
    const Gwk::Point screenSize(1024, 768);

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
        return EXIT_FAILURE;

    if (TTF_Init() != 0)
        return EXIT_FAILURE;

    //Setup our window and renderer
    SDL_Window *window = SDL_CreateWindow("Gwork SDL2 Sample", 100, 100,
                                          screenSize.x, screenSize.y, SDL_WINDOW_SHOWN);
    if (!window)
        return EXIT_FAILURE;

    // Create a Gwork Allegro Renderer
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    {
        Gwk::Platform::RelativeToExecutablePaths paths(GWK_SAMPLE_RESOURCE_DIR);
        Gwk::Renderer::SDL2ResourceLoader loader(paths, sdlRenderer);
        
        std::unique_ptr<Gwk::Renderer::SDL2> renderer(new Gwk::Renderer::SDL2(loader, window));
        
        // Create a Gwork skin
        std::unique_ptr<Gwk::Skin::TexturedBase> skin(new Gwk::Skin::TexturedBase(renderer.get()));
        skin->SetRender(renderer.get());
        skin->Init("DefaultSkin.png");
        
        // Note, you can get fonts that cover many languages/locales to do Chinese,
        //       Arabic, Korean, etc. e.g. "Arial Unicode" (but it's 23MB!).
        skin->SetDefaultFont("OpenSans.ttf", 11);
        
        // Create a Canvas (it's root, on which all other Gwork panels are created)
        std::unique_ptr<Gwk::Controls::Canvas> canvas(new Gwk::Controls::Canvas(skin.get()));
        canvas->SetSize(screenSize.x, screenSize.y);
        canvas->SetDrawBackground(true);
        canvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));
        
        // Create our unittest control (which is a Window with controls in it)
        std::unique_ptr<Gwk::Controls::Base> unit(Gwk::Test::CreateTests(canvas.get()));
        unit->SetPos(10, 10);
        
        Gwk::Input::SDL2 input;
        input.Initialize(canvas.get());
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
            
            renderer->BeginContext(nullptr);
            canvas->RenderCanvas();
            renderer->PresentContext(nullptr);
            renderer->EndContext(nullptr);
        }
    }

    TTF_Quit();
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
