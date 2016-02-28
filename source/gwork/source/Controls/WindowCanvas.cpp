/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include "Gwork/Gwork.h"
#include "Gwork/Controls/WindowCanvas.h"
#include "Gwork/Skin.h"
#include "Gwork/Controls/Menu.h"
#include "Gwork/DragAndDrop.h"
#include "Gwork/ToolTip.h"

#ifndef GWK_NO_ANIMATION
#include "Gwork/Anim.h"
#endif

using namespace Gwk;
using namespace Gwk::Controls;

WindowCanvas::WindowCanvas(int x, int y, int w, int h, Gwk::Skin::Base* pSkin,
                           const Gwk::String& strWindowTitle)
: ParentClass(NULL)
{
    m_bQuit = false;
    m_bCanMaximize = true;
    m_bIsMaximized = false;
    SetPadding(Padding(1, 0, 1, 1));
    
    // Centering the window on the desktop
    {
        int dw, dh;
        Gwk::Platform::GetDesktopSize(dw, dh);

        if (x < 0)
            x = (dw-w)/2;

        if (y < 0)
            y = (dh-h)/2;
    }
    
    m_pOSWindow = Gwk::Platform::CreatePlatformWindow(x, y, w, h, strWindowTitle);
    m_WindowPos  = Gwk::Point(x, y);
    pSkin->GetRender()->InitializeContext(this);
    pSkin->GetRender()->Init();
    m_pSkinChange = pSkin;
    SetSize(w, h);
    
    m_TitleBar = new Gwk::ControlsInternal::Dragger(this);
    m_TitleBar->SetHeight(24);
    m_TitleBar->SetPadding(Padding(0, 0, 0, 0));
    m_TitleBar->SetMargin(Margin(0, 0, 0, 0));
    m_TitleBar->Dock(Docking::Top);
    m_TitleBar->SetDoMove(false);
    m_TitleBar->onDragged.Add(this, &ThisClass::Dragger_Moved);
    m_TitleBar->onDragStart.Add(this, &ThisClass::Dragger_Start);
    m_TitleBar->onDoubleClickLeft.Add(this, &ThisClass::OnTitleDoubleClicked);
    
    m_Title = new Gwk::Controls::Label(m_TitleBar);
    m_Title->SetAlignment(Docking::Left|Docking::CenterV);
    m_Title->SetText(strWindowTitle);
    m_Title->Dock(Docking::Fill);
    m_Title->SetPadding(Padding(8, 0, 0, 0));
    m_Title->SetTextColor(GetSkin()->Colors.Window.TitleInactive);
    
    // CLOSE
    {
        m_pClose = new Gwk::Controls::WindowCloseButton(m_TitleBar, "Close");
        m_pClose->Dock(Docking::Right);
        m_pClose->SetMargin(Margin(0, 0, 4, 0));
        m_pClose->onPress.Add(this, &WindowCanvas::CloseButtonPressed);
        m_pClose->SetTabable(false);
        m_pClose->SetWindow(this);
    }
    // MAXIMIZE
    {
        m_pMaximize = new Gwk::Controls::WindowMaximizeButton(m_TitleBar, "Maximize");
        m_pMaximize->Dock(Docking::Right);
        m_pMaximize->onPress.Add(this, &WindowCanvas::MaximizeButtonPressed);
        m_pMaximize->SetTabable(false);
        m_pMaximize->SetWindow(this);
    }
    // MINIMiZE
    {
        m_pMinimize = new Gwk::Controls::WindowMinimizeButton(m_TitleBar, "Minimize");
        m_pMinimize->Dock(Docking::Right);
        m_pMinimize->onPress.Add(this, &WindowCanvas::MinimizeButtonPressed);
        m_pMinimize->SetTabable(false);
        m_pMinimize->SetWindow(this);
    }
    // Bottom Right Corner Sizer
    {
        m_Sizer = new Gwk::ControlsInternal::Dragger(this);
        m_Sizer->SetSize(16, 16);
        m_Sizer->SetDoMove(false);
        m_Sizer->onDragged.Add(this, &WindowCanvas::Sizer_Moved);
        m_Sizer->onDragStart.Add(this, &WindowCanvas::Dragger_Start);
        m_Sizer->SetCursor(Gwk::CursorType::SizeNWSE);
    }
}

WindowCanvas::~WindowCanvas()
{
    DestroyWindow();
}

void* WindowCanvas::GetWindow()
{
    return m_pOSWindow;
}

void WindowCanvas::Layout(Skin::Base* skin)
{
    m_Sizer->BringToFront();
    m_Sizer->Position(Docking::Right|Docking::Bottom);
    ParentClass::Layout(skin);
}

void WindowCanvas::DoThink()
{
    if (Platform::MessagePump(m_pOSWindow))
    {
        Redraw();
    }
    ParentClass::DoThink();
    RenderCanvas();
}

