/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/ColorPicker.h>
#include <Gwork/Controls/HSVColorPicker.h>

using namespace Gwk;

class ColorPicker : public TestUnit
{
public:

    GWK_CONTROL_INLINE(ColorPicker, TestUnit)
    {
        Controls::ColorPicker* ctrl = new Controls::ColorPicker(this);
        ctrl->SetBounds(0, 10, 250, 150);
        Controls::HSVColorPicker* hSVCtrl = new Controls::HSVColorPicker(this);
        hSVCtrl->SetBounds(260, 10, 250, 150);
    }
};


DECLARE_TEST(ColorPicker);
