/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2016 Billy Quith
 *  See license in Gwork.h
 */

#include <SFML/Graphics.hpp>
#include <cmath>

#include <Gwork/Renderers/SFML2.h>
#include <Gwork/Input/SFML.h>

#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
////////////////////////////////////////////////////////////
int main()
{
    // Create the window of the application
    sf::RenderWindow App(sf::VideoMode(1004, 650, 32), "Gwork: SFML2");

    Gwk::Renderer::SFML2 GworkRenderer(App);
    
    //
    // Create a Gwork skin
    //
    // Gwk::Skin::Simple skin;
    // skin.SetRender( &GworkRenderer );

    Gwk::Skin::TexturedBase skin(&GworkRenderer);
    skin.Init("DefaultSkin.png");

    // The fonts work differently in SFML - it can't use
    // system fonts. So force the skin to use a local one.
    skin.SetDefaultFont("OpenSans.ttf", 11);


    //
    // Create a Canvas (it's root, on which all other Gwork panels are created)
    //
    Gwk::Controls::Canvas* pCanvas = new Gwk::Controls::Canvas(&skin);

    pCanvas->SetSize(App.getSize().x, App.getSize().y);
    pCanvas->SetDrawBackground(true);
    pCanvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));

    //
    // Create our unittest control (which is a Window with controls in it)
    //
    new UnitTest(pCanvas);

    //
    // Create an input processor
    //
    Gwk::Input::SFML GworkInput;
    GworkInput.Initialize(pCanvas);
    
    while (App.isOpen())
    {
        // Handle events
        sf::Event Event;

        while (App.pollEvent(Event))
        {
            // Window closed or escape key pressed : exit
            if (Event.type == sf::Event::Closed
                || (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape))
            {
                App.close();
                break;
            }
            else if (Event.type == sf::Event::Resized)
            {
                pCanvas->SetSize(Event.size.width, Event.size.height);
            }

            GworkInput.ProcessMessage(Event);
        }

        // Clear the window

        App.clear();
        pCanvas->RenderCanvas();
        App.display();
    }

    return EXIT_SUCCESS;
}
