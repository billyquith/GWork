/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls.h>

using namespace Gwk;

class PageControl : public TestUnit
{
public:

    GWK_CONTROL_INLINE(PageControl, TestUnit)
    {
        Gwk::Controls::PageControl* control = new Gwk::Controls::PageControl(this);
        control->SetSize(500, 300);
        control->SetPageCount(5);
        control->onPageChanged.Add(this, &ThisClass::OnPageChanged);
        control->onFinish.Add(this, &ThisClass::OnFinish);
        // Page 0
        {
            Gwk::Controls::Button* button = new Gwk::Controls::Button(control->GetPage(0));
            button->Dock(Position::Fill);
            button->SetText("This button is fill docked on page 0");
        }
        // Page 1
        {
            Gwk::Controls::Button* button = new Gwk::Controls::Button(control->GetPage(1));
            button->Dock(Position::Top);
            button->SetText("This button is top docked on page 1");
        }
        // Page 2
        {
            Gwk::Controls::Button* button = new Gwk::Controls::Button(control->GetPage(2));
            button->SetSize(400, 1000);
            button->SetPos(50, 50);
            button->SetText("This button is long to test scrolling (page 2)");
        }
    }

    void OnPageChanged(Event::Info info)
    {
        OutputToLog(Utility::Format("PageControl Changed Page: %i", info.Integer));
    }

    void OnFinish(Event::Info info)
    {
        OutputToLog("Finish Pressed");
    }

};


DECLARE_TEST(PageControl);
