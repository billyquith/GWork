/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/LabelClickable.h>

using namespace Gwk;
using namespace Gwk::Controls;

GWK_CONTROL_CONSTRUCTOR(LabelClickable)
{
    SetIsToggle(false);
    SetCursor(Gwk::CursorType::Finger);
    SetAlignment(Gwk::Position::Left|Gwk::Position::CenterV);
}

void LabelClickable::Render(Skin::Base* /*skin*/)
{
    // skin->DrawButton( this, IsDepressed(), IsToggle() && GetToggleState() );
}


void LabelClickable::UpdateColors()
{
    if (IsDisabled())
        return SetTextColor(GetSkin()->Colors.Button.Disabled);

    if (IsDepressed() || GetToggleState())
        return SetTextColor(GetSkin()->Colors.Label.Bright);

    if (IsHovered())
        return SetTextColor(GetSkin()->Colors.Button.Hover);

    SetTextColor(GetSkin()->Colors.Button.Normal);
}
