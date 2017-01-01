/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
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
