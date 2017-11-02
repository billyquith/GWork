/*
 *  Gwork
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Gwork.h>
#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/TestAPI.h>
#include <Gwork/Renderers/Software.h>
#include <Gwork/Platform.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Gwork/External/stb_image_write.h>


int main(int argc, char** argv)
{
    // Setup our window and renderer
    const Gwk::Point screenSize(1024, 768);

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

    renderer->BeginContext(nullptr);
    canvas->RenderCanvas();
    renderer->PresentContext(nullptr);
    renderer->EndContext(nullptr);
    
    stbi_write_png("sw.png", screenSize.x, screenSize.y, 4, &pixbuff.At(0,0), 4*screenSize.x);
    
    delete unit;
    delete renderer;
    
    return EXIT_SUCCESS;
}
