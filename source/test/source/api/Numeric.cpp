/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/NumericUpDown.h>

namespace Gwk {
namespace Test {

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
        ctrl->onChanged.Add(this, &ThisClass::OnChanged);
    }

    void OnChanged(Event::Info info)
    {
        auto control = info.Control;
        Gwk::Controls::NumericUpDown* numeric = (Gwk::Controls::NumericUpDown*)control;
        OutputToLog(Utility::Format("Numeric Changed: %i", numeric->GetIntValue()));
    }

};

GWK_REGISTER_TEST(Numeric);

}}

