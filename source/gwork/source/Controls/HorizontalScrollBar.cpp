/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/ScrollBar.h>
#include <Gwork/Controls/HorizontalScrollBar.h>

using namespace Gwk;
using namespace Gwk::Controls;

GWK_CONTROL_CONSTRUCTOR(HorizontalScrollBar)
{
    m_bar->SetHorizontal();
    m_scrollButton[SCROLL_BUTTON_LEFT]->SetDirectionLeft();
    m_scrollButton[SCROLL_BUTTON_LEFT]->onPress.Add(this, &HorizontalScrollBar::NudgeLeft);
    m_scrollButton[SCROLL_BUTTON_RIGHT]->SetDirectionRight();
    m_scrollButton[SCROLL_BUTTON_RIGHT]->onPress.Add(this, &HorizontalScrollBar::NudgeRight);
    m_bar->onDragged.Add(this, &HorizontalScrollBar::OnBarMoved);
}

void HorizontalScrollBar::Layout(Skin::Base* skin)
{
    ParentClass::Layout(skin);
    m_scrollButton[SCROLL_BUTTON_LEFT]->SetWidth(Height());
    m_scrollButton[SCROLL_BUTTON_LEFT]->Dock(Position::Left);
    m_scrollButton[SCROLL_BUTTON_RIGHT]->SetWidth(Height());
    m_scrollButton[SCROLL_BUTTON_RIGHT]->Dock(Position::Right);
    m_bar->SetHeight(GetButtonSize());
    m_bar->SetPadding(Padding(GetButtonSize(), 0, GetButtonSize(), 0));
    float barWidth = (m_fViewableContentSize/m_fContentSize)*(Width()-GetButtonSize());

    if (barWidth < GetButtonSize()/2)
        barWidth = GetButtonSize()/2;

    m_bar->SetWidth(barWidth);
    m_bar->SetHidden(Width()-(GetButtonSize()*2) <= barWidth);

    // Based on our last scroll amount, produce a position for the bar
    if (!m_bar->IsDepressed())
        SetScrolledAmount(GetScrolledAmount(), true);
}

void HorizontalScrollBar::NudgeLeft(Event::Info)
{
    if (!IsDisabled())
        SetScrolledAmount(GetScrolledAmount()-GetNudgeAmount(), true);
}

void HorizontalScrollBar::NudgeRight(Event::Info)
{
    if (!IsDisabled())
        SetScrolledAmount(GetScrolledAmount()+GetNudgeAmount(), true);
}

void HorizontalScrollBar::ScrollToLeft()
{
    SetScrolledAmount(0, true);
}

void HorizontalScrollBar::ScrollToRight()
{
    SetScrolledAmount(1, true);
}

float HorizontalScrollBar::GetNudgeAmount()
{
    if (m_bDepressed)
        return m_fViewableContentSize/m_fContentSize;
    else
        return ParentClass::GetNudgeAmount();
}

void HorizontalScrollBar::OnMouseClickLeft(int x, int y, bool bDown)
{
    if (bDown)
    {
        m_bDepressed = true;
        Gwk::MouseFocus = this;
    }
    else
    {
        Gwk::Point clickPos = CanvasPosToLocal(Gwk::Point(x, y));

        if (clickPos.x < m_bar->X())
            NudgeLeft(this);
        else if (clickPos.x > m_bar->X()+m_bar->Width())
            NudgeRight(this);

        m_bDepressed = false;
        Gwk::MouseFocus = nullptr;
    }
}

float HorizontalScrollBar::CalculateScrolledAmount()
{
    return static_cast<float>(m_bar->X() - GetButtonSize())
                                / (Width() - m_bar->Width() - GetButtonSize()*2);
}

bool HorizontalScrollBar::SetScrolledAmount(float amount, bool forceUpdate)
{
    amount = Gwk::Clamp(amount, 0.f, 1.f);

    if (!ParentClass::SetScrolledAmount(amount, forceUpdate))
        return false;

    if (forceUpdate)
    {
        int newX = GetButtonSize()+(amount*((Width()-m_bar->Width())-(GetButtonSize()*2)));
        m_bar->MoveTo(newX, m_bar->Y());
    }

    return true;
}

void HorizontalScrollBar::OnBarMoved(Event::Info info)
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
