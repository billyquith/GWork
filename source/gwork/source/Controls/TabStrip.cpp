/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/TabStrip.h>
#include <Gwork/Controls/TabControl.h>
#include <Gwork/Controls/Highlight.h>
#include <Gwork/DragAndDrop.h>
#include <Gwork/Utility.h>

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(TabStrip)
{
    m_tabDragControl = nullptr;
    m_bAllowReorder = false;
}

bool TabStrip::DragAndDrop_HandleDrop(Gwk::DragAndDrop::Package* /*package*/, int x, int y)
{
    Gwk::Point LocalPos = CanvasPosToLocal(Gwk::Point(x, y));
    TabButton* button = gwk_cast<TabButton>(DragAndDrop::SourceControl);
    TabControl* tabControl = gwk_cast<TabControl>(GetParent());

    if (tabControl && button)
    {
        if (button->GetTabControl() != tabControl)
        {
            // We've moved tab controls!
            tabControl->AddPage(button);
        }
    }

    Base* DroppedOn = GetControlAt(LocalPos.x, LocalPos.y);

    if (DroppedOn)
    {
        Gwk::Point DropPos = DroppedOn->CanvasPosToLocal(Gwk::Point(x, y));
        DragAndDrop::SourceControl->BringNextToControl(DroppedOn, DropPos.x > DroppedOn->Width()/2);
    }
    else
    {
        DragAndDrop::SourceControl->BringToFront();
    }

    return true;
}

bool TabStrip::DragAndDrop_CanAcceptPackage(Gwk::DragAndDrop::Package* package)
{
    if (!m_bAllowReorder)
        return false;

    if (package->name == "TabButtonMove")
        return true;

    return false;
}

void TabStrip::Layout(Skin::Base* skin)
{
    Gwk::Point largestTab(5, 5);
    int iNum = 0;

    for (auto&& control : Children)
    {
        TabButton* button = gwk_cast<TabButton>(control);

        if (!button)
            continue;

        button->SizeToContents();
        Margin m;
        int iNotFirst = iNum > 0 ? -1 : 0;

        {
            m.left = iNotFirst;
            button->Dock(Position::Left);
        }

        if (m_dock == Position::Left)
        {
            m.top = iNotFirst;
            button->Dock(Position::Top);
        }

        if (m_dock == Position::Right)
        {
            m.top = iNotFirst;
            button->Dock(Position::Top);
        }

        if (m_dock == Position::Bottom)
        {
            m.left = iNotFirst;
            button->Dock(Position::Left);
        }

        largestTab.x = std::max(largestTab.x, button->Width());
        largestTab.y = std::max(largestTab.y, button->Height());
        button->SetMargin(m);
        iNum++;
    }

    if (m_dock == Position::Top || m_dock == Position::Bottom)
        SetSize(Width(), largestTab.y);

    if (m_dock == Position::Left || m_dock == Position::Right)
        SetSize(largestTab.x, Height());

    ParentClass::Layout(skin);
}

void TabStrip::DragAndDrop_HoverEnter(Gwk::DragAndDrop::Package* /*package*/, int /*x*/,
                                      int /*y*/)
{
    if (m_tabDragControl)
        Debug::Msg("ERROR! TabStrip::DragAndDrop_HoverEnter\n");

    m_tabDragControl = new ControlsInternal::Highlight(this);
    m_tabDragControl->SetMouseInputEnabled(false);
    m_tabDragControl->SetSize(3, Height());
}

void TabStrip::DragAndDrop_HoverLeave(Gwk::DragAndDrop::Package* /*package*/)
{
    delete m_tabDragControl;
    m_tabDragControl = nullptr;
}

void TabStrip::DragAndDrop_Hover(Gwk::DragAndDrop::Package* /*package*/, int x, int y)
{
    Gwk::Point LocalPos = CanvasPosToLocal(Gwk::Point(x, y));
    Base* DroppedOn = GetControlAt(LocalPos.x, LocalPos.y);

    if (DroppedOn && DroppedOn != this)
    {
        Gwk::Point DropPos = DroppedOn->CanvasPosToLocal(Gwk::Point(x, y));
        m_tabDragControl->SetBounds(Gwk::Rect(0, 0, 3, Height()));
        m_tabDragControl->BringToFront();
        m_tabDragControl->SetPos(DroppedOn->X()-1, 0);

        if (DropPos.x > DroppedOn->Width()/2)
            m_tabDragControl->MoveBy(DroppedOn->Width()-1, 0);

        m_tabDragControl->Dock(Position::None);
    }
    else
    {
        m_tabDragControl->Dock(Position::Left);
        m_tabDragControl->BringToFront();
    }
}

void TabStrip::SetTabPosition(Position pos)
{
    Dock(pos);

    if (m_dock == Position::Top)
        SetPadding(Padding(5, 0, 0, 0));

    if (m_dock == Position::Left)
        SetPadding(Padding(0, 5, 0, 0));

    if (m_dock == Position::Right)
        SetPadding(Padding(0, 5, 0, 0));

    if (m_dock == Position::Bottom)
        SetPadding(Padding(5, 0, 0, 0));

    InvalidateChildren(true);
}
