/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/TreeControl.h>

using namespace Gwk;

class Window : public GUnit
{
public:

    GWK_CONTROL_INLINE(Window, GUnit)
    {
        {
            Controls::Button* button = new Controls::Button(this);
            button->SetText("Normal Window");
            button->onPress.Add(this, &ThisClass::OpenWindow);
            button->SetPos(0, 0);
        }
        {
            Controls::Button* button = new Controls::Button(this);
            button->SetText("Modal Window");
            button->onPress.Add(this, &ThisClass::OpenModalWindow);
            button->SetPos(0, 32);
        }
        m_windowCount = 1;
    }

    void OpenWindow()
    {
        Controls::WindowControl* window = new Controls::WindowControl(GetCanvas());
        window->SetTitle(Utility::Format("Window %i", m_windowCount));
        window->SetSize(200+rand()%100, 200+rand()%100);
        window->SetPos(rand()%700, rand()%400);
        window->SetDeleteOnClose(true);
        m_windowCount++;
    }

    void OpenModalWindow()
    {
        Controls::WindowControl* window = new Controls::WindowControl(GetCanvas());
        window->SetTitle(Utility::Format("Window %i", m_windowCount));
        window->SetSize(200+rand()%100, 200+rand()%100);
        window->MakeModal(true);
        window->Position(Docking::Center);
        window->SetDeleteOnClose(true);
        m_windowCount++;
    }

    int m_windowCount;

};


DEFINE_UNIT_TEST(Window);
