/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>

using namespace Gwk;

class Button : public TestUnit
{
public:

    GWK_CONTROL_INLINE(Button, TestUnit)
    {
        // Normal button
        Controls::Button* buttonA = new Controls::Button(this);
        buttonA->SetText("Event Tester");
        buttonA->onPress.Add(this, &Button::OnButtonA);
        {
            Controls::Button* buttonA = new Controls::Button(this);
            buttonA->SetBounds(200, 30, 300, 200);
            buttonA->SetText("Event Tester 2");
            buttonA->onPress.Add(this, &Button::OnButtonA);
        }
        // Unicode test
        Controls::Button* buttonB = new Controls::Button(this);
        buttonB->SetText(Utility::Narrow(
            L"\u0417\u0430\u043C\u0435\u0436\u043D\u0430\u044F \u043C\u043E\u0432\u0430"));
        Gwk::Align::PlaceBelow(buttonB, buttonA, 10);
        // Image with text
        Controls::Button* buttonC = new Controls::Button(this);
        buttonC->SetText("Image Button");
        buttonC->SetImage("test16.png");
        Gwk::Align::PlaceBelow(buttonC, buttonB, 10);
        // Just image
        Controls::Button* buttonD = new Controls::Button(this);
        buttonD->SetText("");
        buttonD->SetImage("test16.png");
        buttonD->SetSize(20, 20);
        Gwk::Align::PlaceBelow(buttonD, buttonC, 10);
        // Toggle button
        Controls::Button* buttonE = new Controls::Button(this);
        buttonE->SetText("Toggle Me");
        buttonE->SetIsToggle(true);
        buttonE->onToggle.Add(this, &Button::OnToggle);
        buttonE->onToggleOn.Add(this, &Button::OnToggleOn);
        buttonE->onToggleOff.Add(this, &Button::OnToggleOff);
        Gwk::Align::PlaceBelow(buttonE, buttonD, 10);
        // Disabled Button
        Controls::Button* buttonF = new Controls::Button(this);
        buttonF->SetText("Disabled :D");
        buttonF->SetDisabled(true);
        Gwk::Align::PlaceBelow(buttonF, buttonE, 10);
        // Tooltip Button
        Controls::Button* buttonG = new Controls::Button(this);
        buttonG->SetText("With Tooltip");
        buttonG->SetToolTip("This is a tooltip!");
        Gwk::Align::PlaceBelow(buttonG, buttonF, 10);
    }

    void OnButtonA(Event::Info)
    {
        OutputToLog("Button Pressed (using 'OnPress' event)");
    }

    void OnToggle(Event::Info)
    {
        OutputToLog("Button Toggled (using 'OnToggle' event)");
    }

    void OnToggleOn(Event::Info)
    {
        OutputToLog("Button Toggled ON (using 'OnToggleOn' event)");
    }

    void OnToggleOff(Event::Info)
    {
        OutputToLog("Button Toggled Off (using 'OnToggleOff' event)");
    }

};


DECLARE_TEST(Button);
