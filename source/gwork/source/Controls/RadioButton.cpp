/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/RadioButton.h>

using namespace Gwk;
using namespace Gwk::Controls;

GWK_CONTROL_CONSTRUCTOR(RadioButton)
{
    SetSize(15, 15);
    SetMouseInputEnabled(true);
    SetTabable(false);
}

void RadioButton::Render(Skin::Base* skin)
{
    skin->DrawRadioButton(this, IsChecked(), IsDepressed());
}
