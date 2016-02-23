/*
 *  GWEN
 *  Copyright (c) 2010 Facepunch Studios
 *  See license in Gwen.h
 */


#include "Gwen/InputHandler.h"
#include "Gwen/Controls/Base.h"
#include "Gwen/DragAndDrop.h"
#include "Gwen/Hook.h"
#include "Gwen/Platform.h"

using namespace Gwen;


static const float      DOUBLE_CLICK_SPEED  = 0.5f;
static const unsigned   MAX_MOUSE_BUTTONS   = 5;


struct Action
{
    unsigned char       type;

    int                 x,y;
    Gwen::UnicodeChar   chr;
};

static const float KeyRepeatRate = 0.03f;
static const float KeyRepeatDelay = 0.3f;

struct KeyData
{
    KeyData()
    {
        for (int i = 0; i < Gwen::Key::Count; i++)
        {
            KeyState[i] = false;
            NextRepeat[i] = 0;
        }

        Target = NULL;
        LeftMouseDown = false;
        RightMouseDown = false;
    }

    bool    KeyState[ Gwen::Key::Count ];
    float   NextRepeat[ Gwen::Key::Count ];
    Controls::Base* Target;
    bool    LeftMouseDown;
    bool    RightMouseDown;

};

static KeyData      g_KeyData;
static Gwen::Point  g_MousePosition;

static float        g_fLastClickTime[MAX_MOUSE_BUTTONS];
static Gwen::Point  g_pntLastClickPos;

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

    if (pHovered != Gwen::HoveredControl)
    {
        if (Gwen::HoveredControl)
        {
            Controls::Base* OldHover = Gwen::HoveredControl;
            Gwen::HoveredControl = NULL;
            OldHover->OnMouseLeave();
        }

        Gwen::HoveredControl = pHovered;

        if (Gwen::HoveredControl)
            Gwen::HoveredControl->OnMouseEnter();
    }

    if (Gwen::MouseFocus && Gwen::MouseFocus->GetCanvas() == pInCanvas)
    {
        if (Gwen::HoveredControl)
        {
            Controls::Base* OldHover = Gwen::HoveredControl;
            Gwen::HoveredControl = NULL;
            OldHover->Redraw();
        }

        Gwen::HoveredControl = Gwen::MouseFocus;
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

            if (pChild == Gwen::KeyboardFocus)
                return false;
        }

        pControl->Focus();
        return true;
    }

    return FindKeyboardFocus(pControl->GetParent());
}

Gwen::Point Gwen::Input::GetMousePosition()
{
    return g_MousePosition;
}

void Gwen::Input::OnCanvasThink(Controls::Base* pControl)
{
    if (Gwen::MouseFocus && !Gwen::MouseFocus->Visible())
        Gwen::MouseFocus = NULL;

    if (Gwen::KeyboardFocus
        && (!Gwen::KeyboardFocus->Visible() || !KeyboardFocus->GetKeyboardInputEnabled()))
    {
        Gwen::KeyboardFocus = NULL;
    }

    if (!KeyboardFocus)
        return;

    if (KeyboardFocus->GetCanvas() != pControl)
        return;

    float fTime = Gwen::Platform::GetTimeInSeconds();

    //
    // Simulate Key-Repeats
    //
    for (int i = 0; i < Gwen::Key::Count; i++)
    {
        if (g_KeyData.KeyState[i] && g_KeyData.Target != KeyboardFocus)
        {
            g_KeyData.KeyState[i] = false;
            continue;
        }

        if (g_KeyData.KeyState[i] && fTime > g_KeyData.NextRepeat[i])
        {
            g_KeyData.NextRepeat[i] = Gwen::Platform::GetTimeInSeconds()+KeyRepeatRate;

            if (KeyboardFocus)
                KeyboardFocus->OnKeyPress(i);
        }
    }
}

bool Gwen::Input::IsKeyDown(int iKey)
{
    return g_KeyData.KeyState[ iKey ];
}

bool Gwen::Input::IsLeftMouseDown()
{
    return g_KeyData.LeftMouseDown;
}

bool Gwen::Input::IsRightMouseDown()
{
    return g_KeyData.RightMouseDown;
}

void Gwen::Input::OnMouseMoved(Controls::Base* pCanvas, int x, int y, int /*deltaX*/,
                               int /*deltaY*/)
{
    g_MousePosition.x = x;
    g_MousePosition.y = y;
    UpdateHoveredControl(pCanvas);
}

