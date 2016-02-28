/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/Test.h>
#include <Gwork/Controls/ScrollControl.h>

using namespace Gwk;

class ScrollControl : public GUnit
{
public:

    GWK_CONTROL_INLINE(ScrollControl, GUnit)
    {
        {
            Gwk::Controls::ScrollControl* pCtrl = new Gwk::Controls::ScrollControl(this);
            pCtrl->SetBounds(10, 10, 100, 100);
            Controls::Button* pTestButton = new Controls::Button(pCtrl);
            pTestButton->SetText("Twice As Big");
            pTestButton->SetBounds(0, 0, 200, 200);
        }
        {
            Gwk::Controls::ScrollControl* pCtrl = new Gwk::Controls::ScrollControl(this);
            pCtrl->SetBounds(110, 10, 100, 100);
            Controls::Button* pTestButton = new Controls::Button(pCtrl);
            pTestButton->SetText("Same Size");
            pTestButton->SetBounds(0, 0, 100, 100);
        }
        {
            Gwk::Controls::ScrollControl* pCtrl = new Gwk::Controls::ScrollControl(this);
            pCtrl->SetBounds(210, 10, 100, 100);
            Controls::Button* pTestButton = new Controls::Button(pCtrl);
            pTestButton->SetText("Wide");
            pTestButton->SetBounds(0, 0, 200, 50);
        }
        {
            Gwk::Controls::ScrollControl* pCtrl = new Gwk::Controls::ScrollControl(this);
            pCtrl->SetBounds(310, 10, 100, 100);
            Controls::Button* pTestButton = new Controls::Button(pCtrl);
            pTestButton->SetText("Tall");
            pTestButton->SetBounds(0, 0, 50, 200);
        }
        {
            Gwk::Controls::ScrollControl* pCtrl = new Gwk::Controls::ScrollControl(this);
            pCtrl->SetBounds(410, 10, 100, 100);
            pCtrl->SetScroll(false, true);
            Controls::Button* pTestButton = new Controls::Button(pCtrl);
            pTestButton->SetText("Vertical");
            pTestButton->SetBounds(0, 0, 200, 200);
        }
        {
            Gwk::Controls::ScrollControl* pCtrl = new Gwk::Controls::ScrollControl(this);
            pCtrl->SetBounds(510, 10, 100, 100);
            pCtrl->SetScroll(true, false);
            Controls::Button* pTestButton = new Controls::Button(pCtrl);
            pTestButton->SetText("Horinzontal");
            pTestButton->SetBounds(0, 0, 200, 200);
        }
        // Bottom Row
        {
            Gwk::Controls::ScrollControl* pCtrl = new Gwk::Controls::ScrollControl(this);
            pCtrl->SetBounds(10, 110, 100, 100);
            pCtrl->SetAutoHideBars(true);
            Controls::Button* pTestButton = new Controls::Button(pCtrl);
            pTestButton->SetText("Twice As Big");
            pTestButton->SetBounds(0, 0, 200, 200);
        }
        {
            Gwk::Controls::ScrollControl* pCtrl = new Gwk::Controls::ScrollControl(this);
            pCtrl->SetBounds(110, 110, 100, 100);
            pCtrl->SetAutoHideBars(true);
            Controls::Button* pTestButton = new Controls::Button(pCtrl);
            pTestButton->SetText("Same Size");
            pTestButton->SetBounds(0, 0, 100, 100);
        }
        {
            Gwk::Controls::ScrollControl* pCtrl = new Gwk::Controls::ScrollControl(this);
            pCtrl->SetBounds(210, 110, 100, 100);
            pCtrl->SetAutoHideBars(true);
            Controls::Button* pTestButton = new Controls::Button(pCtrl);
            pTestButton->SetText("Wide");
            pTestButton->SetBounds(0, 0, 200, 50);
        }
        {
            Gwk::Controls::ScrollControl* pCtrl = new Gwk::Controls::ScrollControl(this);
            pCtrl->SetBounds(310, 110, 100, 100);
            pCtrl->SetAutoHideBars(true);
            Controls::Button* pTestButton = new Controls::Button(pCtrl);
            pTestButton->SetText("Tall");
            pTestButton->SetBounds(0, 0, 50, 200);
        }
        {
            Gwk::Controls::ScrollControl* pCtrl = new Gwk::Controls::ScrollControl(this);
            pCtrl->SetBounds(410, 110, 100, 100);
            pCtrl->SetAutoHideBars(true);
            pCtrl->SetScroll(false, true);
            Controls::Button* pTestButton = new Controls::Button(pCtrl);
            pTestButton->SetText("Vertical");
            pTestButton->SetBounds(0, 0, 200, 200);
        }
        {
            Gwk::Controls::ScrollControl* pCtrl = new Gwk::Controls::ScrollControl(this);
            pCtrl->SetBounds(510, 110, 100, 100);
            pCtrl->SetAutoHideBars(true);
            pCtrl->SetScroll(true, false);
            Controls::Button* pTestButton = new Controls::Button(pCtrl);
            pTestButton->SetText("Horinzontal");
            pTestButton->SetBounds(0, 0, 200, 200);
        }
    }

};


DEFINE_UNIT_TEST(ScrollControl);
