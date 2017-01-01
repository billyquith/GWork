/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/ScrollBar.h>
#include <Gwork/Controls/ScrollBarButton.h>

using namespace Gwk;
using namespace Gwk::Controls;
using namespace Gwk::ControlsInternal;


GWK_CONTROL_CONSTRUCTOR(ScrollBarButton)
{
    SetDirectionUp();
}

void ScrollBarButton::SetDirectionUp()
{
    m_direction = Position::Top;
}

void ScrollBarButton::SetDirectionDown()
{
    m_direction = Position::Bottom;
}

void ScrollBarButton::SetDirectionLeft()
{
    m_direction = Position::Left;
}

void ScrollBarButton::SetDirectionRight()
{
    m_direction = Position::Right;
}

void ScrollBarButton::Render(Skin::Base* skin)
{
    skin->DrawScrollButton(this, m_direction, m_bDepressed, IsHovered(), IsDisabled());
}
