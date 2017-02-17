/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Controls/Menu.h>
#include <Gwork/Skin.h>
#include <Gwork/Utility.h>
#include <Gwork/Controls/Canvas.h>

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(Menu)
{
    SetBounds(0, 0, 10, 10);
    SetPadding(Padding(2, 2, 2, 2));
    SetDisableIconMargin(false);
    SetAutoHideBars(true);
    SetScroll(false, true);
    SetDeleteOnClose(false);
}


void Menu::Render(Skin::Base* skin)
{
    skin->DrawMenu(this, IconMarginDisabled());
}

void Menu::RenderUnder(Skin::Base* skin)
{
    ParentClass::RenderUnder(skin);
    skin->DrawShadow(this);
}

void Menu::Layout(Skin::Base* skin)
{
    int menuHeight = 0;

    for (auto&& child : m_innerPanel->Children)
    {
        if (child)
            menuHeight += child->Height();
    }

    // if tall menu descends off screen then clamp to screen height
    if (Y() + menuHeight > GetCanvas()->Height())
    {
        const int dy = GetCanvas()->Height() - Y();
        const int ytop = std::max(Y() - dy, 0);
        
        SetPos(X(), ytop);
        menuHeight = std::min(menuHeight, GetCanvas()->Height());
    }
    
    SetSize(Width(), menuHeight);

    ParentClass::Layout(skin);
}

MenuItem* Menu::AddItem(const String& strName,
                        const String& strIconName,
                        const String& strAccelerator)
{
    MenuItem* item = new MenuItem(this);
    item->SetPadding(Padding(2, 4, 4, 4));
    item->SetText(strName);
    item->SetImage(strIconName);
    item->SetAccelerator(strAccelerator);
    OnAddItem(item);
    return item;
}

void Menu::OnAddItem(MenuItem* item)
{
    item->SetTextPadding(Padding(IconMarginDisabled() ? 0 : 24, 0, 16, 0));
    item->Dock(Position::Top);
    item->SizeToContents();
    item->SetAlignment(Position::CenterV|Position::Left);
    item->onHoverEnter.Add(this, &Menu::OnHoverItem);
    
    // Do this here - after Top Docking these values mean nothing in layout
    const int w = std::max(item->Width()+10+32, Width());

    SetSize(w, Height());
}

void Menu::ClearItems()
{
    for (auto&& child : m_innerPanel->Children)
    {
        if (!child)
            continue;

        child->DelayedDelete();
    }
}

void Menu::CloseAll()
{
    for (auto&& child : m_innerPanel->Children)
    {
        MenuItem* item = gwk_cast<MenuItem>(child);

        if (!item)
            continue;

        item->CloseMenu();
    }
}

bool Menu::IsMenuOpen()
{
    for (auto&& child : m_innerPanel->Children)
    {
        MenuItem* item = gwk_cast<MenuItem>(child);

        if (!item)
            continue;

        if (item->IsMenuOpen())
            return true;
    }

    return false;
}

void Menu::OnHoverItem(Event::Info info)
{
    if (!ShouldHoverOpenMenu())
        return;

    MenuItem* item = gwk_cast<MenuItem>(info.ControlCaller);

    if (!item)
        return;

    if (item->IsMenuOpen())
        return;

    CloseAll();
    item->OpenMenu();
}

void Menu::Open(Position pos)
{
    SetHidden(false);
    BringToFront();
    Gwk::Point MousePos = Input::GetMousePosition();
    SetPos(MousePos.x, MousePos.y);
}

void Menu::Close()
{
    SetHidden(true);

    if (DeleteOnClose())
        DelayedDelete();
}

void Menu::CloseMenus()
{
    ParentClass::CloseMenus();
    CloseAll();
    Close();
}

void Menu::AddDivider()
{
    MenuDivider* divider = new MenuDivider(this);
    divider->Dock(Position::Top);
    divider->SetMargin(Margin(IconMarginDisabled() ? 0 : 24, 0, 4, 0));
}

void MenuDivider::Render(Gwk::Skin::Base* skin)
{
    skin->DrawMenuDivider(this);
}
