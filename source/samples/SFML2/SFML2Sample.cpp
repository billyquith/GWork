/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2016 Billy Quith
 *  See license in Gwork.h
 */

#include <SFML/Graphics.hpp>
#include <cmath>

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
    Gwk::Renderer::SFML2 renderer(app);
    
    // Create a Gwork skin
    Gwk::Skin::TexturedBase skin(&renderer);
    skin.Init("DefaultSkin.png");

    // The fonts work differently in SFML - it can't use
    // system fonts. So force the skin to use a local one.
    skin.SetDefaultFont("OpenSans.ttf", 11);

    // Create a Canvas (it's root, on which all other Gwork panels are created)
    Gwk::Controls::Canvas* pCanvas = new Gwk::Controls::Canvas(&skin);

    pCanvas->SetSize(app.getSize().x, app.getSize().y);
    pCanvas->SetDrawBackground(true);
    pCanvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));

    // Create our unittest control (which is a Window with controls in it)
    new UnitTest(pCanvas);

    // Create an input processor
    Gwk::Input::SFML GworkInput;
    GworkInput.Initialize(pCanvas);
    
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
                pCanvas->SetSize(Event.size.width, Event.size.height);
            }

            GworkInput.ProcessMessage(Event);
        }

        // Render the control canvas
        app.clear();
        pCanvas->RenderCanvas();
        app.display();
    }

    return EXIT_SUCCESS;
}