bool Gwen::Input::OnMouseClicked(Controls::Base* pCanvas, int iMouseButton, bool bDown)
{
    // If we click on a control that isn't a menu we want to close
    // all the open menus. Menus are children of the canvas.
    if (bDown && (!Gwen::HoveredControl || !Gwen::HoveredControl->IsMenuComponent()))
        pCanvas->CloseMenus();

    if (!Gwen::HoveredControl)
        return false;

    if (Gwen::HoveredControl->GetCanvas() != pCanvas)
        return false;

    if (!Gwen::HoveredControl->Visible())
        return false;

    if (Gwen::HoveredControl == pCanvas)
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
        && (Gwen::Platform::GetTimeInSeconds()-g_fLastClickTime[iMouseButton]) < DOUBLE_CLICK_SPEED)
    {
        bIsDoubleClick = true;
    }

    if (bDown && !bIsDoubleClick)
    {
        g_fLastClickTime[ iMouseButton ] = Gwen::Platform::GetTimeInSeconds();
        g_pntLastClickPos = g_MousePosition;
    }

    if (bDown)
    {
        if (!FindKeyboardFocus(Gwen::HoveredControl))
        {
            if (Gwen::KeyboardFocus)
                Gwen::KeyboardFocus->Blur();
        }
    }

    Gwen::HoveredControl->UpdateCursor();

    // This tells the child it has been touched, which
    // in turn tells its parents, who tell their parents.
    // This is basically so that Windows can pop themselves
    // to the top when one of their children has been clicked.
    if (bDown)
        Gwen::HoveredControl->Touch();

#ifdef GWEN_HOOKSYSTEM

    if (bDown)
    {
        if (Hook::CallHook(&Hook::BaseHook::OnControlClicked, Gwen::HoveredControl,
                           g_MousePosition.x,
                           g_MousePosition.y))
            return true;
    }

#endif

    switch (iMouseButton)
    {
    case 0:
        if (DragAndDrop::OnMouseButton(Gwen::HoveredControl,
                                       g_MousePosition.x, g_MousePosition.y,
                                       bDown))
        {
            return true;
        }

        if (bIsDoubleClick)
            Gwen::HoveredControl->OnMouseDoubleClickLeft(g_MousePosition.x, g_MousePosition.y);
        else
            Gwen::HoveredControl->OnMouseClickLeft(g_MousePosition.x, g_MousePosition.y, bDown);

        return true;

    case 1:
        if (bIsDoubleClick)
            Gwen::HoveredControl->OnMouseDoubleClickRight(g_MousePosition.x, g_MousePosition.y);
        else
            Gwen::HoveredControl->OnMouseClickRight(g_MousePosition.x, g_MousePosition.y, bDown);

        return true;
    }

    return false;
}

bool Gwen::Input::HandleAccelerator(Controls::Base* pCanvas, Gwen::UnicodeChar chr)
{
    // Build the accelerator search string
    Gwen::String accelString;

    if (Gwen::Input::IsControlDown())
        accelString += "CTRL+";

    if (Gwen::Input::IsShiftDown())
        accelString += "SHIFT+";

    chr = towupper(chr);
    accelString += chr;

    // Debug::Msg("Accelerator string :%S\n", accelString.c_str());

    if (Gwen::KeyboardFocus && Gwen::KeyboardFocus->HandleAccelerator(accelString))
        return true;

    if (Gwen::MouseFocus && Gwen::MouseFocus->HandleAccelerator(accelString))
        return true;

    if (pCanvas->HandleAccelerator(accelString))
        return true;

    return false;
}

bool Gwen::Input::DoSpecialKeys(Controls::Base* pCanvas, Gwen::UnicodeChar chr)
{
    if (!Gwen::KeyboardFocus)
        return false;

    if (Gwen::KeyboardFocus->GetCanvas() != pCanvas)
        return false;

    if (!Gwen::KeyboardFocus->Visible())
        return false;

    if (!Gwen::Input::IsControlDown())
        return false;

    if (chr == 'C' || chr == 'c')
    {
        Gwen::KeyboardFocus->OnCopy(NULL);
        return true;
    }

    if (chr == 'V' || chr == 'v')
    {
        Gwen::KeyboardFocus->OnPaste(NULL);
        return true;
    }

    if (chr == 'X' || chr == 'x')
    {
        Gwen::KeyboardFocus->OnCut(NULL);
        return true;
    }

    if (chr == 'A' || chr == 'a')
    {
        Gwen::KeyboardFocus->OnSelectAll(NULL);
        return true;
    }

    return false;
}

bool Gwen::Input::OnKeyEvent(Controls::Base* pCanvas, int iKey, bool bDown)
{
    Gwen::Controls::Base* pTarget = Gwen::KeyboardFocus;

    if (pTarget && pTarget->GetCanvas() != pCanvas)
        pTarget = NULL;

    if (pTarget && !pTarget->Visible())
        pTarget = NULL;

    if (bDown)
    {
        if (!g_KeyData.KeyState[iKey])
        {
            g_KeyData.KeyState[iKey] = true;
            g_KeyData.NextRepeat[iKey] = Gwen::Platform::GetTimeInSeconds()+KeyRepeatDelay;
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
