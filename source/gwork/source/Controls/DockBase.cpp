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

void DockBase::SetupChildDock(Docking::Area pos)
{
    if (!m_dockedTabControl)
    {
        m_dockedTabControl = new DockedTabControl(this);
        m_dockedTabControl->onLoseTab.Add(this, &DockBase::OnTabRemoved);
        m_dockedTabControl->SetTabStripPosition(Docking::Bottom);
        m_dockedTabControl->SetShowTitlebar(true);
    }

    Dock(pos);
    Docking::Area iSizeDirection = Docking::Left;

    if (pos == Docking::Left)
        iSizeDirection = Docking::Right;

    if (pos == Docking::Top)
        iSizeDirection = Docking::Bottom;

    if (pos == Docking::Bottom)
        iSizeDirection = Docking::Top;

    ControlsInternal::Resizer* sizer = new ControlsInternal::Resizer(this);
    sizer->Dock(iSizeDirection);
    sizer->SetResizeDir(iSizeDirection);
    sizer->SetSize(2, 2);
    sizer->SetTarget(this);
}

void DockBase::Render(Skin::Base* /*skin*/)
{
    // Gwk::Render->SetDrawColor( Colors::Black );
    // Gwk::Render->DrawLinedRect( GetRenderBounds() );
}

DockBase** DockBase::GetChildDockPtr(Docking::Area pos)
{
    if (pos == Docking::Left)
        return &m_left;

    if (pos == Docking::Right)
        return &m_right;

    if (pos == Docking::Top)
        return &m_top;

    if (pos == Docking::Bottom)
        return &m_bottom;

    return nullptr;
}

DockBase* DockBase::GetChildDock(Docking::Area pos)
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

Docking::Area DockBase::GetDroppedTabDirection(int x, int y)
{
    int w = Width();
    int h = Height();
    float top = (float)y/(float)h;
    float left = (float)x/(float)w;
    float right = (float)(w-x)/(float)w;
    float bottom = (float)(h-y)/(float)h;
    float minimum = Gwk::Min(Gwk::Min(Gwk::Min(top, left), right), bottom);
    m_bDropFar = (minimum < 0.2f);

    if (minimum > 0.3)
        return Docking::Fill;

    if (top == minimum && (!m_top || m_top->Hidden()))
        return Docking::Top;

    if (left == minimum && (!m_left || m_left->Hidden()))
        return Docking::Left;

    if (right == minimum && (!m_right || m_right->Hidden()))
        return Docking::Right;

    if (bottom == minimum && (!m_bottom || m_bottom->Hidden()))
        return Docking::Bottom;

    return Docking::Fill;
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
    Docking::Area dir = GetDroppedTabDirection(pos.x, pos.y);
    DockedTabControl* addTo = m_dockedTabControl;

    if (dir == Docking::Fill && addTo == nullptr)
        return false;

    if (dir != Docking::Fill)
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

void DockBase::OnTabRemoved(Gwk::Controls::Base* /*control*/)
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
    int dir = GetDroppedTabDirection(pos.x, pos.y);

    if (dir == Docking::Fill)
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

    if (dir == Docking::Left)
    {
        HelpBarWidth = m_hoverRect.w/4;
        m_hoverRect.w = HelpBarWidth;
    }

    if (dir == Docking::Right)
    {
        HelpBarWidth = m_hoverRect.w/4;
        m_hoverRect.x = m_hoverRect.w-HelpBarWidth;
        m_hoverRect.w = HelpBarWidth;
    }

    if (dir == Docking::Top)
    {
        HelpBarWidth = m_hoverRect.h/4;
        m_hoverRect.h = HelpBarWidth;
    }

    if (dir == Docking::Bottom)
    {
        HelpBarWidth = m_hoverRect.h/4;
        m_hoverRect.y = m_hoverRect.h-HelpBarWidth;
        m_hoverRect.h = HelpBarWidth;
    }

    if ((dir == Docking::Top || dir == Docking::Bottom) && !m_bDropFar)
    {
        if (m_left && m_left->Visible())
        {
            m_hoverRect.x += m_left->Width();
            m_hoverRect.w -= m_left->Width();
        }

        if (m_right && m_right->Visible())
            m_hoverRect.w -= m_right->Width();
    }

    if ((dir == Docking::Left || dir == Docking::Right) && !m_bDropFar)
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
