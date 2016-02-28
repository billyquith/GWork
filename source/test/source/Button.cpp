/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/Test.h>

using namespace Gwk;

class Button : public GUnit
{
public:

    GWK_CONTROL_INLINE(Button, GUnit)
    {
        // Normal button
        Controls::Button* pButtonA = new Controls::Button(this);
        pButtonA->SetText("Event Tester");
        pButtonA->onPress.Add(this, &Button::onButtonA);
        {
            Controls::Button* pButtonA = new Controls::Button(this);
            pButtonA->SetBounds(200, 30, 300, 200);
            pButtonA->SetText("Event Tester");
            pButtonA->onPress.Add(this, &Button::onButtonA);
        }
        // Unicode test
        Controls::Button* pButtonB = new Controls::Button(this);
        pButtonB->SetText(Utility::Narrow(L"\u0417\u0430\u043C\u0435\u0436\u043D\u0430\u044F \u043C\u043E\u0432\u0430"));
        Gwk::Align::PlaceBelow(pButtonB, pButtonA, 10);
        // Image with text
        Controls::Button* pButtonC = new Controls::Button(this);
        pButtonC->SetText("Image Button");
        pButtonC->SetImage("test16.png");
        Gwk::Align::PlaceBelow(pButtonC, pButtonB, 10);
        // Just image
        Controls::Button* pButtonD = new Controls::Button(this);
        pButtonD->SetText("");
        pButtonD->SetImage("test16.png");
        pButtonD->SetSize(20, 20);
        Gwk::Align::PlaceBelow(pButtonD, pButtonC, 10);
        // Toggle button
        Controls::Button* pButtonE = new Controls::Button(this);
        pButtonE->SetText("Toggle Me");
        pButtonE->SetIsToggle(true);
        pButtonE->onToggle.Add(this, &Button::OnToggle);
        pButtonE->onToggleOn.Add(this, &Button::OnToggleOn);
        pButtonE->onToggleOff.Add(this, &Button::OnToggleOff);
        Gwk::Align::PlaceBelow(pButtonE, pButtonD, 10);
        // Disabled Button
        Controls::Button* pButtonF = new Controls::Button(this);
        pButtonF->SetText("Disabled :D");
        pButtonF->SetDisabled(true);
        Gwk::Align::PlaceBelow(pButtonF, pButtonE, 10);
        // Tooltip Button
        Controls::Button* pButtonG = new Controls::Button(this);
        pButtonG->SetText("With Tooltip");
        pButtonG->SetToolTip("This is a tooltip!");
        Gwk::Align::PlaceBelow(pButtonG, pButtonF, 10);
    }

    void onButtonA(Controls::Base* pControl)
    {
        UnitPrint("Button Pressed (using 'OnPress' event)");
    }

    void OnToggle(Controls::Base* pControl)
    {
        UnitPrint("Button Toggled (using 'OnToggle' event)");
    }

    void OnToggleOn(Controls::Base* pControl)
    {
        UnitPrint("Button Toggled ON (using 'OnToggleOn' event)");
    }

    void OnToggleOff(Controls::Base* pControl)
    {
        UnitPrint("Button Toggled Off (using 'OnToggleOff' event)");
    }

};


DEFINE_UNIT_TEST(Button);
