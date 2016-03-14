/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Controls/MenuItem.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/Canvas.h>

using namespace Gwk;
using namespace Gwk::Controls;

class GWK_EXPORT RightArrow : public Controls::Base
{
public:

    GWK_CONTROL_INLINE(RightArrow, Controls::Base)
    {
        SetMouseInputEnabled(false);
    }

    void Render(Skin::Base* skin) override
    {
        skin->DrawMenuRightArrow(this);
    }

};


GWK_CONTROL_CONSTRUCTOR(MenuItem)
{
    m_menu = nullptr;
    m_bOnStrip = false;
    m_submenuArrow = nullptr;
    m_accelerator = nullptr;
    SetTabable(false);
    SetCheckable(false);
    SetChecked(false);
}

MenuItem::~MenuItem()
{
}

void MenuItem::Render(Skin::Base* skin)
{
    skin->DrawMenuItem(this, IsMenuOpen(), m_bCheckable ? m_bChecked : false);

    // HACK!
    if (m_accelerator)
        m_accelerator->SetTextColorOverride(GetTextColor());
}

void MenuItem::Layout(Skin::Base* skin)
{
    if (m_submenuArrow)
        m_submenuArrow->SetPosition(Position::Right|Position::CenterV, 4, 0);

    ParentClass::Layout(skin);
}

Menu* MenuItem::GetMenu()
{
    if (!m_menu)
    {
        m_menu = new Menu(GetCanvas());
        m_menu->SetHidden(true);

        if (!m_bOnStrip)
        {
            m_submenuArrow = new RightArrow(this);
            m_submenuArrow->SetSize(15, 15);
        }

        Invalidate();
    }

    return m_menu;
}

void MenuItem::SetChecked(bool bCheck)
{
    if (bCheck == m_bChecked)
        return;

    m_bChecked = bCheck;
    onCheckChange.Call(this);

    if (bCheck)
        onChecked.Call(this);
    else
        onUnChecked.Call(this);
}

void MenuItem::OnPress(Event::Info info)
{
    if (m_menu)
    {
        ToggleMenu();
    }
    else if (!m_bOnStrip)
    {
        SetChecked(!GetChecked());
        onMenuItemSelected.Call(this);
        GetCanvas()->CloseMenus();
    }

    ParentClass::OnPress(info);
}

void MenuItem::ToggleMenu()
{
    if (IsMenuOpen())
        CloseMenu();
    else
        OpenMenu();
}

bool MenuItem::IsMenuOpen()
{
    if (!m_menu)
        return false;

    return !m_menu->Hidden();
}

void MenuItem::OpenMenu()
{
    if (!m_menu)
        return;

    m_menu->SetHidden(false);
    m_menu->BringToFront();
    Gwk::Point p = LocalPosToCanvas(Gwk::Point(0, 0));

    // Strip menus open downwards
    if (m_bOnStrip)
        m_menu->SetPos(p.x, p.y+Height()+1);
    // Submenus open sidewards
    else
        m_menu->SetPos(p.x+Width(), p.y);

    // TODO: Option this.
    // TODO: Make sure on screen, open the other side of the
    // parent if it's better...
}

void MenuItem::CloseMenu()
{
    if (!m_menu)
        return;

    m_menu->Close();
    m_menu->CloseAll();
}

void MenuItem::SetAccelerator(const String& strAccelerator)
{
    if (m_accelerator)
    {
        m_accelerator->DelayedDelete();
        m_accelerator = nullptr;
    }

    if (strAccelerator.empty())
        return;

    m_accelerator = new Controls::Label(this);
    m_accelerator->Dock(Position::Right);
    m_accelerator->SetAlignment(Position::Right|Position::CenterV);
    m_accelerator->SetText(strAccelerator);
    m_accelerator->SetMargin(Margin(0, 0, 16, 0));
    // TODO.
}

void MenuItem::SizeToContents()
{
    ParentClass::SizeToContents();

    if (m_accelerator)
    {
        m_accelerator->SizeToContents();
        SetWidth(Width()+m_accelerator->Width());
    }
}
