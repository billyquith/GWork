/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>
#include <Gwork/Input/Allegro5.h>
#include <Gwork/Renderers/Allegro5.h>
#include <Gwork/Platform.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>


int main(int argc, char** argv)
{
    const Gwk::Point screenSize(1024, 768);

    if (!al_init())
        return EXIT_FAILURE;

    ALLEGRO_DISPLAY* display = al_create_display(screenSize.x, screenSize.y);
    Gwk::Platform::SetPlatformWindow(display);

    if (!display)
        return EXIT_FAILURE;

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    if (!event_queue)
        return EXIT_FAILURE;

    al_init_image_addon();
    al_init_font_addon();
    al_init_primitives_addon();
    al_init_ttf_addon();
    al_install_mouse();
    al_install_keyboard();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    Gwk::Platform::RelativeToExecutablePaths paths(GWK_SAMPLE_RESOURCE_DIR);

    // Create a Gwork Allegro Renderer
    Gwk::Renderer::Allegro* renderer = new Gwk::Renderer::Allegro(paths);

    // Create a Gwork skin
    auto skin = new Gwk::Skin::TexturedBase(renderer);
    skin->SetRender(renderer);
    skin->Init("DefaultSkin.png");

    // The fonts work differently in Allegro - it can't use
    // system fonts. So force the skin to use a local one.
    // Note, you can get fonts that cover many languages/locales to do Chinese,
    //       Arabic, Korean, etc. e.g. "Arial Unicode" (but it's 23MB!).
    skin->SetDefaultFont("OpenSans.ttf", 11);

    // Create a Canvas (it's root, on which all other Gwork panels are created)
    Gwk::Controls::Canvas* canvas = new Gwk::Controls::Canvas(skin);
    canvas->SetSize(screenSize.x, screenSize.y);
    canvas->SetDrawBackground(true);
    canvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));

    // Create our unittest control (which is a Window with controls in it)
    auto unit = Gwk::Test::CreateTests(canvas);
    unit->SetPos(10, 10);

    // Create a Windows Control helper
    // (Processes Windows MSG's and fires input at Gwork)
    Gwk::Input::Allegro input;
    input.Initialize(canvas);

    ALLEGRO_EVENT ev;
    bool haveQuit = false;
    while (!haveQuit)
    {
        while (al_get_next_event(event_queue, &ev))
        {
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                haveQuit = true;

            input.ProcessMessage(ev);
        }

        renderer->BeginContext(nullptr);
        canvas->RenderCanvas();
        renderer->PresentContext(nullptr);
        renderer->EndContext(nullptr);

        al_rest(0.001);
    }

    delete unit;
    delete canvas;
    delete skin;
    delete renderer;

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return EXIT_SUCCESS;
}
