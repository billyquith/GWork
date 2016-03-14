/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Controls/ScrollBar.h>
#include <Gwork/Controls/VerticalScrollBar.h>

using namespace Gwk;
using namespace Gwk::Controls;

GWK_CONTROL_CONSTRUCTOR(VerticalScrollBar)
{
    m_bar->SetVertical();
    m_scrollButton[SCROLL_BUTTON_UP]->SetDirectionUp();
    m_scrollButton[SCROLL_BUTTON_UP]->onPress.Add(this, &VerticalScrollBar::NudgeUp);
    m_scrollButton[SCROLL_BUTTON_DOWN]->SetDirectionDown();
    m_scrollButton[SCROLL_BUTTON_DOWN]->onPress.Add(this, &VerticalScrollBar::NudgeDown);
    m_bar->onDragged.Add(this, &VerticalScrollBar::OnBarMoved);
}

void VerticalScrollBar::Layout(Skin::Base* skin)
{
    ParentClass::Layout(skin);
    m_scrollButton[SCROLL_BUTTON_UP]->Dock(Position::Top);
    m_scrollButton[SCROLL_BUTTON_UP]->SetHeight(Width());
    m_scrollButton[SCROLL_BUTTON_DOWN]->Dock(Position::Bottom);
    m_scrollButton[SCROLL_BUTTON_DOWN]->SetHeight(Width());
    m_bar->SetWidth(GetButtonSize());
    // Add padding
    m_bar->SetPadding(Padding(0, GetButtonSize(), 0, GetButtonSize()));
    // Calculate bar sizes
    float barHeight = (m_fViewableContentSize/m_fContentSize)*(Height()-GetButtonSize());

    if (barHeight < GetButtonSize()/2)
        barHeight = GetButtonSize()/2;

    m_bar->SetHeight(barHeight);
    m_bar->SetHidden(Height()-(GetButtonSize()*2) <= barHeight);

    if (Hidden())
        SetScrolledAmount(0, true);

    // Based on our last scroll amount, produce a position for the bar
    if (!m_bar->IsDepressed())
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

void VerticalScrollBar::NudgeUp(Event::Info)
{
    if (!IsDisabled())
        SetScrolledAmount(GetScrolledAmount()-GetNudgeAmount(), true);
}

void VerticalScrollBar::NudgeDown(Event::Info)
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

        if (clickPos.y < m_bar->Y())
            NudgeUp(this);
        else if (clickPos.y > m_bar->Y()+m_bar->Height())
            NudgeDown(this);

        m_bDepressed = false;
        Gwk::MouseFocus = nullptr;
    }
}

float VerticalScrollBar::CalculateScrolledAmount()
{
    return (float)(m_bar->Y()-
                   GetButtonSize())/(float)(Height()-m_bar->Height()-(GetButtonSize()*2));
}

bool VerticalScrollBar::SetScrolledAmount(float amount, bool forceUpdate)
{
    amount = Gwk::Clamp(amount, 0.f, 1.f);

    if (!ParentClass::SetScrolledAmount(amount, forceUpdate))
        return false;

    if (forceUpdate)
    {
        int newY = GetButtonSize()+(amount*((Height()-m_bar->Height())-(GetButtonSize()*2)));
        m_bar->MoveTo(m_bar->X(), newY);
    }

    return true;
}

void VerticalScrollBar::OnBarMoved(Event::Info info)
{
    if (m_bar->IsDepressed())
    {
        SetScrolledAmount(CalculateScrolledAmount(), false);
        ParentClass::OnBarMoved(info);
    }
    else
    {
        InvalidateParent();
    }
}
