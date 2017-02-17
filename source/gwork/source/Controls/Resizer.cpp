/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/Resizer.h>

using namespace Gwk;
using namespace Gwk::ControlsInternal;


GWK_CONTROL_CONSTRUCTOR(Resizer)
{
    m_resizeDir = Position::Left;
    SetMouseInputEnabled(true);
    SetSize(6, 6);
}

void Resizer::OnMouseMoved(int x, int y, int /*deltaX*/, int /*deltaY*/)
{
    if (m_bDisabled)
        return;
    
    if (!m_target)
        return;

    if (!m_bDepressed)
        return;

    Gwk::Rect bounds = m_target->GetBounds();
    Gwk::Point pntMin = m_target->GetMinimumSize();
    Gwk::Point cursorPos = m_target->CanvasPosToLocal(Gwk::Point(x, y));
    Gwk::Point delta = m_target->LocalPosToCanvas(m_holdPos);
    delta.x -= x;
    delta.y -= y;

    if (m_resizeDir & Position::Left)
    {
        bounds.x -= delta.x;
        bounds.w += delta.x;

        // Conform to minimum size here so we don't
        // go all weird when we snap it in the base conrt
        if (bounds.w < pntMin.x)
        {
            const int diff = pntMin.x-bounds.w;
            bounds.w += diff;
            bounds.x -= diff;
        }
    }

    if (m_resizeDir & Position::Top)
    {
        bounds.y -= delta.y;
        bounds.h += delta.y;

        // Conform to minimum size here so we don't
        // go all weird when we snap it in the base conrt
        if (bounds.h < pntMin.y)
        {
            const int diff = pntMin.y-bounds.h;
            bounds.h += diff;
            bounds.y -= diff;
        }
    }

    if (m_resizeDir & Position::Right)
    {
        // This is complicated.
        // Basically we want to use the HoldPos, so it doesn't snap to the edge
        // of the control
        // But we need to move the HoldPos with the window movement. Yikes.
        // I actually think this might be a big hack around the way this control
        // works with regards to the holdpos being on the parent panel.
        const int woff = bounds.w - m_holdPos.x;
        int diff = bounds.w;
        bounds.w = cursorPos.x + woff;

        if (bounds.w < pntMin.x)
            bounds.w = pntMin.x;

        diff -= bounds.w;
        m_holdPos.x -= diff;
    }

    if (m_resizeDir & Position::Bottom)
    {
        const int hoff = bounds.h - m_holdPos.y;
        int diff = bounds.h;
        bounds.h = cursorPos.y + hoff;

        if (bounds.h < pntMin.y)
            bounds.h = pntMin.y;

        diff -= bounds.h;
        m_holdPos.y -= diff;
    }

    m_target->SetBounds(bounds);
    onResize.Call(this);
}

void Resizer::SetResizeDir(Position dir)
{
    m_resizeDir = dir;

    if (((dir & Position::Left) && (dir & Position::Top))
        || ((dir & Position::Right) && (dir & Position::Bottom)))
        return SetCursor(Gwk::CursorType::SizeNWSE);

    if (((dir & Position::Right) && (dir & Position::Top))
        || ((dir & Position::Left) && (dir & Position::Bottom)))
        return SetCursor(Gwk::CursorType::SizeNESW);

    if ((dir & Position::Right) || (dir & Position::Left))
        return SetCursor(Gwk::CursorType::SizeWE);

    if ((dir & Position::Top) || (dir & Position::Bottom))
        return SetCursor(Gwk::CursorType::SizeNS);
}
