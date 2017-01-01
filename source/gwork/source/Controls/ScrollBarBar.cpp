/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/ScrollBar.h>
#include <Gwork/Controls/ScrollBarBar.h>

using namespace Gwk;
using namespace Gwk::Controls;
using namespace Gwk::ControlsInternal;

// Actual bar representing height of parent

GWK_CONTROL_CONSTRUCTOR(ScrollBarBar)
{
    RestrictToParent(true);
    SetTarget(this);
}

void ScrollBarBar::Render(Skin::Base* skin)
{
    skin->DrawScrollBarBar(this, m_bDepressed, IsHovered(), m_bHorizontal);
    ParentClass::Render(skin);
}

void ScrollBarBar::OnMouseMoved(int x, int y, int deltaX, int deltaY)
{
    ParentClass::OnMouseMoved(x, y, deltaX, deltaY);

    if (!m_bDepressed)
        return;

    InvalidateParent();
}

void ScrollBarBar::OnMouseClickLeft(int x, int y, bool bDown)
{
    ParentClass::OnMouseClickLeft(x, y, bDown);
    InvalidateParent();
}

void ScrollBarBar::Layout(Skin::Base* /*skin*/)
{
    if (!GetParent())
        return;

    // Move to our current position to force clamping - is this a hack?
    MoveTo(X(), Y());
}

void ScrollBarBar::MoveTo(int x, int y)
{
    ParentClass::MoveTo(x, y);
}
