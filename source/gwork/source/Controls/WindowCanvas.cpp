/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Controls/WindowCanvas.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/Menu.h>
#include <Gwork/DragAndDrop.h>
#include <Gwork/ToolTip.h>

#if GWK_ANIMATE
#include <Gwork/Anim.h>
#endif

using namespace Gwk;
using namespace Gwk::Controls;

WindowCanvas::WindowCanvas(int x, int y, int w, int h, Gwk::Skin::Base* skin,
                           const Gwk::String& strWindowTitle)
: ParentClass(nullptr)
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
    
    m_oSWindowSWindow = Gwk::Platform::CreatePlatformWindow(x, y, w, h, strWindowTitle);
    m_windowPos  = Gwk::Point(x, y);
    skin->GetRender()->InitializeContext(this);
    skin->GetRender()->Init();
    m_skinChange = skin;
    SetSize(w, h);
    
    m_titleBar = new Gwk::ControlsInternal::Dragger(this);
    m_titleBar->SetHeight(24);
    m_titleBar->SetPadding(Padding(0, 0, 0, 0));
    m_titleBar->SetMargin(Margin(0, 0, 0, 0));
    m_titleBar->Dock(Position::Top);
    m_titleBar->SetDoMove(false);
    m_titleBar->onDragged.Add(this, &ThisClass::Dragger_Moved);
    m_titleBar->onDragStart.Add(this, &ThisClass::Dragger_Start);
    m_titleBar->onDoubleClickLeft.Add(this, &ThisClass::OnTitleDoubleClicked);
    
    m_title = new Gwk::Controls::Label(m_titleBar);
    m_title->SetAlignment(Position::Left|Position::CenterV);
    m_title->SetText(strWindowTitle);
    m_title->Dock(Position::Fill);
    m_title->SetPadding(Padding(8, 0, 0, 0));
    m_title->SetTextColor(GetSkin()->Colors.Window.TitleInactive);
    
    // CLOSE
    {
        m_close = new Gwk::Controls::WindowCloseButton(m_titleBar, "Close");
        m_close->Dock(Position::Right);
        m_close->SetMargin(Margin(0, 0, 4, 0));
        m_close->onPress.Add(this, &WindowCanvas::CloseButtonPressed);
        m_close->SetTabable(false);
        m_close->SetWindow(this);
    }
    // MAXIMIZE
    {
        m_maximize = new Gwk::Controls::WindowMaximizeButton(m_titleBar, "Maximize");
        m_maximize->Dock(Position::Right);
        m_maximize->onPress.Add(this, &WindowCanvas::MaximizeButtonPressed);
        m_maximize->SetTabable(false);
        m_maximize->SetWindow(this);
    }
    // MINIMiZE
    {
        m_minimize = new Gwk::Controls::WindowMinimizeButton(m_titleBar, "Minimize");
        m_minimize->Dock(Position::Right);
        m_minimize->onPress.Add(this, &WindowCanvas::MinimizeButtonPressed);
        m_minimize->SetTabable(false);
        m_minimize->SetWindow(this);
    }
    // Bottom Right Corner Sizer
    {
        m_sizer = new Gwk::ControlsInternal::Dragger(this);
        m_sizer->SetSize(16, 16);
        m_sizer->SetDoMove(false);
        m_sizer->onDragged.Add(this, &WindowCanvas::Sizer_Moved);
        m_sizer->onDragStart.Add(this, &WindowCanvas::Dragger_Start);
        m_sizer->SetCursor(Gwk::CursorType::SizeNWSE);
    }
}

WindowCanvas::~WindowCanvas()
{
    DestroyWindow();
}

void* WindowCanvas::GetWindow()
{
    return m_oSWindowSWindow;
}

void WindowCanvas::Layout(Skin::Base* skin)
{
    m_sizer->BringToFront();
    m_sizer->SetPosition(Position::Right|Position::Bottom);
    ParentClass::Layout(skin);
}

void WindowCanvas::DoThink()
{
    if (Platform::MessagePump(m_oSWindowSWindow))
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
    Gwk::Renderer::Base* render = m_skin->GetRender();

    if (render->BeginContext(this))
    {
        render->Begin();
        RecurseLayout(m_skin);
        render->SetClipRegion(GetRenderBounds());
        render->SetRenderOffset( Gwk::Point(-X(), -Y()) );
        render->SetScale(Scale());

        if (m_bDrawBackground)
        {
            render->SetDrawColor(m_backgroundColor);
            render->DrawFilledRect(GetRenderBounds());
        }

        DoRender(m_skin);
        DragAndDrop::RenderOverlay(this, m_skin);
        ToolTip::RenderToolTip(m_skin);
        render->End();
    }

    render->EndContext(this);
    render->PresentContext(this);
}

