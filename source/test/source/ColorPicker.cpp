/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include "Gwork/Test/Test.h"
#include "Gwork/Controls/ColorPicker.h"
#include "Gwork/Controls/HSVColorPicker.h"

using namespace Gwk;

class ColorPicker : public GUnit
{
public:

    GWK_CONTROL_INLINE(ColorPicker, GUnit)
    {
        Controls::ColorPicker* pCtrl = new Controls::ColorPicker(this);
        pCtrl->SetBounds(0, 10, 250, 150);
        Controls::HSVColorPicker* pHSVCtrl = new Controls::HSVColorPicker(this);
        pHSVCtrl->SetBounds(260, 10, 250, 150);
    }
};


DEFINE_UNIT_TEST(ColorPicker);
