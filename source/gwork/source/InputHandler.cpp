/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/InputHandler.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/DragAndDrop.h>
#include <Gwork/Hook.h>
#include <Gwork/Platform.h>
#include <Gwork/Controls/Canvas.h>
#include <Gwork/Gwork.h>

namespace Gwk {

static const float      c_DoubleClickSpeed  = 0.5f;
static const float      c_KeyRepeatRate     = 0.03f;
static const float      c_KeyRepeatDelay    = 0.3f;
static const unsigned   c_MaxMouseButtons   = 5;

struct Action
{
    unsigned char       type;
    int                 x,y;
    Gwk::UnicodeChar    chr;
};

struct KeyData
{
    KeyData()
    {
        for (int i = 0; i < Key::KeysCount; i++)
        {
            KeyState[i] = false;
            NextRepeat[i] = 0;
        }

        Target = nullptr;
        LeftMouseDown = false;
        RightMouseDown = false;
    }

    bool    KeyState[Gwk::Key::KeysCount];
    float   NextRepeat[Gwk::Key::KeysCount];
    Controls::Base* Target;
    bool    LeftMouseDown;
    bool    RightMouseDown;

};

static KeyData      g_keyData;
static Gwk::Point   g_mousePosition;

static float        g_fLastClickTime[c_MaxMouseButtons];
static Gwk::Point   g_pntLastClickPos;


static void UpdateHoveredControl(Controls::Base* inCanvas)
{
    Controls::Base* hovered = inCanvas->GetControlAt(g_mousePosition.x, g_mousePosition.y);

    if (hovered != Gwk::HoveredControl)
    {
        if (Gwk::HoveredControl)
        {
            Controls::Base* OldHover = Gwk::HoveredControl;
            Gwk::HoveredControl = nullptr;
            OldHover->OnMouseLeave();
        }

        Gwk::HoveredControl = hovered;

        if (Gwk::HoveredControl)
            Gwk::HoveredControl->OnMouseEnter();
    }

    if (Gwk::MouseFocus && Gwk::MouseFocus->GetCanvas() == inCanvas)
    {
        if (Gwk::HoveredControl)
        {
            Controls::Base* OldHover = Gwk::HoveredControl;
            Gwk::HoveredControl = nullptr;
            OldHover->Redraw();
        }

        Gwk::HoveredControl = Gwk::MouseFocus;
    }
}

static bool FindKeyboardFocus(Controls::Base* control)
{
    if (!control)
        return false;

    if (control->GetKeyboardInputEnabled())
    {
        // Make sure none of our children have keyboard focus first
        //  todo recursive
        for (auto&& child : control->Children)
        {
            if (child == Gwk::KeyboardFocus)
                return false;
        }

        control->Focus();
        return true;
    }

    return FindKeyboardFocus(control->GetParent());
}

Gwk::Point Gwk::Input::GetMousePosition()
{
    return g_mousePosition;
}

void Gwk::Input::OnCanvasThink(Controls::Base* control)
{
    if (Gwk::MouseFocus && !Gwk::MouseFocus->Visible())
        Gwk::MouseFocus = nullptr;

    if (Gwk::KeyboardFocus
        && (!Gwk::KeyboardFocus->Visible() || !KeyboardFocus->GetKeyboardInputEnabled()))
    {
        Gwk::KeyboardFocus = nullptr;
    }

    if (!KeyboardFocus)
        return;

    if (KeyboardFocus->GetCanvas() != control)
        return;

    float fTime = Gwk::Platform::GetTimeInSeconds();

    // Simulate Key-Repeats
    for (int i = 0; i < Gwk::Key::KeysCount; i++)
    {
        if (g_keyData.KeyState[i] && g_keyData.Target != KeyboardFocus)
        {
            g_keyData.KeyState[i] = false;
            continue;
        }

        if (g_keyData.KeyState[i] && fTime > g_keyData.NextRepeat[i])
        {
            g_keyData.NextRepeat[i] = Gwk::Platform::GetTimeInSeconds()+c_KeyRepeatRate;

            if (KeyboardFocus)
                KeyboardFocus->OnKeyPress(i);
        }
    }
}

bool Gwk::Input::IsKeyDown(int iKey)
{
    return g_keyData.KeyState[ iKey ];
}

bool Gwk::Input::IsLeftMouseDown()
{
    return g_keyData.LeftMouseDown;
}

bool Gwk::Input::IsRightMouseDown()
{
    return g_keyData.RightMouseDown;
}

void Gwk::Input::OnMouseMoved(Controls::Base* canvas, int x, int y,
                              int /*deltaX*/, int /*deltaY*/)
{
    g_mousePosition.x = x;
    g_mousePosition.y = y;
    UpdateHoveredControl(canvas);
}

bool Gwk::Input::OnMouseClicked(Controls::Base* canvas, int iMouseButton, bool bDown)
{
    // If we click on a control that isn't a menu we want to close
    // all the open menus. Menus are children of the canvas.
    if (bDown && (!Gwk::HoveredControl || !Gwk::HoveredControl->IsMenuComponent()))
        canvas->CloseMenus();

    if (!Gwk::HoveredControl)
        return false;

    if (Gwk::HoveredControl->GetCanvas() != canvas)
        return false;

    if (!Gwk::HoveredControl->Visible())
        return false;

    if (Gwk::HoveredControl == canvas)
        return false;

    if (iMouseButton > static_cast<int>(c_MaxMouseButtons))
        return false;

    if (iMouseButton == 0)
        g_keyData.LeftMouseDown = bDown;
    else if (iMouseButton == 1)
        g_keyData.RightMouseDown = bDown;

    // Double click.
    // TODO - Shouldn't double click if mouse has moved significantly
    bool bIsDoubleClick = false;

    if (bDown
        && g_pntLastClickPos.x == g_mousePosition.x
        && g_pntLastClickPos.y == g_mousePosition.y
        && (Gwk::Platform::GetTimeInSeconds()-g_fLastClickTime[iMouseButton]) < c_DoubleClickSpeed)
    {
        bIsDoubleClick = true;
    }

    if (bDown && !bIsDoubleClick)
    {
        g_fLastClickTime[ iMouseButton ] = Gwk::Platform::GetTimeInSeconds();
        g_pntLastClickPos = g_mousePosition;
    }

    if (bDown)
    {
        if (!FindKeyboardFocus(Gwk::HoveredControl))
        {
            if (Gwk::KeyboardFocus)
                Gwk::KeyboardFocus->Unfocus();
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
                           g_mousePosition.x,
                           g_mousePosition.y))
        return true;
    }

#endif

