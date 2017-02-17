/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/TreeControl.h>
#include <Gwork/Controls/ScrollControl.h>
#include <Gwork/Utility.h>
#include <Gwork/InputHandler.h>

using namespace Gwk;
using namespace Gwk::Controls;

GWK_CONTROL_CONSTRUCTOR(TreeControl)
{
    m_treeControl = this;
    m_toggleButton->DelayedDelete();
    m_toggleButton = nullptr;
    m_title->DelayedDelete();
    m_title = nullptr;
    m_innerPanel->DelayedDelete();
    m_innerPanel = nullptr;
    m_bAllowMultipleSelection = false;
    m_scrollControl = new ScrollControl(this);
    m_scrollControl->Dock(Position::Fill);
    m_scrollControl->SetScroll(false, true);
    m_scrollControl->SetAutoHideBars(true);
    m_scrollControl->SetMargin(Margin(1, 1, 1, 1));
    m_innerPanel = m_scrollControl;
    m_scrollControl->SetInnerSize(1000, 1000);
}

void TreeControl::Render(Skin::Base* skin)
{
    if (ShouldDrawBackground())
        skin->DrawTreeControl(this);
}

void TreeControl::OnChildBoundsChanged(Gwk::Rect /*oldChildBounds*/, Base* /*child*/)
{
    m_scrollControl->UpdateScrollBars();
    Invalidate();
}

void TreeControl::Clear()
{
    m_scrollControl->Clear();
}

void TreeControl::Layout(Skin::Base* skin)
{
    ParentClass::ParentClass::Layout(skin);
}

void TreeControl::PostLayout(Skin::Base* skin)
{
    ParentClass::ParentClass::PostLayout(skin);
}

void TreeControl::OnNodeAdded(TreeNode* node)
{
    node->onNamePress.Add(this, &TreeControl::OnNodeSelection);
}

void TreeControl::OnNodeSelection(Event::Info info)
{
    if (!m_bAllowMultipleSelection || !Gwk::Input::IsKeyDown(Key::Control))
        DeselectAll();
}
