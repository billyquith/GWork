/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include "Gwork/Test/Test.h"
#include "Gwork/Controls/RadioButtonController.h"
#include "Gwork/Controls/VerticalSlider.h"
#include "Gwork/Controls/HorizontalSlider.h"

using namespace Gwk;

class Slider : public GUnit
{
public:

    GWK_CONTROL_INLINE(Slider, GUnit)
    {
        {
            Gwk::Controls::HorizontalSlider* pSlider = new Gwk::Controls::HorizontalSlider(this);
            pSlider->SetPos(10, 10);
            pSlider->SetSize(150, 20);
            pSlider->SetRange(0, 100);
            pSlider->SetFloatValue(25);
            pSlider->onValueChanged.Add(this, &Slider::SliderMoved);
        }
        {
            Gwk::Controls::HorizontalSlider* pSlider = new Gwk::Controls::HorizontalSlider(this);
            pSlider->SetPos(10, 40);
            pSlider->SetSize(150, 20);
            pSlider->SetRange(0, 100);
            pSlider->SetFloatValue(20);
            pSlider->SetNotchCount(10);
            pSlider->SetClampToNotches(true);
            pSlider->onValueChanged.Add(this, &Slider::SliderMoved);
        }
        {
            Gwk::Controls::VerticalSlider* pSlider = new Gwk::Controls::VerticalSlider(this);
            pSlider->SetPos(160, 10);
            pSlider->SetSize(20, 200);
            pSlider->SetRange(0, 100);
            pSlider->SetFloatValue(25);
            pSlider->onValueChanged.Add(this, &Slider::SliderMoved);
        }
        {
            Gwk::Controls::VerticalSlider* pSlider = new Gwk::Controls::VerticalSlider(this);
            pSlider->SetPos(190, 10);
            pSlider->SetSize(20, 200);
            pSlider->SetRange(0, 100);
            pSlider->SetFloatValue(20);
            pSlider->SetNotchCount(10);
            pSlider->SetClampToNotches(true);
            pSlider->onValueChanged.Add(this, &Slider::SliderMoved);
        }
    }

    void SliderMoved(Base* pControl)
    {
        Gwk::Controls::Slider* pSlider = (Gwk::Controls::Slider*)pControl;
        UnitPrint(Utility::Format("Slider Value: %.2f", (float)pSlider->GetFloatValue()));
    }

};


DEFINE_UNIT_TEST(Slider);
