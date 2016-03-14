/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
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
    int childrenHeight = 0;

    for (Base::List::iterator it = m_innerPanel->Children.begin();
         it != m_innerPanel->Children.end();
         ++it)
    {
        Base* child = (*it);
        
        if (!child)
            continue;
        
        childrenHeight += child->Height();
    }

    if (Y()+childrenHeight > GetCanvas()->Height())
    {
        childrenHeight = GetCanvas()->Height() - Y();
    }

    SetSize(Width(), childrenHeight);
    
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
    for (Base::List::iterator it = m_innerPanel->Children.begin();
         it != m_innerPanel->Children.end();
         ++it)
    {
        Base* child = *it;

        if (!child)
            continue;

        child->DelayedDelete();
    }
}

void Menu::CloseAll()
{
    for (Base::List::iterator it = m_innerPanel->Children.begin();
         it != m_innerPanel->Children.end();
         ++it)
    {
        Base* child = *it;
        MenuItem* item = gwk_cast<MenuItem>(child);

        if (!item)
            continue;

        item->CloseMenu();
    }
}

bool Menu::IsMenuOpen()
{
    for (Base::List::iterator it = m_innerPanel->Children.begin();
         it != m_innerPanel->Children.end();
         ++it)
    {
        Base* child = *it;
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
