/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
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
