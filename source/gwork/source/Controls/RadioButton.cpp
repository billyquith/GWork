/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Controls/RadioButton.h>

using namespace Gwk;
using namespace Gwk::Controls;

RadioButton::RadioButton(Gwk::Controls::Base *parent, const Gwk::String &name):BaseClass(parent, name)
{
    SetSize(15, 15);
    SetMouseInputEnabled(true);
    SetTabable(false);
}

void RadioButton::Render(Skin::Base* skin)
{
    skin->DrawRadioButton(this, IsChecked(), IsDepressed());
}
