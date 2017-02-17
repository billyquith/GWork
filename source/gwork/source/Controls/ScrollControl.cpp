/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/ScrollControl.h>
#include <Gwork/Controls/ScrollBar.h>
#include <Gwork/Controls/VerticalScrollBar.h>
#include <Gwork/Controls/HorizontalScrollBar.h>
#include <Gwork/Utility.h>

using namespace Gwk;
using namespace Gwk::Controls;
using namespace Gwk::ControlsInternal;

GWK_CONTROL_CONSTRUCTOR(ScrollControl)
{
    SetMouseInputEnabled(false);
    m_verticalScrollBar = new VerticalScrollBar(this);
    m_verticalScrollBar->Dock(Position::Right);
    m_verticalScrollBar->onBarMoved.Add(this, &ScrollControl::OnVBarMoved);
    m_verticalScrollBar->SetNudgeAmount(30);
    m_bCanScrollV = true;
    m_horizontalScrollBar = new HorizontalScrollBar(this);
    m_horizontalScrollBar->Dock(Position::Bottom);
    m_horizontalScrollBar->onBarMoved.Add(this, &ScrollControl::OnHBarMoved);
    m_bCanScrollH = true;
    m_horizontalScrollBar->SetNudgeAmount(30);
    m_innerPanel = new Base(this);
    m_innerPanel->SetPos(0, 0);
    m_innerPanel->SetMargin(Margin(5, 5, 5, 5));
    m_innerPanel->SendToBack();
    m_innerPanel->SetMouseInputEnabled(true);
    m_bAutoHideBars = true;
}

void ScrollControl::SetScroll(bool h, bool v)
{
    m_bCanScrollV = v;
    m_bCanScrollH = h;
    m_verticalScrollBar->SetHidden(!m_bCanScrollV);
    m_horizontalScrollBar->SetHidden(!m_bCanScrollH);
}

void ScrollControl::SetInnerSize(int w, int h)
{
    m_innerPanel->SetSize(w, h);
}

void ScrollControl::OnVBarMoved(Event::Info)
{
    Invalidate();
}

void ScrollControl::OnHBarMoved(Event::Info)
{
    Invalidate();
}

void ScrollControl::OnChildBoundsChanged(Gwk::Rect /*oldChildBounds*/, Base* /*child*/)
{
    UpdateScrollBars();
    Invalidate();
}

void ScrollControl::Layout(Skin::Base* skin)
{
    UpdateScrollBars();
    ParentClass::Layout(skin);
}

bool ScrollControl::OnMouseWheeled(int iDelta)
{
    if (CanScrollV() && m_verticalScrollBar->Visible())
    {
        if (m_verticalScrollBar->SetScrolledAmount(
                m_verticalScrollBar->GetScrolledAmount()
                    - m_verticalScrollBar->GetNudgeAmount() * iDelta / 60.0f, true))
            return true;
    }

    if (CanScrollH() && m_horizontalScrollBar->Visible())
    {
        if (m_horizontalScrollBar->SetScrolledAmount(
                m_horizontalScrollBar->GetScrolledAmount()
                        - m_horizontalScrollBar->GetNudgeAmount() * iDelta / 60.0f, true))
            return true;
    }

    return false;
}

void ScrollControl::Render(Skin::Base* skin)
{
#if 0
    // Debug render - this shouldn't render ANYTHING REALLY - it should be up to
    // the parent!
    Gwk::Rect rect = GetRenderBounds();
    Gwk::Renderer::Base* render = skin->GetRender();
    render->SetDrawColor(Gwk::Color(255, 255, 0, 100));
    render->DrawFilledRect(rect);
    render->SetDrawColor(Gwk::Color(255, 0, 0, 100));
    render->DrawFilledRect(m_innerPanel->GetBounds());
    render->RenderText(skin->GetDefaultFont(),
                       Gwk::Point(0,0),
                       Utility::Format("Offset: %i %i", m_innerPanel->X(), m_innerPanel->Y()));
#else // 0
    (void)skin;
#endif // 0
}

bool ScrollControl::ContentsAreDocked()
{
    if (!m_innerPanel)
        return false;

    for (auto&& child : m_innerPanel->Children)
    {
        if (child->GetDock() == Position::None)
            return false;
    }

    return true;
}

