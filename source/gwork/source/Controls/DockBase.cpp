/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/DockBase.h>
#include <Gwork/Controls/DockedTabControl.h>
#include <Gwork/Controls/Highlight.h>
#include <Gwork/DragAndDrop.h>
#include <Gwork/Controls/Resizer.h>

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(DockBase)
{
    SetPadding(Padding(1, 1, 1, 1));
    SetSize(200, 200);
    m_dockedTabControl = nullptr;
    m_left = nullptr;
    m_right = nullptr;
    m_top = nullptr;
    m_bottom = nullptr;
    m_bDrawHover = false;
}

TabControl* DockBase::GetTabControl()
{
    return m_dockedTabControl;
}

void DockBase::SetupChildDock(Position pos)
{
    if (!m_dockedTabControl)
    {
        m_dockedTabControl = new DockedTabControl(this);
        m_dockedTabControl->onLoseTab.Add(this, &DockBase::OnTabRemoved);
        m_dockedTabControl->SetTabStripPosition(Position::Bottom);
        m_dockedTabControl->SetShowTitlebar(true);
    }

    Dock(pos);
    Position sizeDirection = Position::Left;

    if (pos == Position::Left)
        sizeDirection = Position::Right;

    if (pos == Position::Top)
        sizeDirection = Position::Bottom;

    if (pos == Position::Bottom)
        sizeDirection = Position::Top;

    ControlsInternal::Resizer* sizer = new ControlsInternal::Resizer(this);
    sizer->Dock(sizeDirection);
    sizer->SetResizeDir(sizeDirection);
    sizer->SetSize(2, 2);
    sizer->SetTarget(this);
}

void DockBase::Render(Skin::Base* /*skin*/)
{
    // Gwk::Render->SetDrawColor( Colors::Black );
    // Gwk::Render->DrawLinedRect( GetRenderBounds() );
}

DockBase** DockBase::GetChildDockPtr(Position pos)
{
    if (pos == Position::Left)
        return &m_left;

    if (pos == Position::Right)
        return &m_right;

    if (pos == Position::Top)
        return &m_top;

    if (pos == Position::Bottom)
        return &m_bottom;

    return nullptr;
}

DockBase* DockBase::GetChildDock(Position pos)
{
    DockBase** dock = GetChildDockPtr(pos);

    if (!(*dock))
    {
        (*dock) = new DockBase(this);
        (*dock)->SetupChildDock(pos);
    }
    else
    {
        (*dock)->SetHidden(false);
    }

    return *dock;
}

Position DockBase::GetDroppedTabDirection(int x, int y)
{
    const int w = Width();
    const int h = Height();
    float top = (float)y/(float)h;
    float left = (float)x/(float)w;
    float right = (float)(w-x)/(float)w;
    float bottom = (float)(h-y)/(float)h;
    float minimum = std::min(std::min(std::min(top, left), right), bottom);
    m_bDropFar = (minimum < 0.2f);

    if (minimum > 0.3)
        return Position::Fill;

    if (top == minimum && (!m_top || m_top->Hidden()))
        return Position::Top;

    if (left == minimum && (!m_left || m_left->Hidden()))
        return Position::Left;

    if (right == minimum && (!m_right || m_right->Hidden()))
        return Position::Right;

    if (bottom == minimum && (!m_bottom || m_bottom->Hidden()))
        return Position::Bottom;

    return Position::Fill;
}

bool DockBase::DragAndDrop_CanAcceptPackage(Gwk::DragAndDrop::Package* package)
{
    // A TAB button dropped
    if (package->name == "TabButtonMove")
        return true;

    // a TAB window dropped
    if (package->name == "TabWindowMove")
        return true;

    return false;
}

bool DockBase::DragAndDrop_HandleDrop(Gwk::DragAndDrop::Package* package, int x, int y)
{
    Gwk::Point pos = CanvasPosToLocal(Gwk::Point(x, y));
    Position dir = GetDroppedTabDirection(pos.x, pos.y);
    DockedTabControl* addTo = m_dockedTabControl;

    if (dir == Position::Fill && addTo == nullptr)
        return false;

    if (dir != Position::Fill)
    {
        DockBase* dock = GetChildDock(dir);
        addTo = dock->m_dockedTabControl;

        if (!m_bDropFar)
            dock->BringToFront();
        else
            dock->SendToBack();
    }

    if (package->name == "TabButtonMove")
    {
        TabButton* tabButton = gwk_cast<TabButton>(DragAndDrop::SourceControl);

        if (!tabButton)
            return false;

        addTo->AddPage(tabButton);
    }

    if (package->name == "TabWindowMove")
    {
        DockedTabControl* tabControl = gwk_cast<DockedTabControl>(DragAndDrop::SourceControl);

        if (!tabControl)
            return false;

        if (tabControl == addTo)
            return false;

        tabControl->MoveTabsTo(addTo);
    }

    Invalidate();
    return true;
}

