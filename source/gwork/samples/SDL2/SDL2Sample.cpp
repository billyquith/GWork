

#include <Gwork/Gwork.h>
#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/UnitTest/UnitTest.h>
#include <Gwork/Input/SDL2.h>
#include <Gwork/Renderers/SDL2.h>
#include <SDL.h>
#include <SDL_ttf.h>


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

    // Create a Gwork Allegro Renderer
    Gwk::Renderer::SDL2 *pRenderer = new Gwk::Renderer::SDL2(window);

    // Create a Gwork skin
    Gwk::Skin::TexturedBase skin(pRenderer);
    skin.SetRender(pRenderer);
    skin.Init("DefaultSkin.png");
    
    // Note, you can get fonts that cover many languages/locales to do Chinese,
    //       Arabic, Korean, etc. e.g. "Arial Unicode" (but it's 23MB!).
    skin.SetDefaultFont("OpenSans.ttf", 11);
    
    // Create a Canvas (it's root, on which all other Gwork panels are created)
    Gwk::Controls::Canvas* pCanvas = new Gwk::Controls::Canvas(&skin);
    pCanvas->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    pCanvas->SetDrawBackground(true);
    pCanvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));

    // Create our unittest control (which is a Window with controls in it)
    UnitTest* pUnit = new UnitTest(pCanvas);
    pUnit->SetPos(10, 10);

    Gwk::Input::SDL2 GworkInput;
    GworkInput.Initialize(pCanvas);
    bool bQuit = false;

    while (!bQuit)
    {
        SDL_Event evt;
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT)
                bQuit = true;

            GworkInput.ProcessEvent(&evt);
        }
        
        pRenderer->BeginContext(NULL);
        pCanvas->RenderCanvas();
        pRenderer->PresentContext(NULL);
        pRenderer->EndContext(NULL);
    }

    //TTF_Quit();   TODO: Currently crashes. Gwork needs work.
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