void WindowCanvas::RenderCanvas()
{
    //
    // If there isn't anything going on we sleep the thread for a few ms
    // This gives some cpu time back to the os. If you're using a rendering
    // method that needs continual updates, just call canvas->redraw every
    // frame.
    //
    if (!NeedsRedraw())
    {
        Platform::Sleep(10);
        return;
    }

    m_bNeedsRedraw = false;
    Gwk::Renderer::Base* render = m_Skin->GetRender();

    if (render->BeginContext(this))
    {
        render->Begin();
        RecurseLayout(m_Skin);
        render->SetClipRegion(GetRenderBounds());
        render->SetRenderOffset( Gwk::Point(-X(), -Y()) );
        render->SetScale(Scale());

        if (m_bDrawBackground)
        {
            render->SetDrawColor(m_BackgroundColor);
            render->DrawFilledRect(GetRenderBounds());
        }

        DoRender(m_Skin);
        DragAndDrop::RenderOverlay(this, m_Skin);
        ToolTip::RenderToolTip(m_Skin);
        render->End();
    }

    render->EndContext(this);
    render->PresentContext(this);
}

void WindowCanvas::Render(Skin::Base* skin)
{
    bool bHasFocus = IsOnTop();

    if (bHasFocus)
        m_Title->SetTextColor(GetSkin()->Colors.Window.TitleActive);
    else
        m_Title->SetTextColor(GetSkin()->Colors.Window.TitleInactive);

    skin->DrawWindow(this, m_TitleBar->Bottom(), bHasFocus);
}

void WindowCanvas::DestroyWindow()
{
    if (m_pOSWindow)
    {
        GetSkin()->GetRender()->ShutdownContext(this);
        Gwk::Platform::DestroyPlatformWindow(m_pOSWindow);
        m_pOSWindow = NULL;
    }
}

bool WindowCanvas::InputQuit()
{
    m_bQuit = true;
    return true;
}

Skin::Base* WindowCanvas::GetSkin(void)
{
    if (m_pSkinChange)
    {
        SetSkin(m_pSkinChange);
        m_pSkinChange = NULL;
    }

    return ParentClass::GetSkin();
}

void WindowCanvas::Dragger_Start()
{
    Gwk::Platform::GetCursorPos(m_HoldPos);
    m_HoldPos.x -= m_WindowPos.x;
    m_HoldPos.y -= m_WindowPos.y;
}

void WindowCanvas::Dragger_Moved()
{
    Gwk::Point p;
    Gwk::Platform::GetCursorPos(p);

    //
    // Dragged out of maximized
    //
    if (m_bIsMaximized)
    {
        float fOldWidth = Width();
        SetMaximize(false);
        // Change the hold pos to be the same distance across the titlebar of
        // the resized window
        m_HoldPos.x = ((float)m_HoldPos.x)*((float)Width()/fOldWidth);
        m_HoldPos.y = 10;
    }

    SetPos(p.x-m_HoldPos.x, p.y-m_HoldPos.y);
}

void WindowCanvas::SetPos(int x, int y)
{
    int w, h;
    Gwk::Platform::GetDesktopSize(w, h);
    y = Gwk::Clamp(y, 0, h);
    m_WindowPos.x = x;
    m_WindowPos.y = y;
    Gwk::Platform::SetBoundsPlatformWindow(m_pOSWindow, x, y, Width(), Height());
}

void WindowCanvas::CloseButtonPressed()
{
    InputQuit();
}

bool WindowCanvas::IsOnTop()
{
    return Gwk::Platform::HasFocusPlatformWindow(m_pOSWindow);
}

void WindowCanvas::Sizer_Moved()
{
    Gwk::Point p;
    Gwk::Platform::GetCursorPos(p);
    int w = (p.x)-m_WindowPos.x;
    int h = (p.y)-m_WindowPos.y;
    w = Clamp(w, 100, 9999);
    h = Clamp(h, 100, 9999);
    Gwk::Platform::SetBoundsPlatformWindow(m_pOSWindow, m_WindowPos.x, m_WindowPos.y, w, h);
    GetSkin()->GetRender()->ResizedContext(this, w, h);
    this->SetSize(w, h);
    ParentClass::DoThink();
    RenderCanvas();
}

void WindowCanvas::OnTitleDoubleClicked()
{
    if (!CanMaximize())
        return;

    SetMaximize(!m_bIsMaximized);
}

void WindowCanvas::SetMaximize(bool b)
{
    m_bIsMaximized = b;
    m_pMaximize->SetMaximized(m_bIsMaximized);
    Gwk::Point pSize, pPos;
    Gwk::Platform::SetWindowMaximized(m_pOSWindow, m_bIsMaximized, pPos, pSize);
    SetSize(pSize.x, pSize.y);
    m_WindowPos = pPos;
    GetSkin()->GetRender()->ResizedContext(this, pSize.x, pSize.y);
    ParentClass::DoThink();
    RenderCanvas();
}

void WindowCanvas::MaximizeButtonPressed()
{
    if (!CanMaximize())
        return;

    SetMaximize(!m_bIsMaximized);
}

void WindowCanvas::MinimizeButtonPressed()
{
    Gwk::Platform::SetWindowMinimized(m_pOSWindow, true);
}

void WindowCanvas::SetCanMaximize(bool b)
{
    if (m_bCanMaximize == b)
        return;

    m_bCanMaximize = b;
    m_pMaximize->SetDisabled(!b);
}
