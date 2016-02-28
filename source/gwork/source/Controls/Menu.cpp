/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include "Gwork/Gwork.h"
#include "Gwork/Controls/Menu.h"
#include "Gwork/Skin.h"
#include "Gwork/Utility.h"

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

    for (Base::List::iterator it = m_InnerPanel->Children.begin();
         it != m_InnerPanel->Children.end();
         ++it)
    {
        Base* pChild = (*it);
        
        if (!pChild)
            continue;
        
        childrenHeight += pChild->Height();
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
    MenuItem* pItem = new MenuItem(this);
    pItem->SetPadding(Padding(2, 4, 4, 4));
    pItem->SetText(strName);
    pItem->SetImage(strIconName);
    pItem->SetAccelerator(strAccelerator);
    OnAddItem(pItem);
    return pItem;
}

void Menu::OnAddItem(MenuItem* item)
{
    item->SetTextPadding(Padding(IconMarginDisabled() ? 0 : 24, 0, 16, 0));
    item->Dock(Docking::Top);
    item->SizeToContents();
    item->SetAlignment(Docking::CenterV|Docking::Left);
    item->onHoverEnter.Add(this, &Menu::OnHoverItem);
    
    // Do this here - after Top Docking these values mean nothing in layout
    const int w = Max(item->Width()+10+32, Width());

    SetSize(w, Height());
}

void Menu::ClearItems()
{
    for (Base::List::iterator it = m_InnerPanel->Children.begin();
         it != m_InnerPanel->Children.end();
         ++it)
    {
        Base* pChild = *it;

        if (!pChild)
            continue;

        pChild->DelayedDelete();
    }
}

void Menu::CloseAll()
{
    for (Base::List::iterator it = m_InnerPanel->Children.begin();
         it != m_InnerPanel->Children.end();
         ++it)
    {
        Base* pChild = *it;
        MenuItem* pItem = gwk_cast<MenuItem>(pChild);

        if (!pItem)
            continue;

        pItem->CloseMenu();
    }
}

bool Menu::IsMenuOpen()
{
    for (Base::List::iterator it = m_InnerPanel->Children.begin();
         it != m_InnerPanel->Children.end();
         ++it)
    {
        Base* pChild = *it;
        MenuItem* pItem = gwk_cast<MenuItem>(pChild);

        if (!pItem)
            continue;

        if (pItem->IsMenuOpen())
            return true;
    }

    return false;
}

void Menu::OnHoverItem(Gwk::Controls::Base* pControl)
{
    if (!ShouldHoverOpenMenu())
        return;

    MenuItem* pItem = gwk_cast<MenuItem>(pControl);

    if (!pItem)
        return;

    if (pItem->IsMenuOpen())
        return;

    CloseAll();
    pItem->OpenMenu();
}

void Menu::Open(unsigned int iPos)
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
    divider->Dock(Docking::Top);
    divider->SetMargin(Margin(IconMarginDisabled() ? 0 : 24, 0, 4, 0));
}

void MenuDivider::Render(Gwk::Skin::Base* skin)
{
    skin->DrawMenuDivider(this);
}
