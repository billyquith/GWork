/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Gwork.h>
#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/TestAPI.h>
#include <Gwork/Input/SDL2.h>
#include <Gwork/Renderers/SDL2.h>
#include <SDL.h>
#include <SDL_ttf.h>


int main(int argc, char** argv)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
        return EXIT_FAILURE;
    
    if (TTF_Init() != 0)
        return EXIT_FAILURE;
        
    //Setup our window and renderer
    const int SCREEN_WIDTH = 1024, SCREEN_HEIGHT = 768;
	SDL_Window *window = SDL_CreateWindow("Gwork: SDL2", 100, 100,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window)
		return EXIT_FAILURE;

    // Create a Gwork Allegro Renderer
    Gwk::Renderer::SDL2 *renderer = new Gwk::Renderer::SDL2(window);

    // Create a Gwork skin
    Gwk::Skin::TexturedBase skin(renderer);
    skin.SetRender(renderer);
    skin.Init("DefaultSkin.png");
    
    // Note, you can get fonts that cover many languages/locales to do Chinese,
    //       Arabic, Korean, etc. e.g. "Arial Unicode" (but it's 23MB!).
    skin.SetDefaultFont("OpenSans.ttf", 11);
    
    // Create a Canvas (it's root, on which all other Gwork panels are created)
    Gwk::Controls::Canvas* canvas = new Gwk::Controls::Canvas(&skin);
    canvas->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    canvas->SetDrawBackground(true);
    canvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));

    // Create our unittest control (which is a Window with controls in it)
    TestAPI* unit = new TestAPI(canvas);
    unit->SetPos(10, 10);

    Gwk::Input::SDL2 GworkInput;
    GworkInput.Initialize(canvas);
    bool haveQuit = false;

    while (!haveQuit)
    {
        SDL_Event evt;
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT)
                haveQuit = true;

            GworkInput.ProcessEvent(&evt);
        }
        
        renderer->BeginContext(nullptr);
        canvas->RenderCanvas();
        renderer->PresentContext(nullptr);
        renderer->EndContext(nullptr);
    }

    //TTF_Quit();   TODO: Currently crashes. Gwork needs work.
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
