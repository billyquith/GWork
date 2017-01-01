/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/ScrollControl.h>

using namespace Gwk;

class ScrollControl : public TestUnit
{
public:

    GWK_CONTROL_INLINE(ScrollControl, TestUnit)
    {
        {
            Gwk::Controls::ScrollControl* ctrl = new Gwk::Controls::ScrollControl(this);
            ctrl->SetBounds(10, 10, 100, 100);
            Controls::Button* testButton = new Controls::Button(ctrl);
            testButton->SetText("Twice As Big");
            testButton->SetBounds(0, 0, 200, 200);
        }
        {
            Gwk::Controls::ScrollControl* ctrl = new Gwk::Controls::ScrollControl(this);
            ctrl->SetBounds(110, 10, 100, 100);
            Controls::Button* testButton = new Controls::Button(ctrl);
            testButton->SetText("Same Size");
            testButton->SetBounds(0, 0, 100, 100);
        }
        {
            Gwk::Controls::ScrollControl* ctrl = new Gwk::Controls::ScrollControl(this);
            ctrl->SetBounds(210, 10, 100, 100);
            Controls::Button* testButton = new Controls::Button(ctrl);
            testButton->SetText("Wide");
            testButton->SetBounds(0, 0, 200, 50);
        }
        {
            Gwk::Controls::ScrollControl* ctrl = new Gwk::Controls::ScrollControl(this);
            ctrl->SetBounds(310, 10, 100, 100);
            Controls::Button* testButton = new Controls::Button(ctrl);
            testButton->SetText("Tall");
            testButton->SetBounds(0, 0, 50, 200);
        }
        {
            Gwk::Controls::ScrollControl* ctrl = new Gwk::Controls::ScrollControl(this);
            ctrl->SetBounds(410, 10, 100, 100);
            ctrl->SetScroll(false, true);
            Controls::Button* testButton = new Controls::Button(ctrl);
            testButton->SetText("Vertical");
            testButton->SetBounds(0, 0, 200, 200);
        }
        {
            Gwk::Controls::ScrollControl* ctrl = new Gwk::Controls::ScrollControl(this);
            ctrl->SetBounds(510, 10, 100, 100);
            ctrl->SetScroll(true, false);
            Controls::Button* testButton = new Controls::Button(ctrl);
            testButton->SetText("Horinzontal");
            testButton->SetBounds(0, 0, 200, 200);
        }
        // Bottom Row
        {
            Gwk::Controls::ScrollControl* ctrl = new Gwk::Controls::ScrollControl(this);
            ctrl->SetBounds(10, 110, 100, 100);
            ctrl->SetAutoHideBars(true);
            Controls::Button* testButton = new Controls::Button(ctrl);
            testButton->SetText("Twice As Big");
            testButton->SetBounds(0, 0, 200, 200);
        }
        {
            Gwk::Controls::ScrollControl* ctrl = new Gwk::Controls::ScrollControl(this);
            ctrl->SetBounds(110, 110, 100, 100);
            ctrl->SetAutoHideBars(true);
            Controls::Button* testButton = new Controls::Button(ctrl);
            testButton->SetText("Same Size");
            testButton->SetBounds(0, 0, 100, 100);
        }
        {
            Gwk::Controls::ScrollControl* ctrl = new Gwk::Controls::ScrollControl(this);
            ctrl->SetBounds(210, 110, 100, 100);
            ctrl->SetAutoHideBars(true);
            Controls::Button* testButton = new Controls::Button(ctrl);
            testButton->SetText("Wide");
            testButton->SetBounds(0, 0, 200, 50);
        }
        {
            Gwk::Controls::ScrollControl* ctrl = new Gwk::Controls::ScrollControl(this);
            ctrl->SetBounds(310, 110, 100, 100);
            ctrl->SetAutoHideBars(true);
            Controls::Button* testButton = new Controls::Button(ctrl);
            testButton->SetText("Tall");
            testButton->SetBounds(0, 0, 50, 200);
        }
        {
            Gwk::Controls::ScrollControl* ctrl = new Gwk::Controls::ScrollControl(this);
            ctrl->SetBounds(410, 110, 100, 100);
            ctrl->SetAutoHideBars(true);
            ctrl->SetScroll(false, true);
            Controls::Button* testButton = new Controls::Button(ctrl);
            testButton->SetText("Vertical");
            testButton->SetBounds(0, 0, 200, 200);
        }
        {
            Gwk::Controls::ScrollControl* ctrl = new Gwk::Controls::ScrollControl(this);
            ctrl->SetBounds(510, 110, 100, 100);
            ctrl->SetAutoHideBars(true);
            ctrl->SetScroll(true, false);
            Controls::Button* testButton = new Controls::Button(ctrl);
            testButton->SetText("Horinzontal");
            testButton->SetBounds(0, 0, 200, 200);
        }
    }

};


DECLARE_TEST(ScrollControl);
