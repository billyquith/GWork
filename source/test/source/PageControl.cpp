/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include "Gwork/Test/Test.h"
#include "Gwork/Controls.h"

using namespace Gwk;

class PageControl : public GUnit
{
public:

    GWK_CONTROL_INLINE(PageControl, GUnit)
    {
        Gwk::Controls::PageControl* control = new Gwk::Controls::PageControl(this);
        control->SetSize(500, 300);
        control->SetPageCount(5);
        control->onPageChanged.Add(this, &ThisClass::OnPageChanged);
        control->onFinish.Add(this, &ThisClass::OnFinish);
        // Page 0
        {
            Gwk::Controls::Button* pButton = new Gwk::Controls::Button(control->GetPage(0));
            pButton->Dock(Docking::Fill);
            pButton->SetText("This button is fill docked on page 0");
        }
        // Page 1
        {
            Gwk::Controls::Button* pButton = new Gwk::Controls::Button(control->GetPage(1));
            pButton->Dock(Docking::Top);
            pButton->SetText("This button is top docked on page 1");
        }
        // Page 2
        {
            Gwk::Controls::Button* pButton = new Gwk::Controls::Button(control->GetPage(2));
            pButton->SetSize(400, 1000);
            pButton->SetPos(50, 50);
            pButton->SetText("This button is long to test scrolling (page 2)");
        }
    }

    void OnPageChanged(Event::Info info)
    {
        UnitPrint(Utility::Format("PageControl Changed Page: %i", info.Integer));
    }

    void OnFinish(Event::Info info)
    {
        UnitPrint("Finish Pressed");
    }

};


DEFINE_UNIT_TEST(PageControl);
