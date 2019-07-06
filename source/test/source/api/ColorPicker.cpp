/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/ColorPicker.h>
#include <Gwork/Controls/HSVColorPicker.h>

namespace Gwk {
namespace Test {

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

GWK_REGISTER_TEST(ColorPicker);

}}

