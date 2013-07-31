#include "Gwen/UnitTest/UnitTest.h"
#include "Gwen/Controls/TabControl.h"
#include "Gwen/Controls/RadioButtonController.h"

using namespace Gwen;

class TabControl : public GUnit
{
public:

    Controls::TabControl* m_pDockControlLeft;

    GWEN_CONTROL_INLINE(TabControl, GUnit)
    {
        {
            m_pDockControlLeft = new Controls::TabControl(this);
            m_pDockControlLeft->SetBounds(10, 10, 200, 200);
            {
                Controls::TabButton* pButton = m_pDockControlLeft->AddPage("Controls");
                Base* pPage = pButton->GetPage();
                {
                    Controls::RadioButtonController* pRadio = new Controls::RadioButtonController(
                        pPage);
                    pRadio->SetBounds(10, 10, 100, 100);
                    pRadio->AddOption("Top")->Select();
                    pRadio->AddOption("Bottom");
                    pRadio->AddOption("Left");
                    pRadio->AddOption("Right");
                    pRadio->onSelectionChange.Add(this, &ThisClass::OnDockChange);
                }
            }
            m_pDockControlLeft->AddPage("Red");
            m_pDockControlLeft->AddPage("Green");
            m_pDockControlLeft->AddPage("Blue");
        }
        {
            Controls::TabControl* pDragMe = new Controls::TabControl(this);
            pDragMe->SetBounds(220, 10, 200, 200);
            pDragMe->AddPage("You");
            pDragMe->AddPage("Can");
            pDragMe->AddPage("Reorder")->SetImage("test16.png");
            pDragMe->AddPage("These");
            pDragMe->AddPage("Tabs");
            pDragMe->SetAllowReorder(true);
        }
    }

    void OnDockChange(Gwen::Controls::Base* pControl)
    {
        Gwen::Controls::RadioButtonController* rc =
            (Gwen::Controls::RadioButtonController*)pControl;

        if (rc->GetSelectedLabel() == "Top")
            m_pDockControlLeft->SetTabStripPosition(Pos::Top);

        if (rc->GetSelectedLabel() == "Bottom")
            m_pDockControlLeft->SetTabStripPosition(Pos::Bottom);

        if (rc->GetSelectedLabel() == "Left")
            m_pDockControlLeft->SetTabStripPosition(Pos::Left);

        if (rc->GetSelectedLabel() == "Right")
            m_pDockControlLeft->SetTabStripPosition(Pos::Right);
    }

    Gwen::Font m_Font;
};


DEFINE_UNIT_TEST(TabControl, "TabControl");
