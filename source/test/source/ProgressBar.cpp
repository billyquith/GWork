/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/Test.h>
#include <Gwork/Controls/RadioButtonController.h>
#include <Gwork/Controls/ProgressBar.h>

using namespace Gwk;

class ProgressBar : public GUnit
{
public:

    GWK_CONTROL_INLINE(ProgressBar, GUnit)
    {
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(110, 20, 200, 20));
            pb->SetValue(0.27f);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(110, 50, 200, 20));
            pb->SetValue(0.66f);
            pb->SetAlignment(Docking::Right|Docking::CenterV);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(110, 80, 200, 20));
            pb->SetValue(0.88f);
            pb->SetAlignment(Docking::Left|Docking::CenterV);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(110, 110, 200, 20));
            pb->SetAutoLabel(false);
            pb->SetValue(0.20f);
            pb->SetAlignment(Docking::Right|Docking::CenterV);
            pb->SetText("40,245 MB");
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(110, 140, 200, 20));
            pb->SetAutoLabel(false);
            pb->SetValue(1.00f);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(110, 170, 200, 20));
            pb->SetAutoLabel(false);
            pb->SetValue(0.00f);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(110, 200, 200, 20));
            pb->SetAutoLabel(false);
            pb->SetValue(0.50f);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(20, 20, 25, 200));
            pb->SetVertical();
            pb->SetValue(0.25f);
            pb->SetAlignment(Docking::Top|Docking::CenterH);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(50, 20, 25, 200));
            pb->SetVertical();
            pb->SetValue(0.40f);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(80, 20, 25, 200));
            pb->SetVertical();
            pb->SetAlignment(Docking::Bottom|Docking::CenterH);
            pb->SetValue(0.65f);
        }
    }

};


DEFINE_UNIT_TEST(ProgressBar);
