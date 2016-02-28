/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include "Gwork/Test/Test.h"
#include "Gwork/Controls/NumericUpDown.h"

using namespace Gwk;

class Numeric : public GUnit
{
public:

    GWK_CONTROL_INLINE(Numeric, GUnit)
    {
        Controls::NumericUpDown* pCtrl = new Controls::NumericUpDown(this);
        pCtrl->SetBounds(10, 10, 50, 20);
        pCtrl->SetIntValue(50);
        pCtrl->SetMax(1000);
        pCtrl->SetMin(-1000);
        //	pCtrl->onPress.Add( this, &ThisClass::onButtonA );
    }

    void onButtonA(Controls::Base* pControl)
    {
        //	UnitPrint( "Button Pressed (using 'OnPress' event)" );
    }

};


DEFINE_UNIT_TEST(Numeric);
