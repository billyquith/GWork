/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/GroupBox.h>

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(GroupBox)
{
    // Set to true, because it's likely that our
    // children will want mouse input, and they
    // can't get it without us..
    SetMouseInputEnabled(true);
    SetTextPadding(Padding(10, 0, 0, 0));
    SetAlignment(Position::Top|Position::Left);
    Invalidate();
    m_innerPanel = new Base(this);
    m_innerPanel->Dock(Position::Fill);
    m_innerMargin = 6;
}

void GroupBox::Layout(Skin::Base* skin)
{
	m_innerPanel->SetMargin( Margin(m_innerMargin, TextHeight()/2 + m_innerMargin,
                                    m_innerMargin, m_innerMargin) );
    ParentClass::Layout(skin);
}

void GroupBox::Render(Skin::Base* skin)
{
    skin->DrawGroupBox(this, TextX(), TextHeight(), TextWidth());
}
