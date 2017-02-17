/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/NumericUpDown.h>

using namespace Gwk;

class Numeric : public TestUnit
{
public:

    GWK_CONTROL_INLINE(Numeric, TestUnit)
    {
        Controls::NumericUpDown* ctrl = new Controls::NumericUpDown(this);
        ctrl->SetBounds(10, 10, 50, 20);
        ctrl->SetIntValue(50);
        ctrl->SetMax(1000);
        ctrl->SetMin(-1000);
        //	ctrl->onPress.Add( this, &ThisClass::onButtonA );
    }

    void onButtonA(Controls::Base* control)
    {
        //	OutputToLog( "Button Pressed (using 'OnPress' event)" );
    }

};


DECLARE_TEST(Numeric);
