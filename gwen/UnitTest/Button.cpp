#include "Gwen/UnitTest/UnitTest.h"

using namespace Gwen;

class Button : public GUnit
{
public:

    GWEN_CONTROL_INLINE(Button, GUnit)
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
        Gwen::Align::PlaceBelow(pButtonB, pButtonA, 10);
        // Image with text
        Controls::Button* pButtonC = new Controls::Button(this);
        pButtonC->SetText("Image Button");
        pButtonC->SetImage("test16.png");
        Gwen::Align::PlaceBelow(pButtonC, pButtonB, 10);
        // Just image
        Controls::Button* pButtonD = new Controls::Button(this);
        pButtonD->SetText("");
        pButtonD->SetImage("test16.png");
        pButtonD->SetSize(20, 20);
        Gwen::Align::PlaceBelow(pButtonD, pButtonC, 10);
        // Toggle button
        Controls::Button* pButtonE = new Controls::Button(this);
        pButtonE->SetText("Toggle Me");
        pButtonE->SetIsToggle(true);
        pButtonE->onToggle.Add(this, &Button::OnToggle);
        pButtonE->onToggleOn.Add(this, &Button::OnToggleOn);
        pButtonE->onToggleOff.Add(this, &Button::OnToggleOff);
        Gwen::Align::PlaceBelow(pButtonE, pButtonD, 10);
        // Disabled Button
        Controls::Button* pButtonF = new Controls::Button(this);
        pButtonF->SetText("Disabled :D");
        pButtonF->SetDisabled(true);
        Gwen::Align::PlaceBelow(pButtonF, pButtonE, 10);
        // Tooltip Button
        Controls::Button* pButtonG = new Controls::Button(this);
        pButtonG->SetText("With Tooltip");
        pButtonG->SetToolTip("This is a tooltip!");
        Gwen::Align::PlaceBelow(pButtonG, pButtonF, 10);
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


DEFINE_UNIT_TEST(Button, "Button");