void WindowCanvas::Render(Skin::Base* skin)
{
    bool bIsFocussed = IsOnTop();

    if (bIsFocussed)
        m_title->SetTextColor(GetSkin()->Colors.Window.TitleActive);
    else
        m_title->SetTextColor(GetSkin()->Colors.Window.TitleInactive);

    skin->DrawWindow(this, m_titleBar->Bottom(), bIsFocussed);
}

void WindowCanvas::DestroyWindow()
{
    if (m_oSWindowSWindow)
    {
        GetSkin()->GetRender()->ShutdownContext(this);
        Gwk::Platform::DestroyPlatformWindow(m_oSWindowSWindow);
        m_oSWindowSWindow = nullptr;
    }
}

bool WindowCanvas::InputQuit()
{
    m_bQuit = true;
    return true;
}

Skin::Base* WindowCanvas::GetSkin(void)
{
    if (m_skinChange)
    {
        SetSkin(m_skinChange);
        m_skinChange = nullptr;
    }

    return ParentClass::GetSkin();
}

void WindowCanvas::Dragger_Start(Event::Info)
{
    Gwk::Platform::GetCursorPos(m_holdPos);
    m_holdPos.x -= m_windowPos.x;
    m_holdPos.y -= m_windowPos.y;
}

void WindowCanvas::Dragger_Moved(Event::Info)
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
        m_holdPos.x = ((float)m_holdPos.x)*((float)Width()/fOldWidth);
        m_holdPos.y = 10;
    }

    SetPos(p.x-m_holdPos.x, p.y-m_holdPos.y);
}

void WindowCanvas::SetPos(int x, int y)
{
    int w, h;
    Gwk::Platform::GetDesktopSize(w, h);
    y = Gwk::Clamp(y, 0, h);
    m_windowPos.x = x;
    m_windowPos.y = y;
    Gwk::Platform::SetBoundsPlatformWindow(m_oSWindowSWindow, x, y, Width(), Height());
}

void WindowCanvas::CloseButtonPressed(Event::Info)
{
    InputQuit();
}

bool WindowCanvas::IsOnTop()
{
    return Gwk::Platform::IsFocussedPlatformWindow(m_oSWindowSWindow);
}

void WindowCanvas::Sizer_Moved(Event::Info)
{
    Gwk::Point p;
    Gwk::Platform::GetCursorPos(p);
    int w = (p.x)-m_windowPos.x;
    int h = (p.y)-m_windowPos.y;
    w = Clamp(w, 100, 9999);
    h = Clamp(h, 100, 9999);
    Gwk::Platform::SetBoundsPlatformWindow(m_oSWindowSWindow, m_windowPos.x, m_windowPos.y, w, h);
    GetSkin()->GetRender()->ResizedContext(this, w, h);
    this->SetSize(w, h);
    ParentClass::DoThink();
    RenderCanvas();
}

void WindowCanvas::OnTitleDoubleClicked(Event::Info)
{
    if (!CanMaximize())
        return;

    SetMaximize(!m_bIsMaximized);
}

void WindowCanvas::SetMaximize(bool b)
{
    m_bIsMaximized = b;
    m_maximize->SetMaximized(m_bIsMaximized);
    Gwk::Point size, pos;
    Gwk::Platform::SetWindowMaximized(m_oSWindowSWindow, m_bIsMaximized, pos, size);
    SetSize(size.x, size.y);
    m_windowPos = pos;
    GetSkin()->GetRender()->ResizedContext(this, size.x, size.y);
    ParentClass::DoThink();
    RenderCanvas();
}

void WindowCanvas::MaximizeButtonPressed(Event::Info)
{
    if (!CanMaximize())
        return;

    SetMaximize(!m_bIsMaximized);
}

void WindowCanvas::MinimizeButtonPressed(Event::Info)
{
    Gwk::Platform::SetWindowMinimized(m_oSWindowSWindow, true);
}

void WindowCanvas::SetCanMaximize(bool b)
{
    if (m_bCanMaximize == b)
        return;

    m_bCanMaximize = b;
    m_maximize->SetDisabled(!b);
}
