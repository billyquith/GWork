/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Controls/Slider.h>
#include <Gwork/Controls/HorizontalSlider.h>

using namespace Gwk;
using namespace Gwk::Controls;
using namespace Gwk::ControlsInternal;


GWK_CONTROL_CONSTRUCTOR(HorizontalSlider)
{
    m_SliderBar->SetHorizontal(true);
}

float HorizontalSlider::CalculateValue()
{
    return (float)m_SliderBar->X()/(float)(Width()-m_SliderBar->Width());
}

void HorizontalSlider::UpdateBarFromValue()
{
    m_SliderBar->MoveTo((Width()-m_SliderBar->Width())*(m_fValue), m_SliderBar->Y());
}

void HorizontalSlider::OnMouseClickLeft(int x, int y, bool bDown)
{
    m_SliderBar->MoveTo( CanvasPosToLocal(Gwk::Point(x,y)).x - m_SliderBar->Width()/2,
                         m_SliderBar->Y() );
    m_SliderBar->OnMouseClickLeft(x, y, bDown);
    OnMoved(m_SliderBar);
}

void HorizontalSlider::Layout(Skin::Base* /*skin*/)
{
    m_SliderBar->SetSize(15, Height());
}

void HorizontalSlider::Render(Skin::Base* skin)
{
    skin->DrawSlider(this, true, m_bClampToNotches ? m_iNumNotches : 0, m_SliderBar->Width());
}
