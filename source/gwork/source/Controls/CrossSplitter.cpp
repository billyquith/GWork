/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Gwork.h>
#include <Gwork/Controls/CrossSplitter.h>
#include <Gwork/Controls/Button.h>

using namespace Gwk;
using namespace Controls;

GWK_CONTROL_CONSTRUCTOR(CrossSplitter)
{
    m_vSplitter = new SplitterBar(this);
    m_vSplitter->SetPos(0, 128);
    m_vSplitter->onDragged.Add(this, &CrossSplitter::OnVerticalMoved);
    m_vSplitter->SetCursor(Gwk::CursorType::SizeNS);
    m_hSplitter = new SplitterBar(this);
    m_hSplitter->SetPos(128, 0);
    m_hSplitter->onDragged.Add(this, &CrossSplitter::OnHorizontalMoved);
    m_hSplitter->SetCursor(Gwk::CursorType::SizeWE);
    m_cSplitter = new SplitterBar(this);
    m_cSplitter->SetPos(128, 128);
    m_cSplitter->onDragged.Add(this, &CrossSplitter::OnCenterMoved);
    m_cSplitter->SetCursor(Gwk::CursorType::SizeAll);
    m_fHVal = 0.5f;
    m_fVVal = 0.5f;
    SetPanel(0, nullptr);
    SetPanel(1, nullptr);
    SetPanel(2, nullptr);
    SetPanel(3, nullptr);
    SetSplitterSize(5);
    m_zoomedSection = -1;
}

void CrossSplitter::UpdateVSplitter()
{
    m_vSplitter->MoveTo(m_vSplitter->X(), (Height()-m_vSplitter->Height())*(m_fVVal));
}

void CrossSplitter::UpdateHSplitter()
{
    m_hSplitter->MoveTo((Width()-m_hSplitter->Width())*(m_fHVal), m_hSplitter->Y());
}

void CrossSplitter::OnCenterMoved(Event::Info)
{
    // Move the other two bars into position
    CalculateValueCenter();
    Invalidate();
}

void CrossSplitter::UpdateCSplitter()
{
    m_cSplitter->MoveTo((Width()-m_cSplitter->Width())*(m_fHVal),
                        (Height()-m_cSplitter->Height())*(m_fVVal));
}

void CrossSplitter::OnHorizontalMoved(Event::Info)
{
    m_fHVal = CalculateValueHorizontal();
    Invalidate();
}

void CrossSplitter::OnVerticalMoved(Event::Info)
{
    m_fVVal = CalculateValueVertical();
    Invalidate();
}

void CrossSplitter::CalculateValueCenter()
{
    m_fHVal = (float)m_cSplitter->X()/(float)(Width()-m_cSplitter->Width());
    m_fVVal = (float)m_cSplitter->Y()/(float)(Height()-m_cSplitter->Height());
}

float CrossSplitter::CalculateValueHorizontal()
{
    return (float)m_hSplitter->X()/(float)(Width()-m_hSplitter->Width());
}

float CrossSplitter::CalculateValueVertical()
{
    return (float)m_vSplitter->Y()/(float)(Height()-m_vSplitter->Height());
}

void CrossSplitter::Layout(Skin::Base* /*skin*/)
{
    m_vSplitter->SetSize(Width(), m_fBarSize);
    m_hSplitter->SetSize(m_fBarSize, Height());
    m_cSplitter->SetSize(m_fBarSize, m_fBarSize);
    UpdateVSplitter();
    UpdateHSplitter();
    UpdateCSplitter();

    if (m_zoomedSection == -1)
    {
        if (m_sections[0])
        {
            m_sections[0]->SetBounds(0,
                                     0,
                                     m_hSplitter->X(),
                                     m_vSplitter->Y());
        }

        if (m_sections[1])
        {
            m_sections[1]->SetBounds(m_hSplitter->X()+m_fBarSize,
                                     0,
                                     Width()-(m_hSplitter->X()+m_fBarSize),
                                     m_vSplitter->Y());
        }

        if (m_sections[2])
        {
            m_sections[2]->SetBounds(0,
                                     m_vSplitter->Y()+m_fBarSize,
                                     m_hSplitter->X(),
                                     Height()-(m_vSplitter->Y()+m_fBarSize));
        }

        if (m_sections[3])
        {
            m_sections[3]->SetBounds(m_hSplitter->X()+m_fBarSize,
                                     m_vSplitter->Y()+m_fBarSize,
                                     Width()-(m_hSplitter->X()+m_fBarSize),
                                     Height()-(m_vSplitter->Y()+m_fBarSize));
        }
    }
    else
    {
        // This should probably use Fill docking instead
        m_sections[m_zoomedSection]->SetBounds(0, 0, Width(), Height());
    }
}

void CrossSplitter::SetPanel(int index, Controls::Base* panel)
{
    Debug::AssertCheck(index >= 0 && index <= 3, "CrossSplitter::SetPanel out of range");
    m_sections[index] = panel;

    if (panel)
    {
        panel->Dock(Position::None);
        panel->SetParent(this);
    }

    Invalidate();
}

Controls::Base* CrossSplitter::GetPanel(int i)
{
    return m_sections[i];
}

void CrossSplitter::ZoomChanged()
{
    onZoomChange.Call(this);

    if (m_zoomedSection == -1)
        onUnZoomed.Call(this);
    else
        onZoomed.Call(this);
}

void CrossSplitter::Zoom(int section)
{
    UnZoom();

    if (m_sections[section])
    {
        for (int i = 0; i < 4; i++)
        {
            if (i != section && m_sections[i])
                m_sections[i]->SetHidden(true);
        }

        m_zoomedSection = section;
        Invalidate();
    }

    ZoomChanged();
}

void CrossSplitter::UnZoom()
{
    m_zoomedSection = -1;

    for (int i = 0; i < 4; i++)
    {
        if (m_sections[i])
            m_sections[i]->SetHidden(false);
    }

    Invalidate();
    ZoomChanged();
}
