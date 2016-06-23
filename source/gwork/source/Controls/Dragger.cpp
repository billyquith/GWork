/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Controls/Dragger.h>

using namespace Gwk;
using namespace Gwk::ControlsInternal;


GWK_CONTROL_CONSTRUCTOR(Dragger)
{
    m_target = nullptr;
    SetMouseInputEnabled(true);
    m_bDepressed = false;
    m_bDoMove = true;
}

void Dragger::OnMouseClickLeft(int x, int y, bool bDown)
{
    if (bDown)
    {
        m_bDepressed = true;

        if (m_target)
            m_holdPos = m_target->CanvasPosToLocal(Gwk::Point(x, y));

        Gwk::MouseFocus = this;
        onDragStart.Call(this);
    }
    else
    {
        m_bDepressed = false;
        Gwk::MouseFocus = nullptr;
    }
}

void Dragger::OnMouseMoved(int x, int y, int deltaX, int deltaY)
{
    if (!m_bDepressed)
        return;

    if (m_bDoMove && m_target)
    {
        Gwk::Point p = Gwk::Point(x-m_holdPos.x, y-m_holdPos.y);

        // Translate to parent
        if (m_target->GetParent())
            p = m_target->GetParent()->CanvasPosToLocal(p);

        m_target->MoveTo(p.x, p.y);
    }

    Gwk::Event::Info info(this);
    info.Point = Gwk::Point(deltaX, deltaY);
    onDragged.Call(this, info);
}

void Dragger::Render(Skin::Base* skin)
{
    // skin->DrawButton(this,false,false, false);
}

void Dragger::OnMouseDoubleClickLeft(int x, int y)
{
    onDoubleClickLeft.Call(this);
}
