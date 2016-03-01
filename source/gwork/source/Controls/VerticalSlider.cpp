/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Controls/Slider.h>
#include <Gwork/Controls/VerticalSlider.h>

using namespace Gwk;
using namespace Gwk::Controls;
using namespace Gwk::ControlsInternal;


GWK_CONTROL_CONSTRUCTOR(VerticalSlider)
{
    m_sliderBar->SetHorizontal(false);
}

float VerticalSlider::CalculateValue()
{
    return 1-(float)m_sliderBar->Y()/(float)(Height()-m_sliderBar->Height());
}

void VerticalSlider::UpdateBarFromValue()
{
    m_sliderBar->MoveTo(m_sliderBar->X(), (Height()-m_sliderBar->Height())*(1-m_fValue));
}

void VerticalSlider::OnMouseClickLeft(int x, int y, bool bDown)
{
    m_sliderBar->MoveTo(m_sliderBar->X(), CanvasPosToLocal(Gwk::Point(x,
                                                                       y)).y-m_sliderBar->Height()*
                        0.5);
    m_sliderBar->OnMouseClickLeft(x, y, bDown);
    OnMoved(m_sliderBar);
}

void VerticalSlider::Layout(Skin::Base* /*skin*/)
{
    m_sliderBar->SetSize(Width(), 15);
}

void VerticalSlider::Render(Skin::Base* skin)
{
    skin->DrawSlider(this, false, m_bClampToNotches ? m_numNotches : 0, m_sliderBar->Height());
}