    switch (iMouseButton)
    {
    case 0:
        if (DragAndDrop::OnMouseButton(Gwk::HoveredControl,
                                       g_mousePosition.x, g_mousePosition.y,
                                       bDown))
        {
            return true;
        }

        if (bIsDoubleClick)
            Gwk::HoveredControl->OnMouseDoubleClickLeft(g_mousePosition.x, g_mousePosition.y);
        else
            Gwk::HoveredControl->OnMouseClickLeft(g_mousePosition.x, g_mousePosition.y, bDown);

        return true;

    case 1:
        if (bIsDoubleClick)
            Gwk::HoveredControl->OnMouseDoubleClickRight(g_mousePosition.x, g_mousePosition.y);
        else
            Gwk::HoveredControl->OnMouseClickRight(g_mousePosition.x, g_mousePosition.y, bDown);

        return true;
    }

    return false;
}

bool Gwk::Input::HandleAccelerator(Controls::Base* canvas, Gwk::UnicodeChar chr)
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

    if (canvas->HandleAccelerator(accelString))
        return true;

    return false;
}

bool Gwk::Input::DoSpecialKeys(Controls::Base* canvas, Gwk::UnicodeChar chr)
{
    if (!Gwk::KeyboardFocus)
        return false;

    if (Gwk::KeyboardFocus->GetCanvas() != canvas)
        return false;

    if (!Gwk::KeyboardFocus->Visible())
        return false;

    if (!Gwk::Input::IsControlDown())
        return false;

    if (chr == 'C' || chr == 'c')
    {
        Gwk::KeyboardFocus->OnCopy(nullptr);
        return true;
    }

    if (chr == 'V' || chr == 'v')
    {
        Gwk::KeyboardFocus->OnPaste(nullptr);
        return true;
    }

    if (chr == 'X' || chr == 'x')
    {
        Gwk::KeyboardFocus->OnCut(nullptr);
        return true;
    }

    if (chr == 'A' || chr == 'a')
    {
        Gwk::KeyboardFocus->OnSelectAll(nullptr);
        return true;
    }

    return false;
}

bool Gwk::Input::OnKeyEvent(Controls::Base* canvas, int iKey, bool bDown)
{
    Gwk::Controls::Base* target = Gwk::KeyboardFocus;

    if (target && target->GetCanvas() != canvas)
        target = nullptr;

    if (target && !target->Visible())
        target = nullptr;

    if (bDown)
    {
        if (!g_keyData.KeyState[iKey])
        {
            g_keyData.KeyState[iKey] = true;
            g_keyData.NextRepeat[iKey] = Gwk::Platform::GetTimeInSeconds()+c_KeyRepeatDelay;
            g_keyData.Target = target;

            if (target)
                return target->OnKeyPress(iKey);
        }
    }
    else
    {
        if (g_keyData.KeyState[iKey])
        {
            g_keyData.KeyState[iKey] = false;

            //! @bug This causes shift left arrow in textboxes
            //! to not work. What is disabling it here breaking?
            //! `g_keyData.Target = nullptr;`
            
            if (target)
                return target->OnKeyRelease(iKey);
        }
    }

    return false;
}

} // namespace Gwk
