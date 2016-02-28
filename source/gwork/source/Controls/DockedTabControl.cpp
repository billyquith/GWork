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
    m_WindowControl = NULL;
    Dock(Docking::Fill);
    m_pTitleBar = new TabTitleBar(this);
    m_pTitleBar->Dock(Docking::Top);
    m_pTitleBar->SetHidden(true);
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

    m_pTitleBar->UpdateFromTab(GetCurrentButton());
}

void DockedTabControl::DragAndDrop_StartDragging(Gwk::DragAndDrop::Package* pPackage, int x, int y)
{
    ParentClass::DragAndDrop_StartDragging(pPackage, x, y);
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
     *      if ( !m_WindowControl )
     *      {
     *          m_WindowControl = new WindowControl( GetCanvas() );
     *          m_WindowControl->SetBounds( x, y, Width(), Height() );
     *      }
     *
     *      m_WindowControl->SetPosition( x, y );
     *      SetParent( m_WindowControl );
     *      SetPosition( 0, 0 );
     *      Dock( Docking::Fill );
     *  }
     */
}

void DockedTabControl::MoveTabsTo(DockedTabControl* pTarget)
{
    Base::List Children = GetTabStrip()->Children;

    for (Base::List::iterator iter = Children.begin(); iter != Children.end(); ++iter)
    {
        TabButton* pButton = gwk_cast<TabButton>(*iter);

        if (!pButton)
            continue;

        pTarget->AddPage(pButton);
    }

    Invalidate();
}
