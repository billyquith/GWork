/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/CheckBox.h>

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(CheckBox)
{
    SetSize(15, 15);
    m_bChecked = true;
    Toggle();
}


void CheckBox::Render(Skin::Base* skin)
{
    skin->DrawCheckBox(this, m_bChecked, IsDepressed());
}

void CheckBox::OnPress(Event::Info)
{
    if (IsDisabled())
        return;

    if (IsChecked() && !AllowUncheck())
        return;

    Toggle();
}

void CheckBox::OnCheckStatusChanged()
{
    if (IsChecked())
        onChecked.Call(this);
    else
        onUnChecked.Call(this);

    onCheckChanged.Call(this);
}

void CheckBox::SetChecked(bool bChecked)
{
    if (m_bChecked == bChecked)
        return;

    m_bChecked = bChecked;
    OnCheckStatusChanged();
}
