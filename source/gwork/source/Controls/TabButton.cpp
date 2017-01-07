/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/TabButton.h>
#include <Gwork/Controls/TabControl.h>
#include <Gwork/Controls/Highlight.h>
#include <Gwork/DragAndDrop.h>

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(TabButton)
{
    m_page = nullptr;
    m_control = nullptr;
    DragAndDrop_SetPackage(true, "TabButtonMove");
    SetAlignment(Position::Top|Position::Left);
    SetTextPadding(Padding(2, 2, 2, 2));
}

void TabButton::Layout(Skin::Base* skin)
{
    Position parentDock = m_control->GetTabStrip()->GetDock();

    if (parentDock == Position::Bottom)
        SetPadding(Padding(3, 1, 5, 4));
    else if (parentDock == Position::Top)
        SetPadding(Padding(3, 3, 5, 2));
    else
        SetPadding(Padding(3, 2, 5, 2));

    ParentClass::Layout(skin);
}

void TabButton::Render(Skin::Base* skin)
{
    skin->DrawTabButton(this, IsActive(), m_control->GetTabStrip()->GetDock());
}

void TabButton::SetTabControl(TabControl* ctrl)
{
    if (m_control == ctrl)
        return;

    if (m_control)
        m_control->OnLoseTab(this);

    m_control = ctrl;
}

bool TabButton::DragAndDrop_ShouldStartDrag()
{
    return m_control->DoesAllowDrag();
}

bool TabButton::OnKeyUp(bool bDown)
{
    OnKeyLeft(bDown);
    return true;
}

bool TabButton::OnKeyDown(bool bDown)
{
    OnKeyRight(bDown);
    return true;
}

bool TabButton::OnKeyLeft(bool bDown)
{
    if (bDown)
    {
        Base::List::reverse_iterator it = std::find(
            m_parent->Children.rbegin(), m_parent->Children.rend(), this);

        if (it != m_parent->Children.rend() && (++it != m_parent->Children.rend()))
        {
            Base* nextTab = *it;
            GetTabControl()->OnTabPressed(nextTab);
            Gwk::KeyboardFocus = nextTab;
        }
    }

    return true;
}

bool TabButton::OnKeyRight(bool bDown)
{
    if (bDown)
    {
        Base::List::iterator it = std::find(m_parent->Children.begin(),
                                            m_parent->Children.end(), this);

        if (it != m_parent->Children.end() && (++it != m_parent->Children.end()))
        {
            Base* nextTab = *it;
            GetTabControl()->OnTabPressed(nextTab);
            Gwk::KeyboardFocus = nextTab;
        }
    }

    return true;
}

void TabButton::UpdateColors()
{
    if (!IsActive())
    {
        SetImageAlpha(0.5);

        if (IsDisabled())
            return SetTextColor(GetSkin()->Colors.Tab.Inactive.Disabled);

        if (IsDepressed())
            return SetTextColor(GetSkin()->Colors.Tab.Inactive.Down);

        if (IsHovered())
            return SetTextColor(GetSkin()->Colors.Tab.Inactive.Hover);

        return SetTextColor(GetSkin()->Colors.Tab.Inactive.Normal);
    }

    SetImageAlpha(1.0);

    if (IsDisabled())
        return SetTextColor(GetSkin()->Colors.Tab.Active.Disabled);

    if (IsDepressed())
        return SetTextColor(GetSkin()->Colors.Tab.Active.Down);

    if (IsHovered())
        return SetTextColor(GetSkin()->Colors.Tab.Active.Hover);

    SetTextColor(GetSkin()->Colors.Tab.Active.Normal);
}