void ScrollControl::UpdateScrollBars()
{
    if (!m_innerPanel)
        return;

    // Not sure what this is here to fix/change. Adding it breaks auto-scrollbars for
    // menus, controls, etc. -- BQ
    //
    //    if (ContentsAreDocked())
    //    {
    //        m_verticalScrollBar->SetHidden(false);
    //        m_horizontalScrollBar->SetHidden(false);
    //        m_innerPanel->SetSize(GetSize());
    //        m_innerPanel->SetPos(0, 0);
    //        return;
    //    }

    int childrenWidth = 0;
    int childrenHeight = 0;

    // Get the max size of all our children together
    for (auto&& child : m_innerPanel->Children)
    {
        childrenWidth = std::max(childrenWidth, child->Right());
        childrenHeight = std::max(childrenHeight, child->Bottom());
    }

    if (m_bCanScrollH)
    {
        m_innerPanel->SetSize(std::max( Width(), childrenWidth ),
                              std::max( Height(), childrenHeight ));
    }
    else
    {
        m_innerPanel->SetSize(Width() - (m_verticalScrollBar->Hidden()
                                         ? 0 : m_verticalScrollBar->Width()-1),
                              std::max(Height(), childrenHeight));
    }

    const float wPercent =
        static_cast<float>(Width()) / 
            (childrenWidth + (m_verticalScrollBar->Hidden() ? 0 : m_verticalScrollBar->Width()));
    
    const float hPercent =
        static_cast<float>(Height()) / 
            (childrenHeight 
                + (m_horizontalScrollBar->Hidden() ? 0 : m_horizontalScrollBar->Height()));

    if (m_bCanScrollV)
        SetVScrollRequired(hPercent >= 1);
    else
        m_verticalScrollBar->SetHidden(true);

    if (m_bCanScrollH)
        SetHScrollRequired(wPercent >= 1);
    else
        m_horizontalScrollBar->SetHidden(true);

    m_verticalScrollBar->SetContentSize(m_innerPanel->Height());
    m_verticalScrollBar->SetViewableContentSize(Height() - (m_horizontalScrollBar->Hidden()
                                                            ? 0 : m_horizontalScrollBar->Height()));
    m_horizontalScrollBar->SetContentSize(m_innerPanel->Width());
    m_horizontalScrollBar->SetViewableContentSize(Width() - (m_verticalScrollBar->Hidden()
                                                             ? 0 : m_verticalScrollBar->Width()));
    int newInnerPanelPosX = 0;
    int newInnerPanelPosY = 0;

    if (CanScrollV() && !m_verticalScrollBar->Hidden())
    {
        newInnerPanelPosY =
            -( m_innerPanel->Height() - Height() +
               (m_horizontalScrollBar->Hidden() ? 0 : m_horizontalScrollBar->Height()) )
            * m_verticalScrollBar->GetScrolledAmount();
    }

    if (CanScrollH() && !m_horizontalScrollBar->Hidden())
    {
        newInnerPanelPosX =
            -( m_innerPanel->Width() - Width() +
               (m_verticalScrollBar->Hidden() ? 0 : m_verticalScrollBar->Width()) )
            * m_horizontalScrollBar->GetScrolledAmount();
    }

    m_innerPanel->SetPos(newInnerPanelPosX, newInnerPanelPosY);
}

void ScrollControl::SetVScrollRequired(bool req)
{
    if (req)
    {
        m_verticalScrollBar->SetScrolledAmount(0, true);
        m_verticalScrollBar->SetDisabled(true);

        if (m_bAutoHideBars)
            m_verticalScrollBar->SetHidden(true);
    }
    else
    {
        m_verticalScrollBar->SetHidden(false);
        m_verticalScrollBar->SetDisabled(false);
    }
}

void ScrollControl::SetHScrollRequired(bool req)
{
    if (req)
    {
        m_horizontalScrollBar->SetScrolledAmount(0, true);
        m_horizontalScrollBar->SetDisabled(true);

        if (m_bAutoHideBars)
            m_horizontalScrollBar->SetHidden(true);
    }
    else
    {
        m_horizontalScrollBar->SetHidden(false);
        m_horizontalScrollBar->SetDisabled(true);
    }
}

void ScrollControl::ScrollToBottom()
{
    if (!CanScrollV())
        return;

    UpdateScrollBars();
    m_verticalScrollBar->ScrollToBottom();
}

void ScrollControl::ScrollToTop()
{
    if (CanScrollV())
    {
        UpdateScrollBars();
        m_verticalScrollBar->ScrollToTop();
    }
}

void ScrollControl::ScrollToLeft()
{
    if (CanScrollH())
    {
        UpdateScrollBars();
        m_horizontalScrollBar->ScrollToLeft();
    }
}

void ScrollControl::ScrollToRight()
{
    if (CanScrollH())
    {
        UpdateScrollBars();
        m_horizontalScrollBar->ScrollToRight();
    }
}

void ScrollControl::Clear()
{
    m_innerPanel->RemoveAllChildren();
}
