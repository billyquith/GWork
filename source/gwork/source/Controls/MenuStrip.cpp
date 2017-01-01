/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Controls/MenuStrip.h>
#include <Gwork/Skin.h>

using namespace Gwk;
using namespace Gwk::Controls;

GWK_CONTROL_CONSTRUCTOR(MenuStrip)
{
    SetBounds(0, 0, 200, 22);
    Dock(Position::Top);
    m_innerPanel->SetPadding(Padding(5, 0, 0, 0));
}

void MenuStrip::Render(Skin::Base* skin)
{
    skin->DrawMenuStrip(this);
}

void MenuStrip::Layout(Skin::Base* /*skin*/)
{
    // TODO: We don't want to do vertical sizing the same as Menu, do nothing
    // for now
}

void MenuStrip::OnAddItem(MenuItem* item)
{
    item->Dock(Position::Left);
    item->SetTextPadding(Padding(5, 0, 5, 0));
    item->SetPadding(Padding(10, 0, 10, 0));
    item->SizeToContents();
    item->SetOnStrip(true);
    item->onHoverEnter.Add(this, &Menu::OnHoverItem);
}

bool MenuStrip::ShouldHoverOpenMenu()
{
    return IsMenuOpen();
}
