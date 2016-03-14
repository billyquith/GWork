/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Controls/Slider.h>
#include <cmath>

namespace Gwk
{
    
using namespace Gwk::Controls;
using namespace Gwk::ControlsInternal;

GWK_CONTROL_CONSTRUCTOR(SliderBar)
{
    SetTarget(this);
    RestrictToParent(true);
}

void SliderBar::Render(Skin::Base* skin)
{
    skin->DrawSlideButton(this, IsDepressed(), IsHorizontal());
}

GWK_CONTROL_CONSTRUCTOR(Slider)
{
    SetBounds(Gwk::Rect(0, 0, 32, 128));
    m_sliderBar = new SliderBar(this);
    m_sliderBar->onDragged.Add(this, &Slider::OnMoved);
    m_fMin = 0.0f;
    m_fMax = 1.0f;
    m_bClampToNotches = false;
    m_numNotches = 5;
    m_fValue = 0.0f;
    SetTabable(true);
}

void Slider::OnMoved(Event::Info)
{
    SetValueInternal(CalculateValue());
}

void Slider::Layout(Skin::Base* skin)
{
    ParentClass::Layout(skin);
}

float Slider::CalculateValue()
{
    return 0;
}

void Slider::SetFloatValue(float val, bool /*forceUpdate*/)
{
    val = Clamp(val, m_fMin, m_fMax);

    // Normalize Value
    val = (val-m_fMin)/(m_fMax-m_fMin);
    SetValueInternal(val);
    Redraw();
}

void Slider::SetValueInternal(float val)
{
    if (m_bClampToNotches)
    {
        val = std::floor(val * float(m_numNotches) + 0.5f);
        val /= float(m_numNotches);
    }

    if (m_fValue != val)
    {
        m_fValue = val;
        onValueChanged.Call(this);
    }

    UpdateBarFromValue();
}

float Slider::GetFloatValue()
{
    return m_fMin+(m_fValue*(m_fMax-m_fMin));
}

void Slider::SetRange(float fMin, float fMax)
{
    m_fMin = fMin;
    m_fMax = fMax;
}

void Slider::RenderFocus(Gwk::Skin::Base* skin)
{
    if (Gwk::KeyboardFocus != this)
        return;

    if (!IsTabable())
        return;

    skin->DrawKeyboardHighlight(this, GetRenderBounds(), 0);
}

} // namespace Gwk
