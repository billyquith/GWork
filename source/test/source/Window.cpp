/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include "Gwork/Test/Test.h"
#include "Gwork/Controls/TreeControl.h"

using namespace Gwk;

class Window : public GUnit
{
public:

    GWK_CONTROL_INLINE(Window, GUnit)
    {
        {
            Controls::Button* pButton = new Controls::Button(this);
            pButton->SetText("Normal Window");
            pButton->onPress.Add(this, &ThisClass::OpenWindow);
            pButton->SetPos(0, 0);
        }
        {
            Controls::Button* pButton = new Controls::Button(this);
            pButton->SetText("Modal Window");
            pButton->onPress.Add(this, &ThisClass::OpenModalWindow);
            pButton->SetPos(0, 32);
        }
        m_iWindowCount = 1;
    }

    void OpenWindow()
    {
        Controls::WindowControl* pWindow = new Controls::WindowControl(GetCanvas());
        pWindow->SetTitle(Utility::Format("Window %i", m_iWindowCount));
        pWindow->SetSize(200+rand()%100, 200+rand()%100);
        pWindow->SetPos(rand()%700, rand()%400);
        pWindow->SetDeleteOnClose(true);
        m_iWindowCount++;
    }

    void OpenModalWindow()
    {
        Controls::WindowControl* pWindow = new Controls::WindowControl(GetCanvas());
        pWindow->SetTitle(Utility::Format("Window %i", m_iWindowCount));
        pWindow->SetSize(200+rand()%100, 200+rand()%100);
        pWindow->MakeModal(true);
        pWindow->Position(Docking::Center);
        pWindow->SetDeleteOnClose(true);
        m_iWindowCount++;
    }

    int m_iWindowCount;

};


DEFINE_UNIT_TEST(Window);
