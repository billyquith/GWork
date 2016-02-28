/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include "Gwork/Controls/ScrollBar.h"
#include "Gwork/Controls/VerticalScrollBar.h"

using namespace Gwk;
using namespace Gwk::Controls;

GWK_CONTROL_CONSTRUCTOR(VerticalScrollBar)
{
    m_Bar->SetVertical();
    m_ScrollButton[SCROLL_BUTTON_UP]->SetDirectionUp();
    m_ScrollButton[SCROLL_BUTTON_UP]->onPress.Add(this, &VerticalScrollBar::NudgeUp);
    m_ScrollButton[SCROLL_BUTTON_DOWN]->SetDirectionDown();
    m_ScrollButton[SCROLL_BUTTON_DOWN]->onPress.Add(this, &VerticalScrollBar::NudgeDown);
    m_Bar->onDragged.Add(this, &VerticalScrollBar::OnBarMoved);
}

void VerticalScrollBar::Layout(Skin::Base* skin)
{
    ParentClass::Layout(skin);
    m_ScrollButton[SCROLL_BUTTON_UP]->Dock(Docking::Top);
    m_ScrollButton[SCROLL_BUTTON_UP]->SetHeight(Width());
    m_ScrollButton[SCROLL_BUTTON_DOWN]->Dock(Docking::Bottom);
    m_ScrollButton[SCROLL_BUTTON_DOWN]->SetHeight(Width());
    m_Bar->SetWidth(GetButtonSize());
    // Add padding
    m_Bar->SetPadding(Padding(0, GetButtonSize(), 0, GetButtonSize()));
    // Calculate bar sizes
    float barHeight = (m_fViewableContentSize/m_fContentSize)*(Height()-GetButtonSize());

    if (barHeight < GetButtonSize()/2)
        barHeight = GetButtonSize()/2;

    m_Bar->SetHeight(barHeight);
    m_Bar->SetHidden(Height()-(GetButtonSize()*2) <= barHeight);

    if (Hidden())
        SetScrolledAmount(0, true);

    // Based on our last scroll amount, produce a position for the bar
    if (!m_Bar->IsDepressed())
        SetScrolledAmount(GetScrolledAmount(), true);
}

void VerticalScrollBar::ScrollToTop()
{
    SetScrolledAmount(0, true);
}

void VerticalScrollBar::ScrollToBottom()
{
    SetScrolledAmount(1, true);
}

void VerticalScrollBar::NudgeUp(Base* /*control*/)
{
    if (!IsDisabled())
        SetScrolledAmount(GetScrolledAmount()-GetNudgeAmount(), true);
}

void VerticalScrollBar::NudgeDown(Base* /*control*/)
{
    if (!IsDisabled())
        SetScrolledAmount(GetScrolledAmount()+GetNudgeAmount(), true);
}

float VerticalScrollBar::GetNudgeAmount()
{
    if (m_bDepressed)
        return m_fViewableContentSize/m_fContentSize;
    else
        return ParentClass::GetNudgeAmount();
}

void VerticalScrollBar::OnMouseClickLeft(int x, int y, bool bDown)
{
    if (bDown)
    {
        m_bDepressed = true;
        Gwk::MouseFocus = this;
    }
    else
    {
        Gwk::Point clickPos = CanvasPosToLocal(Gwk::Point(x, y));

        if (clickPos.y < m_Bar->Y())
            NudgeUp(this);
        else if (clickPos.y > m_Bar->Y()+m_Bar->Height())
            NudgeDown(this);

        m_bDepressed = false;
        Gwk::MouseFocus = NULL;
    }
}

float VerticalScrollBar::CalculateScrolledAmount()
{
    return (float)(m_Bar->Y()-
                   GetButtonSize())/(float)(Height()-m_Bar->Height()-(GetButtonSize()*2));
}

bool VerticalScrollBar::SetScrolledAmount(float amount, bool forceUpdate)
{
    amount = Gwk::Clamp(amount, 0.f, 1.f);

    if (!ParentClass::SetScrolledAmount(amount, forceUpdate))
        return false;

    if (forceUpdate)
    {
        int newY = GetButtonSize()+(amount*((Height()-m_Bar->Height())-(GetButtonSize()*2)));
        m_Bar->MoveTo(m_Bar->X(), newY);
    }

    return true;
}

void VerticalScrollBar::OnBarMoved(Controls::Base* control)
{
    if (m_Bar->IsDepressed())
    {
        SetScrolledAmount(CalculateScrolledAmount(), false);
        ParentClass::OnBarMoved(control);
    }
    else
    {
        InvalidateParent();
    }
}
