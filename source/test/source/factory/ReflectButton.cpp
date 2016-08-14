/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Util/ControlFactory.h>
#include <ponder/classget.hpp>
#include <ponder/class.hpp>
#include <ponder/pondertype.hpp>

using namespace Gwk;

class ReflectButton : public TestUnit
{
public:

    GWK_CONTROL(ReflectButton, TestUnit);

    void onButtonA(Event::Info info)
    {
        OutputToLog("Button Pressed (using 'OnPress' event)");
    }

    void OnToggle(Controls::Base* control)
    {
        OutputToLog("Button Toggled (using 'OnToggle' event)");
    }

    void OnToggleOn(Controls::Base* control)
    {
        OutputToLog("Button Toggled ON (using 'OnToggleOn' event)");
    }

    void OnToggleOff(Controls::Base* control)
    {
        OutputToLog("Button Toggled Off (using 'OnToggleOff' event)");
    }

};

static void declare()
{
    ponder::Class::declare<ReflectButton>()
        .base<Controls::Base>();
}

PONDER_AUTO_TYPE(ReflectButton, &declare);

GWK_CONTROL_CONSTRUCTOR(ReflectButton)
{
    // Normal button
    //        Controls::Button* buttonA = new Controls::Button(this);
    //        buttonA->SetText("Event Tester");
    //        buttonA->onPress.Add(this, &Button::onButtonA);
    
    const ponder::Class& metaclass = ponder::classByType<Controls::Button>();
    
    ponder::UserObject buttonA =
        metaclass.construct(ponder::Args(static_cast<Controls::Base*>(this)));
    assert(buttonA != ponder::UserObject::nothing);
    
    buttonA.set("text", "Hello world!");
    
    buttonA.get("onPress").to<Event::Listener*>()->Add(this, &ReflectButton::onButtonA);
    
    //        {
    //            Controls::Button* buttonA = new Controls::Button(this);
    //            buttonA->SetBounds(200, 30, 300, 200);
    //            buttonA->SetText("Event Tester 2");
    //            buttonA->onPress.Add(this, &Button::onButtonA);
    //        }
    //        // Unicode test
    //        Controls::Button* buttonB = new Controls::Button(this);
    //        buttonB->SetText(Utility::Narrow(L"\u0417\u0430\u043C\u0435\u0436\u043D\u0430\u044F \u043C\u043E\u0432\u0430"));
    //        Gwk::Align::PlaceBelow(buttonB, buttonA, 10);
    //        // Image with text
    //        Controls::Button* buttonC = new Controls::Button(this);
    //        buttonC->SetText("Image Button");
    //        buttonC->SetImage("test16.png");
    //        Gwk::Align::PlaceBelow(buttonC, buttonB, 10);
    //        // Just image
    //        Controls::Button* buttonD = new Controls::Button(this);
    //        buttonD->SetText("");
    //        buttonD->SetImage("test16.png");
    //        buttonD->SetSize(20, 20);
    //        Gwk::Align::PlaceBelow(buttonD, buttonC, 10);
    //        // Toggle button
    //        Controls::Button* buttonE = new Controls::Button(this);
    //        buttonE->SetText("Toggle Me");
    //        buttonE->SetIsToggle(true);
    //        buttonE->onToggle.Add(this, &Button::OnToggle);
    //        buttonE->onToggleOn.Add(this, &Button::OnToggleOn);
    //        buttonE->onToggleOff.Add(this, &Button::OnToggleOff);
    //        Gwk::Align::PlaceBelow(buttonE, buttonD, 10);
    //        // Disabled Button
    //        Controls::Button* buttonF = new Controls::Button(this);
    //        buttonF->SetText("Disabled :D");
    //        buttonF->SetDisabled(true);
    //        Gwk::Align::PlaceBelow(buttonF, buttonE, 10);
    //        // Tooltip Button
    //        Controls::Button* buttonG = new Controls::Button(this);
    //        buttonG->SetText("With Tooltip");
    //        buttonG->SetToolTip("This is a tooltip!");
    //        Gwk::Align::PlaceBelow(buttonG, buttonF, 10);
}

DECLARE_TEST(ReflectButton);
