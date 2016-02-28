/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include "Gwork/Gwork.h"
#include "Gwork/Skin.h"
#include "Gwork/Controls/DockBase.h"
#include "Gwork/Controls/DockedTabControl.h"
#include "Gwork/Controls/Highlight.h"
#include "Gwork/DragAndDrop.h"
#include "Gwork/Controls/Resizer.h"

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(DockBase)
{
    SetPadding(Padding(1, 1, 1, 1));
    SetSize(200, 200);
    m_DockedTabControl = NULL;
    m_Left = NULL;
    m_Right = NULL;
    m_Top = NULL;
    m_Bottom = NULL;
    m_bDrawHover = false;
}

TabControl* DockBase::GetTabControl()
{
    return m_DockedTabControl;
}

void DockBase::SetupChildDock(Docking::Area pos)
{
    if (!m_DockedTabControl)
    {
        m_DockedTabControl = new DockedTabControl(this);
        m_DockedTabControl->onLoseTab.Add(this, &DockBase::OnTabRemoved);
        m_DockedTabControl->SetTabStripPosition(Docking::Bottom);
        m_DockedTabControl->SetShowTitlebar(true);
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
        return &m_Left;

    if (pos == Docking::Right)
        return &m_Right;

    if (pos == Docking::Top)
        return &m_Top;

    if (pos == Docking::Bottom)
        return &m_Bottom;

    return NULL;
}

DockBase* DockBase::GetChildDock(Docking::Area pos)
{
    DockBase** pDock = GetChildDockPtr(pos);

    if (!(*pDock))
    {
        (*pDock) = new DockBase(this);
        (*pDock)->SetupChildDock(pos);
    }
    else
    {
        (*pDock)->SetHidden(false);
    }

    return *pDock;
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

    if (top == minimum && (!m_Top || m_Top->Hidden()))
        return Docking::Top;

    if (left == minimum && (!m_Left || m_Left->Hidden()))
        return Docking::Left;

    if (right == minimum && (!m_Right || m_Right->Hidden()))
        return Docking::Right;

    if (bottom == minimum && (!m_Bottom || m_Bottom->Hidden()))
        return Docking::Bottom;

    return Docking::Fill;
}

bool DockBase::DragAndDrop_CanAcceptPackage(Gwk::DragAndDrop::Package* pPackage)
{
    // A TAB button dropped
    if (pPackage->name == "TabButtonMove")
        return true;

    // a TAB window dropped
    if (pPackage->name == "TabWindowMove")
        return true;

    return false;
}

bool DockBase::DragAndDrop_HandleDrop(Gwk::DragAndDrop::Package* pPackage, int x, int y)
{
    Gwk::Point pPos = CanvasPosToLocal(Gwk::Point(x, y));
    Docking::Area dir = GetDroppedTabDirection(pPos.x, pPos.y);
    DockedTabControl* pAddTo = m_DockedTabControl;

    if (dir == Docking::Fill && pAddTo == NULL)
        return false;

    if (dir != Docking::Fill)
    {
        DockBase* pDock = GetChildDock(dir);
        pAddTo = pDock->m_DockedTabControl;

        if (!m_bDropFar)
            pDock->BringToFront();
        else
            pDock->SendToBack();
    }

    if (pPackage->name == "TabButtonMove")
    {
        TabButton* pTabButton = gwk_cast<TabButton>(DragAndDrop::SourceControl);

        if (!pTabButton)
            return false;

        pAddTo->AddPage(pTabButton);
    }

    if (pPackage->name == "TabWindowMove")
    {
        DockedTabControl* pTabControl = gwk_cast<DockedTabControl>(DragAndDrop::SourceControl);

        if (!pTabControl)
            return false;

        if (pTabControl == pAddTo)
            return false;

        pTabControl->MoveTabsTo(pAddTo);
    }

    Invalidate();
    return true;
}

bool DockBase::IsEmpty()
{
    if (m_DockedTabControl && m_DockedTabControl->TabCount() > 0)
        return false;

    if (m_Left && !m_Left->IsEmpty())
        return false;

    if (m_Right && !m_Right->IsEmpty())
        return false;

    if (m_Top && !m_Top->IsEmpty())
        return false;

    if (m_Bottom && !m_Bottom->IsEmpty())
        return false;

    return true;
}

void DockBase::OnTabRemoved(Gwk::Controls::Base* /*pControl*/)
{
    DoRedundancyCheck();
    DoConsolidateCheck();
}

void DockBase::DoRedundancyCheck()
{
    if (!IsEmpty())
        return;

    DockBase* pDockParent = gwk_cast<DockBase>(GetParent());

    if (!pDockParent)
        return;

    pDockParent->OnRedundantChildDock(this);
}

void DockBase::DoConsolidateCheck()
{
    if (IsEmpty())
        return;

    if (!m_DockedTabControl)
        return;

    if (m_DockedTabControl->TabCount() > 0)
        return;

    if (m_Bottom && !m_Bottom->IsEmpty())
    {
        m_Bottom->m_DockedTabControl->MoveTabsTo(m_DockedTabControl);
        return;
    }

    if (m_Top && !m_Top->IsEmpty())
    {
        m_Top->m_DockedTabControl->MoveTabsTo(m_DockedTabControl);
        return;
    }

    if (m_Left && !m_Left->IsEmpty())
    {
        m_Left->m_DockedTabControl->MoveTabsTo(m_DockedTabControl);
        return;
    }

    if (m_Right && !m_Right->IsEmpty())
    {
        m_Right->m_DockedTabControl->MoveTabsTo(m_DockedTabControl);
        return;
    }
}

void DockBase::OnRedundantChildDock(DockBase* pDockBase)
{
    pDockBase->SetHidden(true);
    DoRedundancyCheck();
    DoConsolidateCheck();
}

void DockBase::DragAndDrop_HoverEnter(Gwk::DragAndDrop::Package* /*pPackage*/, int /*x*/,
                                      int /*y*/)
{
    m_bDrawHover = true;
}

void DockBase::DragAndDrop_HoverLeave(Gwk::DragAndDrop::Package* /*pPackage*/)
{
    m_bDrawHover = false;
}

void DockBase::DragAndDrop_Hover(Gwk::DragAndDrop::Package* /*pPackage*/, int x, int y)
{
    Gwk::Point pPos = CanvasPosToLocal(Gwk::Point(x, y));
    int dir = GetDroppedTabDirection(pPos.x, pPos.y);

    if (dir == Docking::Fill)
    {
        if (!m_DockedTabControl)
        {
            m_HoverRect = Gwk::Rect(0, 0, 0, 0);
            return;
        }

        m_HoverRect = GetInnerBounds();
        return;
    }

    m_HoverRect = GetRenderBounds();
    int HelpBarWidth = 0;

    if (dir == Docking::Left)
    {
        HelpBarWidth = m_HoverRect.w/4;
        m_HoverRect.w = HelpBarWidth;
    }

    if (dir == Docking::Right)
    {
        HelpBarWidth = m_HoverRect.w/4;
        m_HoverRect.x = m_HoverRect.w-HelpBarWidth;
        m_HoverRect.w = HelpBarWidth;
    }

    if (dir == Docking::Top)
    {
        HelpBarWidth = m_HoverRect.h/4;
        m_HoverRect.h = HelpBarWidth;
    }

    if (dir == Docking::Bottom)
    {
        HelpBarWidth = m_HoverRect.h/4;
        m_HoverRect.y = m_HoverRect.h-HelpBarWidth;
        m_HoverRect.h = HelpBarWidth;
    }

    if ((dir == Docking::Top || dir == Docking::Bottom) && !m_bDropFar)
    {
        if (m_Left && m_Left->Visible())
        {
            m_HoverRect.x += m_Left->Width();
            m_HoverRect.w -= m_Left->Width();
        }

        if (m_Right && m_Right->Visible())
            m_HoverRect.w -= m_Right->Width();
    }

    if ((dir == Docking::Left || dir == Docking::Right) && !m_bDropFar)
    {
        if (m_Top && m_Top->Visible())
        {
            m_HoverRect.y += m_Top->Height();
            m_HoverRect.h -= m_Top->Height();
        }

        if (m_Bottom && m_Bottom->Visible())
            m_HoverRect.h -= m_Bottom->Height();
    }
}

void DockBase::RenderOver(Skin::Base* skin)
{
    if (!m_bDrawHover)
        return;

    Gwk::Renderer::Base* render = skin->GetRender();
    render->SetDrawColor(Gwk::Color(255, 100, 255, 20));
    render->DrawFilledRect(GetRenderBounds());

    if (m_HoverRect.w == 0)
        return;

    render->SetDrawColor(Gwk::Color(255, 100, 255, 100));
    render->DrawFilledRect(m_HoverRect);
    render->SetDrawColor(Gwk::Color(255, 100, 255, 200));
    render->DrawLinedRect(m_HoverRect);
}
