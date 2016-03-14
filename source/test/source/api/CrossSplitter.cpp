/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/CrossSplitter.h>
#include <Gwork/Controls/StatusBar.h>
#include <Gwork/Controls/Button.h>

using namespace Gwk;

class CrossSplitter : public TestUnit
{
public:

    GWK_CONTROL_INLINE(CrossSplitter, TestUnit)
    {
        Dock(Position::Fill);
        m_bSplittersVisible = false;
        m_curZoom = 0;
        m_splitter = new Gwk::Controls::CrossSplitter(this);
        m_splitter->SetPos(0, 0);
        m_splitter->Dock(Position::Fill);
        {
            Gwk::Controls::Button* testButton =  new Gwk::Controls::Button(m_splitter);
            testButton->SetText("TOPLEFT");
            m_splitter->SetPanel(0, testButton);
        }
        {
            Gwk::Controls::Button* testButton =  new Gwk::Controls::Button(m_splitter);
            testButton->SetText("TOPRIGHT");
            m_splitter->SetPanel(1, testButton);
        }
        {
            Gwk::Controls::Button* testButton =  new Gwk::Controls::Button(m_splitter);
            testButton->SetText("BOTTOMRIGHT");
            m_splitter->SetPanel(2, testButton);
        }
        {
            Gwk::Controls::Button* testButton =  new Gwk::Controls::Button(m_splitter);
            testButton->SetText("BOTTOMLEFT");
            m_splitter->SetPanel(3, testButton);
        }
        // Status bar to hold unit testing buttons
        Gwk::Controls::StatusBar* status = new Gwk::Controls::StatusBar(this);
        status->Dock(Position::Bottom);
        {
            Gwk::Controls::Button* button = new Gwk::Controls::Button(status);
            button->SetText("Zoom");
            button->onPress.Add(this, &CrossSplitter::ZoomTest);
            status->AddControl(button, false);
        }
        {
            Gwk::Controls::Button* button = new Gwk::Controls::Button(status);
            button->SetText("UnZoom");
            button->onPress.Add(this, &CrossSplitter::UnZoomTest);
            status->AddControl(button, false);
        }
        {
            Gwk::Controls::Button* button = new Gwk::Controls::Button(status);
            button->SetText("CenterPanels");
            button->onPress.Add(this, &CrossSplitter::CenterPanels);
            status->AddControl(button, true);
        }
    }

    void ZoomTest(Event::Info)
    {
        m_splitter->Zoom(m_curZoom);
        m_curZoom++;

        if (m_curZoom == 4)
            m_curZoom = 0;
    }

    void UnZoomTest(Event::Info)
    {
        m_splitter->UnZoom();
    }

    void CenterPanels(Event::Info)
    {
        m_splitter->CenterPanels();
        m_splitter->UnZoom();
    }

    void Layout(Gwk::Skin::Base* skin) override {}

    bool m_bSplittersVisible;
    int m_curZoom;
    Controls::CrossSplitter* m_splitter;

};


DECLARE_TEST(CrossSplitter);
