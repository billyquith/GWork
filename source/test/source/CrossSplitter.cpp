/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include "Gwork/Test/Test.h"
#include "Gwork/Controls/CrossSplitter.h"
#include "Gwork/Controls/StatusBar.h"
#include "Gwork/Controls/Button.h"

using namespace Gwk;

class CrossSplitter : public GUnit
{
public:

    GWK_CONTROL_INLINE(CrossSplitter, GUnit)
    {
        Dock(Docking::Fill);
        m_bSplittersVisible = false;
        m_iCurZoom = 0;
        m_Splitter = new Gwk::Controls::CrossSplitter(this);
        m_Splitter->SetPos(0, 0);
        m_Splitter->Dock(Docking::Fill);
        {
            Gwk::Controls::Button* testButton =  new Gwk::Controls::Button(m_Splitter);
            testButton->SetText("TOPLEFT");
            m_Splitter->SetPanel(0, testButton);
        }
        {
            Gwk::Controls::Button* testButton =  new Gwk::Controls::Button(m_Splitter);
            testButton->SetText("TOPRIGHT");
            m_Splitter->SetPanel(1, testButton);
        }
        {
            Gwk::Controls::Button* testButton =  new Gwk::Controls::Button(m_Splitter);
            testButton->SetText("BOTTOMRIGHT");
            m_Splitter->SetPanel(2, testButton);
        }
        {
            Gwk::Controls::Button* testButton =  new Gwk::Controls::Button(m_Splitter);
            testButton->SetText("BOTTOMLEFT");
            m_Splitter->SetPanel(3, testButton);
        }
        // Status bar to hold unit testing buttons
        Gwk::Controls::StatusBar* pStatus = new Gwk::Controls::StatusBar(this);
        pStatus->Dock(Docking::Bottom);
        {
            Gwk::Controls::Button* pButton = new Gwk::Controls::Button(pStatus);
            pButton->SetText("Zoom");
            pButton->onPress.Add(this, &CrossSplitter::ZoomTest);
            pStatus->AddControl(pButton, false);
        }
        {
            Gwk::Controls::Button* pButton = new Gwk::Controls::Button(pStatus);
            pButton->SetText("UnZoom");
            pButton->onPress.Add(this, &CrossSplitter::UnZoomTest);
            pStatus->AddControl(pButton, false);
        }
        {
            Gwk::Controls::Button* pButton = new Gwk::Controls::Button(pStatus);
            pButton->SetText("CenterPanels");
            pButton->onPress.Add(this, &CrossSplitter::CenterPanels);
            pStatus->AddControl(pButton, true);
        }
    }

    void ZoomTest(Gwk::Controls::Base* pFromPanel)
    {
        m_Splitter->Zoom(m_iCurZoom);
        m_iCurZoom++;

        if (m_iCurZoom == 4)
            m_iCurZoom = 0;
    }

    void UnZoomTest(Gwk::Controls::Base* pFromPanel)
    {
        m_Splitter->UnZoom();
    }

    void CenterPanels(Gwk::Controls::Base* pFromPanel)
    {
        m_Splitter->CenterPanels();
        m_Splitter->UnZoom();
    }

    void Layout(Gwk::Skin::Base* skin)
    {
    }

    bool m_bSplittersVisible;
    int m_iCurZoom;
    Controls::CrossSplitter* m_Splitter;

};


DEFINE_UNIT_TEST(CrossSplitter);
