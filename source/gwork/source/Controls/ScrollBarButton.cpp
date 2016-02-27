/*
 *  GWEN
 *  Copyright (c) 2010 Facepunch Studios
 *  See license in Gwen.h
 */


#include "Gwen/Controls/ScrollBar.h"
#include "Gwen/Controls/ScrollBarButton.h"

using namespace Gwen;
using namespace Gwen::Controls;
using namespace Gwen::ControlsInternal;


GWEN_CONTROL_CONSTRUCTOR(ScrollBarButton)
{
    SetDirectionUp();
}

void ScrollBarButton::SetDirectionUp()
{
    m_iDirection = Docking::Top;
}

void ScrollBarButton::SetDirectionDown()
{
    m_iDirection = Docking::Bottom;
}

void ScrollBarButton::SetDirectionLeft()
{
    m_iDirection = Docking::Left;
}

void ScrollBarButton::SetDirectionRight()
{
    m_iDirection = Docking::Right;
}

void ScrollBarButton::Render(Skin::Base* skin)
{
    skin->DrawScrollButton(this, m_iDirection, m_bDepressed, IsHovered(), IsDisabled());
}
