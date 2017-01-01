/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/RadioButtonController.h>
#include <Gwork/Controls/ProgressBar.h>

using namespace Gwk;

class ProgressBar : public TestUnit
{
public:

    GWK_CONTROL_INLINE(ProgressBar, TestUnit)
    {
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(110, 20, 200, 20));
            pb->SetProgress(0.27f);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(110, 50, 200, 20));
            pb->SetProgress(0.66f);
            pb->SetAlignment(Position::Right|Position::CenterV);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(110, 80, 200, 20));
            pb->SetProgress(0.88f);
            pb->SetAlignment(Position::Left|Position::CenterV);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(110, 110, 200, 20));
            pb->SetAutoLabel(false);
            pb->SetProgress(0.20f);
            pb->SetAlignment(Position::Right|Position::CenterV);
            pb->SetText("40,245 MB");
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(110, 140, 200, 20));
            pb->SetAutoLabel(false);
            pb->SetProgress(1.00f);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(110, 170, 200, 20));
            pb->SetAutoLabel(false);
            pb->SetProgress(0.00f);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(110, 200, 200, 20));
            pb->SetAutoLabel(false);
            pb->SetProgress(0.50f);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(20, 20, 25, 200));
            pb->SetVertical();
            pb->SetProgress(0.25f);
            pb->SetAlignment(Position::Top|Position::CenterH);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(50, 20, 25, 200));
            pb->SetVertical();
            pb->SetProgress(0.40f);
        }
        {
            Gwk::Controls::ProgressBar* pb = new Gwk::Controls::ProgressBar(this);
            pb->SetBounds(Gwk::Rect(80, 20, 25, 200));
            pb->SetVertical();
            pb->SetAlignment(Position::Bottom|Position::CenterH);
            pb->SetProgress(0.65f);
        }
    }

};


DECLARE_TEST(ProgressBar);