bool DockBase::IsEmpty()
{
    if (m_dockedTabControl && m_dockedTabControl->TabCount() > 0)
        return false;

    if (m_left && !m_left->IsEmpty())
        return false;

    if (m_right && !m_right->IsEmpty())
        return false;

    if (m_top && !m_top->IsEmpty())
        return false;

    if (m_bottom && !m_bottom->IsEmpty())
        return false;

    return true;
}

void DockBase::OnTabRemoved(Event::Info)
{
    DoRedundancyCheck();
    DoConsolidateCheck();
}

void DockBase::DoRedundancyCheck()
{
    if (!IsEmpty())
        return;

    DockBase* dockParent = gwk_cast<DockBase>(GetParent());

    if (!dockParent)
        return;

    dockParent->OnRedundantChildDock(this);
}

void DockBase::DoConsolidateCheck()
{
    if (IsEmpty())
        return;

    if (!m_dockedTabControl)
        return;

    if (m_dockedTabControl->TabCount() > 0)
        return;

    if (m_bottom && !m_bottom->IsEmpty())
    {
        m_bottom->m_dockedTabControl->MoveTabsTo(m_dockedTabControl);
        return;
    }

    if (m_top && !m_top->IsEmpty())
    {
        m_top->m_dockedTabControl->MoveTabsTo(m_dockedTabControl);
        return;
    }

    if (m_left && !m_left->IsEmpty())
    {
        m_left->m_dockedTabControl->MoveTabsTo(m_dockedTabControl);
        return;
    }

    if (m_right && !m_right->IsEmpty())
    {
        m_right->m_dockedTabControl->MoveTabsTo(m_dockedTabControl);
        return;
    }
}

void DockBase::OnRedundantChildDock(DockBase* dockBase)
{
    dockBase->SetHidden(true);
    DoRedundancyCheck();
    DoConsolidateCheck();
}

void DockBase::DragAndDrop_HoverEnter(Gwk::DragAndDrop::Package* /*package*/, int /*x*/,
                                      int /*y*/)
{
    m_bDrawHover = true;
}

void DockBase::DragAndDrop_HoverLeave(Gwk::DragAndDrop::Package* /*package*/)
{
    m_bDrawHover = false;
}

void DockBase::DragAndDrop_Hover(Gwk::DragAndDrop::Package* /*package*/, int x, int y)
{
    Gwk::Point pos = CanvasPosToLocal(Gwk::Point(x, y));
    Position dir = GetDroppedTabDirection(pos.x, pos.y);

    if (dir == Position::Fill)
    {
        if (!m_dockedTabControl)
        {
            m_hoverRect = Gwk::Rect(0, 0, 0, 0);
            return;
        }

        m_hoverRect = GetInnerBounds();
        return;
    }

    m_hoverRect = GetRenderBounds();
    int HelpBarWidth = 0;

    if (dir == Position::Left)
    {
        HelpBarWidth = m_hoverRect.w/4;
        m_hoverRect.w = HelpBarWidth;
    }

    if (dir == Position::Right)
    {
        HelpBarWidth = m_hoverRect.w/4;
        m_hoverRect.x = m_hoverRect.w-HelpBarWidth;
        m_hoverRect.w = HelpBarWidth;
    }

    if (dir == Position::Top)
    {
        HelpBarWidth = m_hoverRect.h/4;
        m_hoverRect.h = HelpBarWidth;
    }

    if (dir == Position::Bottom)
    {
        HelpBarWidth = m_hoverRect.h/4;
        m_hoverRect.y = m_hoverRect.h-HelpBarWidth;
        m_hoverRect.h = HelpBarWidth;
    }

    if ((dir == Position::Top || dir == Position::Bottom) && !m_bDropFar)
    {
        if (m_left && m_left->Visible())
        {
            m_hoverRect.x += m_left->Width();
            m_hoverRect.w -= m_left->Width();
        }

        if (m_right && m_right->Visible())
            m_hoverRect.w -= m_right->Width();
    }

    if ((dir == Position::Left || dir == Position::Right) && !m_bDropFar)
    {
        if (m_top && m_top->Visible())
        {
            m_hoverRect.y += m_top->Height();
            m_hoverRect.h -= m_top->Height();
        }

        if (m_bottom && m_bottom->Visible())
            m_hoverRect.h -= m_bottom->Height();
    }
}

void DockBase::RenderOver(Skin::Base* skin)
{
    if (!m_bDrawHover)
        return;

    Gwk::Renderer::Base* render = skin->GetRender();
    render->SetDrawColor(Gwk::Color(255, 100, 255, 20));
    render->DrawFilledRect(GetRenderBounds());

    if (m_hoverRect.w == 0)
        return;

    render->SetDrawColor(Gwk::Color(255, 100, 255, 100));
    render->DrawFilledRect(m_hoverRect);
    render->SetDrawColor(Gwk::Color(255, 100, 255, 200));
    render->DrawLinedRect(m_hoverRect);
}
