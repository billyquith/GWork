/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/TreeControl.h>
#include <Gwork/Controls/WindowControl.h>
#include <Gwork/Controls/CheckBox.h>

using namespace Gwk;

class Window : public TestUnit
{
public:
    int m_windowCount;
    bool m_disableResizing = false;

    GWK_CONTROL_INLINE(Window, TestUnit)
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

        {
            Gwk::Controls::CheckBoxWithLabel* labeled = new Gwk::Controls::CheckBoxWithLabel(this);
            labeled->SetPos(0, 64);
            labeled->Label()->SetText("Disable Resizing");
            labeled->Checkbox()->onChecked.Add(this, &Window::DisableResizing);
            labeled->Checkbox()->onUnChecked.Add(this, &Window::EnableResizing);
        }
    }

    void DisableResizing(Controls::Base* control)
    {
        m_disableResizing = true;
    }

    void EnableResizing(Controls::Base* control)
    {
        m_disableResizing = false;
    }

    void OpenWindow()
    {
        Controls::WindowControl* window = new Controls::WindowControl(GetCanvas());
        window->SetTitle(Utility::Format("Window %i", m_windowCount));
        Window::AddExampleContents(window);
        window->SetSize(200+rand()%100, 200+rand()%100);
        window->SetPos(rand()%700, rand()%400);
        window->SetDeleteOnClose(true);
        m_windowCount++;
        if (m_disableResizing) {
            window->DisableResizing();
        }

    }

    void OpenModalWindow()
    {
        Controls::WindowControl* window = new Controls::WindowControl(GetCanvas());
        window->SetTitle(Utility::Format("Window %i", m_windowCount));
        Window::AddExampleContents(window);
        window->SetSize(200+rand()%100, 200+rand()%100);
        window->MakeModal(true);
        window->SetPosition(Position::Center);
        window->SetDeleteOnClose(true);
        m_windowCount++;
        if (m_disableResizing) {
            window->DisableResizing();
        }
    }

    static void AddExampleContents(Controls::WindowControl* window)
    {
        Gwk::Controls::Button* buttonA = new Gwk::Controls::Button(window);
        buttonA->SetText("Start Game");
        Gwk::Controls::Button* buttonB = new Gwk::Controls::Button(window);
        buttonB->SetText("Quit");
        Gwk::Align::PlaceBelow(buttonB, buttonA, 10);
    }

};


DECLARE_TEST(Window);
