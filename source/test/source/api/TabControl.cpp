/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/TabControl.h>
#include <Gwork/Controls/RadioButtonController.h>

using namespace Gwk;

class TabControl : public GUnit
{
public:

    Controls::TabControl* m_dockControlLeft;

    GWK_CONTROL_INLINE(TabControl, GUnit)
    {
        {
            m_dockControlLeft = new Controls::TabControl(this);
            m_dockControlLeft->SetBounds(10, 10, 200, 200);
            {
                Controls::TabButton* button = m_dockControlLeft->AddPage("Controls");
                Base* page = button->GetPage();
                {
                    Controls::RadioButtonController* radio = new Controls::RadioButtonController(
                        page);
                    radio->SetBounds(10, 10, 100, 100);
                    radio->AddOption("Top")->Select();
                    radio->AddOption("Bottom");
                    radio->AddOption("Left");
                    radio->AddOption("Right");
                    radio->onSelectionChange.Add(this, &ThisClass::OnDockChange);
                }
            }
            m_dockControlLeft->AddPage("Red");
            m_dockControlLeft->AddPage("Green");
            m_dockControlLeft->AddPage("Blue");
        }
        {
            Controls::TabControl* dragMe = new Controls::TabControl(this);
            dragMe->SetBounds(220, 10, 200, 200);
            dragMe->AddPage("You");
            dragMe->AddPage("Can");
            dragMe->AddPage("Reorder")->SetImage("test16.png");
            dragMe->AddPage("These");
            dragMe->AddPage("Tabs");
            dragMe->SetAllowReorder(true);
        }
    }

    void OnDockChange(Gwk::Controls::Base* control)
    {
        Gwk::Controls::RadioButtonController* rc =
            (Gwk::Controls::RadioButtonController*)control;

        if (rc->GetSelectedLabel() == "Top")
            m_dockControlLeft->SetTabStripPosition(Docking::Top);

        if (rc->GetSelectedLabel() == "Bottom")
            m_dockControlLeft->SetTabStripPosition(Docking::Bottom);

        if (rc->GetSelectedLabel() == "Left")
            m_dockControlLeft->SetTabStripPosition(Docking::Left);

        if (rc->GetSelectedLabel() == "Right")
            m_dockControlLeft->SetTabStripPosition(Docking::Right);
    }

    Gwk::Font m_font;
};


DEFINE_UNIT_TEST(TabControl);
