/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/Slider.h>
#include <Gwork/Controls/HorizontalSlider.h>

using namespace Gwk;
using namespace Gwk::Controls;
using namespace Gwk::ControlsInternal;


GWK_CONTROL_CONSTRUCTOR(HorizontalSlider)
{
    m_sliderBar->SetHorizontal(true);
}

float HorizontalSlider::CalculateValue()
{
    return static_cast<float>(m_sliderBar->X()) / (Width() - m_sliderBar->Width());
}

void HorizontalSlider::UpdateBarFromValue()
{
    m_sliderBar->MoveTo((Width()-m_sliderBar->Width())*(m_fValue), m_sliderBar->Y());
}

void HorizontalSlider::OnMouseClickLeft(int x, int y, bool bDown)
{
    m_sliderBar->MoveTo( CanvasPosToLocal(Gwk::Point(x,y)).x - m_sliderBar->Width()/2,
                         m_sliderBar->Y() );
    m_sliderBar->OnMouseClickLeft(x, y, bDown);
    OnMoved(m_sliderBar);
}

void HorizontalSlider::Layout(Skin::Base* /*skin*/)
{
    m_sliderBar->SetSize(15, Height());
}

void HorizontalSlider::Render(Skin::Base* skin)
{
    skin->DrawSlider(this, true, m_bClampToNotches ? m_numNotches : 0, m_sliderBar->Width());
}
