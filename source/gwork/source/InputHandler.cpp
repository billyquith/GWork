/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include "Gwork/InputHandler.h"
#include "Gwork/Controls/Base.h"
#include "Gwork/DragAndDrop.h"
#include "Gwork/Hook.h"
#include "Gwork/Platform.h"

using namespace Gwk;


static const float      DOUBLE_CLICK_SPEED  = 0.5f;
static const unsigned   MAX_MOUSE_BUTTONS   = 5;


struct Action
{
    unsigned char       type;

    int                 x,y;
    Gwk::UnicodeChar   chr;
};

static const float KeyRepeatRate = 0.03f;
static const float KeyRepeatDelay = 0.3f;

struct KeyData
{
    KeyData()
    {
        for (int i = 0; i < Gwk::Key::Count; i++)
        {
            KeyState[i] = false;
            NextRepeat[i] = 0;
        }

        Target = NULL;
        LeftMouseDown = false;
        RightMouseDown = false;
    }

    bool    KeyState[ Gwk::Key::Count ];
    float   NextRepeat[ Gwk::Key::Count ];
    Controls::Base* Target;
    bool    LeftMouseDown;
    bool    RightMouseDown;

};

static KeyData      g_KeyData;
static Gwk::Point  g_MousePosition;

static float        g_fLastClickTime[MAX_MOUSE_BUTTONS];
static Gwk::Point  g_pntLastClickPos;

enum
{
    ACT_MOUSEMOVE,
    ACT_MOUSEBUTTON,
    ACT_CHAR,
    ACT_MOUSEWHEEL,
    ACT_KEYPRESS,
    ACT_KEYRELEASE,
    ACT_MESSAGE
};


void UpdateHoveredControl(Controls::Base* pInCanvas)
{
    Controls::Base* pHovered = pInCanvas->GetControlAt(g_MousePosition.x, g_MousePosition.y);

    if (pHovered != Gwk::HoveredControl)
    {
        if (Gwk::HoveredControl)
        {
            Controls::Base* OldHover = Gwk::HoveredControl;
            Gwk::HoveredControl = NULL;
            OldHover->OnMouseLeave();
        }

        Gwk::HoveredControl = pHovered;

        if (Gwk::HoveredControl)
            Gwk::HoveredControl->OnMouseEnter();
    }

    if (Gwk::MouseFocus && Gwk::MouseFocus->GetCanvas() == pInCanvas)
    {
        if (Gwk::HoveredControl)
        {
            Controls::Base* OldHover = Gwk::HoveredControl;
            Gwk::HoveredControl = NULL;
            OldHover->Redraw();
        }

        Gwk::HoveredControl = Gwk::MouseFocus;
    }
}

bool FindKeyboardFocus(Controls::Base* pControl)
{
    if (!pControl)
        return false;

    if (pControl->GetKeyboardInputEnabled())
    {
        // Make sure none of our children have keyboard focus first
        //  todo recursive
        for (Controls::Base::List::iterator iter = pControl->Children.begin();
             iter != pControl->Children.end();
             ++iter)
        {
            Controls::Base* pChild = *iter;

            if (pChild == Gwk::KeyboardFocus)
                return false;
        }

        pControl->Focus();
        return true;
    }

    return FindKeyboardFocus(pControl->GetParent());
}

Gwk::Point Gwk::Input::GetMousePosition()
{
    return g_MousePosition;
}

void Gwk::Input::OnCanvasThink(Controls::Base* pControl)
{
    if (Gwk::MouseFocus && !Gwk::MouseFocus->Visible())
        Gwk::MouseFocus = NULL;

    if (Gwk::KeyboardFocus
        && (!Gwk::KeyboardFocus->Visible() || !KeyboardFocus->GetKeyboardInputEnabled()))
    {
        Gwk::KeyboardFocus = NULL;
    }

    if (!KeyboardFocus)
        return;

    if (KeyboardFocus->GetCanvas() != pControl)
        return;

    float fTime = Gwk::Platform::GetTimeInSeconds();

    //
    // Simulate Key-Repeats
    //
    for (int i = 0; i < Gwk::Key::Count; i++)
    {
        if (g_KeyData.KeyState[i] && g_KeyData.Target != KeyboardFocus)
        {
            g_KeyData.KeyState[i] = false;
            continue;
        }

        if (g_KeyData.KeyState[i] && fTime > g_KeyData.NextRepeat[i])
        {
            g_KeyData.NextRepeat[i] = Gwk::Platform::GetTimeInSeconds()+KeyRepeatRate;

            if (KeyboardFocus)
                KeyboardFocus->OnKeyPress(i);
        }
    }
}

bool Gwk::Input::IsKeyDown(int iKey)
{
    return g_KeyData.KeyState[ iKey ];
}

bool Gwk::Input::IsLeftMouseDown()
{
    return g_KeyData.LeftMouseDown;
}

bool Gwk::Input::IsRightMouseDown()
{
    return g_KeyData.RightMouseDown;
}

void Gwk::Input::OnMouseMoved(Controls::Base* pCanvas, int x, int y, int /*deltaX*/,
                               int /*deltaY*/)
{
    g_MousePosition.x = x;
    g_MousePosition.y = y;
    UpdateHoveredControl(pCanvas);
}

