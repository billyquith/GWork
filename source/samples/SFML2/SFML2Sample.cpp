/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#include <SFML/Graphics.hpp>

#include <Gwork/Renderers/SFML2.h>
#include <Gwork/Input/SFML2.h>

#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>


int main()
{
    // Create the window of the application
    sf::RenderWindow app(sf::VideoMode(1004, 650, 32), "Gwork: SFML2");

    // Create renderer
    Gwk::Renderer::SFML2 renderer(app, GWORK_RESOURCE_DIR);
    
    // Create a Gwork skin
    Gwk::Skin::TexturedBase skin(&renderer);
    skin.Init("DefaultSkin.png");

    // The fonts work differently in SFML - it can't use
    // system fonts. So force the skin to use a local one.
    skin.SetDefaultFont("OpenSans.ttf", 11);

    // Create a Canvas (it's root, on which all other Gwork panels are created)
    Gwk::Controls::Canvas* canvas = new Gwk::Controls::Canvas(&skin);

    canvas->SetSize(app.getSize().x, app.getSize().y);
    canvas->SetDrawBackground(true);
    canvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));

    // Create our unittest control (which is a Window with controls in it)
    new TestFrame(canvas);

    // Create an input processor
    Gwk::Input::SFML GworkInput;
    GworkInput.Initialize(canvas);
    
    while (app.isOpen())
    {
        // Handle events
        sf::Event Event;

        while (app.pollEvent(Event))
        {
            // Window closed or escape key pressed : exit
            if (Event.type == sf::Event::Closed
                || (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape))
            {
                app.close();
                break;
            }
            else if (Event.type == sf::Event::Resized)
            {
                canvas->SetSize(Event.size.width, Event.size.height);
            }

            GworkInput.ProcessMessage(Event);
        }

        // Render the control canvas
        app.clear();
        // <user render here>
        app.pushGLStates();
        canvas->RenderCanvas();
        app.popGLStates();        
        app.display();
    }

    return EXIT_SUCCESS;
}
