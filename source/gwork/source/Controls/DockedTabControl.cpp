/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/DockedTabControl.h>
#include <Gwork/Controls/Highlight.h>
#include <Gwork/DragAndDrop.h>
#include <Gwork/Controls/WindowControl.h>

using namespace Gwk;
using namespace Gwk::Controls;

GWK_CONTROL_CONSTRUCTOR(DockedTabControl)
{
    m_windowControl = nullptr;
    Dock(Position::Fill);
    m_titleBar = new TabTitleBar(this);
    m_titleBar->Dock(Position::Top);
    m_titleBar->SetHidden(true);
    SetAllowReorder(true);
}

void DockedTabControl::Layout(Skin::Base* skin)
{
    GetTabStrip()->SetHidden(TabCount() <= 1);
    UpdateTitleBar();
    ParentClass::Layout(skin);
}

void DockedTabControl::UpdateTitleBar()
{
    if (!GetCurrentButton())
        return;

    m_titleBar->UpdateFromTab(GetCurrentButton());
}

void DockedTabControl::DragAndDrop_StartDragging(Gwk::DragAndDrop::Package* package, int x, int y)
{
    ParentClass::DragAndDrop_StartDragging(package, x, y);
    SetHidden(true);
    // This hiding our parent thing is kind of lousy.
    GetParent()->SetHidden(true);
}

void DockedTabControl::DragAndDrop_EndDragging(bool bSuccess, int /*x*/, int /*y*/)
{
    SetHidden(false);

    if (!bSuccess)
        GetParent()->SetHidden(false);

    /*
     *  if ( !bSuccess )
     *  {
     *      // Create our window control
     *      if ( !m_windowControl )
     *      {
     *          m_windowControl = new WindowControl( GetCanvas() );
     *          m_windowControl->SetBounds( x, y, Width(), Height() );
     *      }
     *
     *      m_windowControl->SetPosition( x, y );
     *      SetParent( m_windowControl );
     *      SetPosition( 0, 0 );
     *      Dock( Position::Fill );
     *  }
     */
}

void DockedTabControl::MoveTabsTo(DockedTabControl* target)
{
    Base::List Children = GetTabStrip()->Children;

    for (auto&& child : Children)
    {
        TabButton* button = gwk_cast<TabButton>(child);

        if (!button)
            continue;

        target->AddPage(button);
    }

    Invalidate();
}
