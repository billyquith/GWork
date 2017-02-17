/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/ScrollBar.h>
#include <Gwork/Controls/ScrollBarButton.h>
#include <Gwork/Controls/ScrollBarBar.h>

using namespace Gwk;
using namespace Gwk::Controls;
using namespace Gwk::ControlsInternal;


GWK_CONTROL_CONSTRUCTOR(BaseScrollBar)
{
    for (int i = 0; i < 2; i++)
    {
        m_scrollButton[i] = new ScrollBarButton(this);
    }

    m_bar = new ScrollBarBar(this);
    SetBounds(0, 0, 15, 15);
    m_bDepressed = false;
    m_fScrolledAmount = 0;
    m_fContentSize = 0;
    m_fViewableContentSize = 0;
    SetNudgeAmount(20);
}

void BaseScrollBar::Render(Skin::Base* skin)
{
    skin->DrawScrollBar(this, IsHorizontal(), m_bDepressed);
}

void BaseScrollBar::OnBarMoved(Event::Info)
{
    onBarMoved.Call(this);
}

void BaseScrollBar::BarMovedNotification()
{
    OnBarMoved(this);
}

void BaseScrollBar::SetContentSize(float size)
{
    if (m_fContentSize != size)
        Invalidate();

    m_fContentSize = size;
}

void BaseScrollBar::SetViewableContentSize(float size)
{
    if (m_fViewableContentSize != size)
        Invalidate();

    m_fViewableContentSize = size;
}

bool BaseScrollBar::SetScrolledAmount(float amount, bool forceUpdate)
{
    if (m_fScrolledAmount == amount && !forceUpdate)
        return false;

    m_fScrolledAmount = amount;
    Invalidate();
    BarMovedNotification();
    return true;
}