bool Gwk::Input::OnMouseClicked(Controls::Base* pCanvas, int iMouseButton, bool bDown)
{
    // If we click on a control that isn't a menu we want to close
    // all the open menus. Menus are children of the canvas.
    if (bDown && (!Gwk::HoveredControl || !Gwk::HoveredControl->IsMenuComponent()))
        pCanvas->CloseMenus();

    if (!Gwk::HoveredControl)
        return false;

    if (Gwk::HoveredControl->GetCanvas() != pCanvas)
        return false;

    if (!Gwk::HoveredControl->Visible())
        return false;

    if (Gwk::HoveredControl == pCanvas)
        return false;

    if (iMouseButton > MAX_MOUSE_BUTTONS)
        return false;

    if (iMouseButton == 0)
        g_KeyData.LeftMouseDown = bDown;
    else if (iMouseButton == 1)
        g_KeyData.RightMouseDown = bDown;

    // Double click.
    //! @todo Shouldn't double click if mouse has moved significantly
    bool bIsDoubleClick = false;

    if (bDown
        && g_pntLastClickPos.x == g_MousePosition.x
        && g_pntLastClickPos.y == g_MousePosition.y
        && (Gwk::Platform::GetTimeInSeconds()-g_fLastClickTime[iMouseButton]) < DOUBLE_CLICK_SPEED)
    {
        bIsDoubleClick = true;
    }

    if (bDown && !bIsDoubleClick)
    {
        g_fLastClickTime[ iMouseButton ] = Gwk::Platform::GetTimeInSeconds();
        g_pntLastClickPos = g_MousePosition;
    }

    if (bDown)
    {
        if (!FindKeyboardFocus(Gwk::HoveredControl))
        {
            if (Gwk::KeyboardFocus)
                Gwk::KeyboardFocus->Blur();
        }
    }

    Gwk::HoveredControl->UpdateCursor();

    // This tells the child it has been touched, which
    // in turn tells its parents, who tell their parents.
    // This is basically so that Windows can pop themselves
    // to the top when one of their children has been clicked.
    if (bDown)
        Gwk::HoveredControl->Touch();

#if GWK_HOOKSYSTEM

    if (bDown)
    {
        if (Hook::CallHook(&Hook::BaseHook::OnControlClicked,
                           Gwk::HoveredControl,
                           g_MousePosition.x,
                           g_MousePosition.y))
        return true;
    }

#endif

    switch (iMouseButton)
    {
    case 0:
        if (DragAndDrop::OnMouseButton(Gwk::HoveredControl,
                                       g_MousePosition.x, g_MousePosition.y,
                                       bDown))
        {
            return true;
        }

        if (bIsDoubleClick)
            Gwk::HoveredControl->OnMouseDoubleClickLeft(g_MousePosition.x, g_MousePosition.y);
        else
            Gwk::HoveredControl->OnMouseClickLeft(g_MousePosition.x, g_MousePosition.y, bDown);

        return true;

    case 1:
        if (bIsDoubleClick)
            Gwk::HoveredControl->OnMouseDoubleClickRight(g_MousePosition.x, g_MousePosition.y);
        else
            Gwk::HoveredControl->OnMouseClickRight(g_MousePosition.x, g_MousePosition.y, bDown);

        return true;
    }

    return false;
}

bool Gwk::Input::HandleAccelerator(Controls::Base* pCanvas, Gwk::UnicodeChar chr)
{
    // Build the accelerator search string
    Gwk::String accelString;

    if (Gwk::Input::IsControlDown())
        accelString += "CTRL+";

    if (Gwk::Input::IsShiftDown())
        accelString += "SHIFT+";

    chr = towupper(chr);
    accelString += chr;

    // Debug::Msg("Accelerator string :%S\n", accelString.c_str());

    if (Gwk::KeyboardFocus && Gwk::KeyboardFocus->HandleAccelerator(accelString))
        return true;

    if (Gwk::MouseFocus && Gwk::MouseFocus->HandleAccelerator(accelString))
        return true;

    if (pCanvas->HandleAccelerator(accelString))
        return true;

    return false;
}

bool Gwk::Input::DoSpecialKeys(Controls::Base* pCanvas, Gwk::UnicodeChar chr)
{
    if (!Gwk::KeyboardFocus)
        return false;

    if (Gwk::KeyboardFocus->GetCanvas() != pCanvas)
        return false;

    if (!Gwk::KeyboardFocus->Visible())
        return false;

    if (!Gwk::Input::IsControlDown())
        return false;

    if (chr == 'C' || chr == 'c')
    {
        Gwk::KeyboardFocus->OnCopy(NULL);
        return true;
    }

    if (chr == 'V' || chr == 'v')
    {
        Gwk::KeyboardFocus->OnPaste(NULL);
        return true;
    }

    if (chr == 'X' || chr == 'x')
    {
        Gwk::KeyboardFocus->OnCut(NULL);
        return true;
    }

    if (chr == 'A' || chr == 'a')
    {
        Gwk::KeyboardFocus->OnSelectAll(NULL);
        return true;
    }

    return false;
}

bool Gwk::Input::OnKeyEvent(Controls::Base* pCanvas, int iKey, bool bDown)
{
    Gwk::Controls::Base* pTarget = Gwk::KeyboardFocus;

    if (pTarget && pTarget->GetCanvas() != pCanvas)
        pTarget = NULL;

    if (pTarget && !pTarget->Visible())
        pTarget = NULL;

    if (bDown)
    {
        if (!g_KeyData.KeyState[iKey])
        {
            g_KeyData.KeyState[iKey] = true;
            g_KeyData.NextRepeat[iKey] = Gwk::Platform::GetTimeInSeconds()+KeyRepeatDelay;
            g_KeyData.Target = pTarget;

            if (pTarget)
                return pTarget->OnKeyPress(iKey);
        }
    }
    else
    {
        if (g_KeyData.KeyState[iKey])
        {
            g_KeyData.KeyState[iKey] = false;

            //! @bug This causes shift left arrow in textboxes
            //! to not work. What is disabling it here breaking?
            //! `g_KeyData.Target = NULL;`
            
            if (pTarget)
                return pTarget->OnKeyRelease(iKey);
        }
    }

    return false;
}
