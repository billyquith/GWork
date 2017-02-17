/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/ImagePanel.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Controls/ResizableControl.h>

using namespace Gwk;
using namespace Gwk::Controls;
using namespace Gwk::ControlsInternal;

GWK_CONTROL_CONSTRUCTOR(ResizableControl)
{
    m_bResizable = true;
    m_minimumSize = Gwk::Point(5, 5);
    m_bClampMovement = false;
    
    // bottom
    m_resizers[0] = new Resizer(this);
    m_resizers[0]->Dock(Position::Bottom);
    m_resizers[0]->SetResizeDir(Position::Bottom);
    m_resizers[0]->SetTarget(this);
    m_resizers[0]->onResize.Add(this, &ResizableControl::OnResizedInternal);
    
    // bottom left
    m_resizers[1] = new Resizer(m_resizers[0]);
    m_resizers[1]->Dock(Position::Left);
    m_resizers[1]->SetResizeDir(Position::Bottom|Position::Left);
    m_resizers[1]->SetTarget(this);
    m_resizers[1]->onResize.Add(this, &ResizableControl::OnResizedInternal);
    
    // bottom right
    m_resizers[2] = new Resizer(m_resizers[0]);
    m_resizers[2]->Dock(Position::Right);
    m_resizers[2]->SetResizeDir(Position::Bottom|Position::Right);
    m_resizers[2]->SetTarget(this);
    m_resizers[2]->onResize.Add(this, &ResizableControl::OnResizedInternal);
    
    // top
    m_resizers[3] = new Resizer(this);
    m_resizers[3]->Dock(Position::Top);
    m_resizers[3]->SetResizeDir(Position::Top);
    m_resizers[3]->SetTarget(this);
    m_resizers[3]->onResize.Add(this, &ResizableControl::OnResizedInternal);
    
    // top left
    m_resizers[4] = new Resizer(m_resizers[3]);
    m_resizers[4]->Dock(Position::Left);
    m_resizers[4]->SetResizeDir(Position::Top|Position::Left);
    m_resizers[4]->SetTarget(this);
    m_resizers[4]->onResize.Add(this, &ResizableControl::OnResizedInternal);
    
    // top right
    m_resizers[5] = new Resizer(m_resizers[3]);
    m_resizers[5]->Dock(Position::Right);
    m_resizers[5]->SetResizeDir(Position::Top|Position::Right);
    m_resizers[5]->SetTarget(this);
    m_resizers[5]->onResize.Add(this, &ResizableControl::OnResizedInternal);
    
    // left
    m_resizers[6] = new Resizer(this);
    m_resizers[6]->Dock(Position::Left);
    m_resizers[6]->SetResizeDir(Position::Left);
    m_resizers[6]->SetTarget(this);
    m_resizers[6]->onResize.Add(this, &ResizableControl::OnResizedInternal);
    
    // right
    m_resizers[7] = new Resizer(this);
    m_resizers[7]->Dock(Position::Right);
    m_resizers[7]->SetResizeDir(Position::Right);
    m_resizers[7]->SetTarget(this);
    m_resizers[7]->onResize.Add(this, &ResizableControl::OnResizedInternal);
}

void ResizableControl::DisableResizing()
{
    for (auto&& resizer : m_resizers)
    {
        resizer->SetDisabled(true);
    }
}

bool ResizableControl::SetBounds(int x, int y, int w, int h)
{
    Gwk::Point minSize = GetMinimumSize();

    // Clamp Minimum Size
    w = std::max(w, minSize.x);
    h = std::max(h, minSize.y);

    // Clamp to parent's window
    Base* parent = GetParent();

    if (parent && m_bClampMovement)
    {
        if (x+w > parent->Width())
            x = parent->Width()-w;

        if (x < 0)
            x = 0;

        if (y+h > parent->Height())
            y = parent->Height()-h;

        if (y < 0)
            y = 0;
    }

    return ParentClass::SetBounds(x, y, w, h);
}

void ResizableControl::OnResizedInternal(Event::Info)
{
    if (!IsDisabled())
    {
        onResize.Call(this);
        OnResized();
    }
}
