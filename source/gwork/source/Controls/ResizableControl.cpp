/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include "Gwork/Controls/ImagePanel.h"
#include "Gwork/Controls/Label.h"
#include "Gwork/Controls/ResizableControl.h"

using namespace Gwk;
using namespace Gwk::Controls;
using namespace Gwk::ControlsInternal;

GWK_CONTROL_CONSTRUCTOR(ResizableControl)
{
    m_bResizable = true;
    m_MinimumSize = Gwk::Point(5, 5);
    m_bClampMovement = false;
    m_Resizer[5] = NULL;
    m_Resizer[0] = NULL;
    m_Resizer[2] = new Resizer(this);
    m_Resizer[2]->Dock(Docking::Bottom);
    m_Resizer[2]->SetResizeDir(Docking::Bottom);
    m_Resizer[2]->SetTarget(this);
    m_Resizer[2]->onResize.Add(this, &ResizableControl::OnResizedInternal);
    m_Resizer[1] = new Resizer(m_Resizer[2]);
    m_Resizer[1]->Dock(Docking::Left);
    m_Resizer[1]->SetResizeDir(Docking::Bottom|Docking::Left);
    m_Resizer[1]->SetTarget(this);
    m_Resizer[1]->onResize.Add(this, &ResizableControl::OnResizedInternal);
    m_Resizer[3] = new Resizer(m_Resizer[2]);
    m_Resizer[3]->Dock(Docking::Right);
    m_Resizer[3]->SetResizeDir(Docking::Bottom|Docking::Right);
    m_Resizer[3]->SetTarget(this);
    m_Resizer[3]->onResize.Add(this, &ResizableControl::OnResizedInternal);
    m_Resizer[8] = new Resizer(this);
    m_Resizer[8]->Dock(Docking::Top);
    m_Resizer[8]->SetResizeDir(Docking::Top);
    m_Resizer[8]->SetTarget(this);
    m_Resizer[8]->onResize.Add(this, &ResizableControl::OnResizedInternal);
    m_Resizer[7] = new Resizer(m_Resizer[8]);
    m_Resizer[7]->Dock(Docking::Left);
    m_Resizer[7]->SetResizeDir(Docking::Top|Docking::Left);
    m_Resizer[7]->SetTarget(this);
    m_Resizer[7]->onResize.Add(this, &ResizableControl::OnResizedInternal);
    m_Resizer[9] = new Resizer(m_Resizer[8]);
    m_Resizer[9]->Dock(Docking::Right);
    m_Resizer[9]->SetResizeDir(Docking::Top|Docking::Right);
    m_Resizer[9]->SetTarget(this);
    m_Resizer[9]->onResize.Add(this, &ResizableControl::OnResizedInternal);
    m_Resizer[4] = new Resizer(this);
    m_Resizer[4]->Dock(Docking::Left);
    m_Resizer[4]->SetResizeDir(Docking::Left);
    m_Resizer[4]->SetTarget(this);
    m_Resizer[4]->onResize.Add(this, &ResizableControl::OnResizedInternal);
    m_Resizer[6] = new Resizer(this);
    m_Resizer[6]->Dock(Docking::Right);
    m_Resizer[6]->SetResizeDir(Docking::Right);
    m_Resizer[6]->SetTarget(this);
    m_Resizer[6]->onResize.Add(this, &ResizableControl::OnResizedInternal);
}

void ResizableControl::DisableResizing()
{
    for (Base::List::iterator it = Children.begin(); it != Children.end(); ++it)
    {
        Resizer* resizer = gwk_cast<Resizer>(*it);

        if (!resizer)
            continue;

        resizer->SetMouseInputEnabled(false);
        resizer->SetHidden(false);
        SetPadding(Padding(resizer->Width(), resizer->Width(), resizer->Width(),
                           resizer->Width()));
    }
}

bool ResizableControl::SetBounds(int x, int y, int w, int h)
{
    Gwk::Point minSize = GetMinimumSize();

    // Clamp Minimum Size
    w = Max(w, minSize.x);
    h = Max(h, minSize.y);

    // Clamp to parent's window
    Base* pParent = GetParent();

    if (pParent && m_bClampMovement)
    {
        if (x+w > pParent->Width())
            x = pParent->Width()-w;

        if (x < 0)
            x = 0;

        if (y+h > pParent->Height())
            y = pParent->Height()-h;

        if (y < 0)
            y = 0;
    }

    return ParentClass::SetBounds(x, y, w, h);
}

void ResizableControl::OnResizedInternal(Controls::Base* /*pControl*/)
{
    onResize.Call(this);
    OnResized();
}
