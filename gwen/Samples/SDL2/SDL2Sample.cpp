

#include <Gwen/Gwen.h>
#include <Gwen/Skins/Simple.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/UnitTest/UnitTest.h>
#include <Gwen/Input/SDL2.h>
#include <Gwen/Renderers/SDL2.h>

#ifdef _WIN32
#   include <SDL.h>
#   include <SDL_ttf.h>
#else
#   include <SDL2/SDL.h>
#   include <SDL2/SDL_ttf.h>
#endif


int main(int argc, char** argv)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
        return -1;
    
    if (TTF_Init() != 0)
        return -1;
        
    //Setup our window and renderer
    const int SCREEN_WIDTH = 1024, SCREEN_HEIGHT = 768;
	SDL_Window *window = SDL_CreateWindow("SDL2", 100, 100,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window)
		return -1;

    // Create a GWEN Allegro Renderer
    Gwen::Renderer::SDL2 *pRenderer = new Gwen::Renderer::SDL2(window);

    // Create a GWEN skin
    Gwen::Skin::TexturedBase skin(pRenderer);
    skin.SetRender(pRenderer);
    skin.Init("DefaultSkin.png");
    
    // Note, you can get fonts that cover many languages/locales to do Chinese,
    //       Arabic, Korean, etc. e.g. "Arial Unicode" (but it's 23MB!).
    skin.SetDefaultFont("OpenSans.ttf", 11);
    
    // Create a Canvas (it's root, on which all other GWEN panels are created)
    Gwen::Controls::Canvas* pCanvas = new Gwen::Controls::Canvas(&skin);
    pCanvas->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    pCanvas->SetDrawBackground(true);
    pCanvas->SetBackgroundColor(Gwen::Color(150, 170, 170, 255));

    // Create our unittest control (which is a Window with controls in it)
    UnitTest* pUnit = new UnitTest(pCanvas);
    pUnit->SetPos(10, 10);

    Gwen::Input::SDL2 GwenInput;
    GwenInput.Initialize(pCanvas);
    bool bQuit = false;

    while (!bQuit)
    {
        SDL_Event evt;
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT)
                bQuit = true;

            GwenInput.ProcessEvent(&evt);
        }
        
        pRenderer->BeginContext(NULL);
        pCanvas->RenderCanvas();
        pRenderer->PresentContext(NULL);
        pRenderer->EndContext(NULL);
    }

    //TTF_Quit();   TODO: Currently crashes. GWEN needs work.
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
