/*
 *  Gwork Null Sample
 *
 * $$$$ NOTE: This is a null application that is used to test Gwork compiles. $$$$
 *
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>
#include <Gwork/Input/Null.h>
#include <Gwork/Renderers/Null.h>
#include <Gwork/Platform.h>
#include <memory>

int main(int argc, char** argv)
{
    // Create a Null renderer
    Gwk::Platform::RelativeToExecutablePaths paths(".");
    std::unique_ptr<Gwk::Renderer::Null> renderer(new Gwk::Renderer::Null(paths));

    // Create a Gwork skin
    std::unique_ptr<Gwk::Skin::TexturedBase> skin(new Gwk::Skin::TexturedBase(renderer.get()));
    skin->SetRender(renderer.get());
    skin->Init("DefaultSkin.png");

    skin->SetDefaultFont("OpenSans.ttf", 11);

    // Create a Canvas (it's root, on which all other Gwork panels are created)
    std::unique_ptr<Gwk::Controls::Canvas> canvas(new Gwk::Controls::Canvas(skin.get()));
    canvas->SetSize(1024, 768);
    canvas->SetDrawBackground(true);
    canvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));

    // Create our test control (which is a Window with controls in it)
    std::unique_ptr<Gwk::Controls::Base> unit(Gwk::Test::CreateTests(canvas.get()));
    unit->SetPos(10, 10);

    Gwk::Input::Null input;
    input.Initialize(canvas.get());

    canvas->RenderCanvas();
    
    return EXIT_SUCCESS;
}
