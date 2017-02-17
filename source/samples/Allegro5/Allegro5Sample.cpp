/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/TestAPI.h>
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
    if (!al_init())
        return EXIT_FAILURE;

    ALLEGRO_DISPLAY* display = al_create_display(1024, 768);
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
    
    // Create a Gwork Allegro Renderer
    Gwk::Renderer::Allegro* renderer = new Gwk::Renderer::Allegro();

    // Create a Gwork skin
    Gwk::Skin::TexturedBase skin(renderer);
    skin.SetRender(renderer);
    skin.Init("DefaultSkin.png");
    
    // The fonts work differently in Allegro - it can't use
    // system fonts. So force the skin to use a local one.
    // Note, you can get fonts that cover many languages/locales to do Chinese,
    //       Arabic, Korean, etc. e.g. "Arial Unicode" (but it's 23MB!).
    skin.SetDefaultFont("OpenSans.ttf", 11);
    
    // Create a Canvas (it's root, on which all other Gwork panels are created)
    Gwk::Controls::Canvas* canvas = new Gwk::Controls::Canvas(&skin);
    canvas->SetSize(1024, 768);
    canvas->SetDrawBackground(true);
    canvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));

    // Create our unittest control (which is a Window with controls in it)
    auto unit = new TestFrame(canvas);
    unit->SetPos(10, 10);

    // Create a Windows Control helper
    // (Processes Windows MSG's and fires input at Gwork)
    Gwk::Input::Allegro GworkInput;
    GworkInput.Initialize(canvas);
    ALLEGRO_EVENT ev;
    bool haveQuit = false;

    while (!haveQuit)
    {
        while (al_get_next_event(event_queue, &ev))
        {
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                haveQuit = true;

            GworkInput.ProcessMessage(ev);
        }

        canvas->RenderCanvas();
        al_flip_display();
        
        al_rest(0.001);
    }

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return EXIT_SUCCESS;
}
