/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Controls/WindowControl.h>
#include <Gwork/Controls/ImagePanel.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Controls/Modal.h>


using namespace Gwk;
using namespace Gwk::Controls;
using namespace Gwk::ControlsInternal;


GWK_CONTROL_CONSTRUCTOR(WindowControl)
{
    m_modal = nullptr;
    m_bDeleteOnClose = false;
    m_titleBar = new Dragger(this);
    m_titleBar->SetHeight(24);
    m_titleBar->SetPadding(Padding(0, 0, 0, 0));
    m_titleBar->SetMargin(Margin(0, 0, 0, 4));
    m_titleBar->SetTarget(this);
    m_titleBar->Dock(Position::Top);
    m_title = new Label(m_titleBar);
    m_title->SetAlignment(Position::Left|Position::CenterV);
    m_title->SetText("Window");
    m_title->Dock(Position::Fill);
    m_title->SetPadding(Padding(8, 0, 0, 0));
    m_title->SetTextColor(GetSkin()->Colors.Window.TitleInactive);
    m_closeButton = new Gwk::Controls::WindowCloseButton(m_titleBar);
    m_closeButton->SetText("");
    m_closeButton->Dock(Position::Right);
    m_closeButton->onPress.Add(this, &WindowControl::CloseButtonPressed);
    m_closeButton->SetTabable(false);
    m_closeButton->SetName("closeButton");
    m_closeButton->SetWindow(this);
    // Create a blank content control, dock it to the top - Should this be a
    // ScrollControl?
    m_innerPanel = new Base(this);
    m_innerPanel->Dock(Position::Fill);
    GetResizer(8)->Hide();
    BringToFront();
    SetTabable(false);
    Focus();
    SetMinimumSize(Gwk::Point(100, 40));
    SetClampMovement(true);
    SetKeyboardInputEnabled(false);
}


WindowControl::~WindowControl()
{
    DestroyModal();
}

void WindowControl::MakeModal(bool bDrawBackground)
{
    if (m_modal)
        return;

    m_modal = new ControlsInternal::Modal(GetCanvas());
    SetParent(m_modal);
    m_modal->SetShouldDrawBackground(bDrawBackground);
}

void WindowControl::DestroyModal()
{
    if (!m_modal)
        return;

    // Really should be restoring our parent here.. but we don't know who it is.
    // Assume it's the canvas.
    SetParent(GetCanvas());
    m_modal->DelayedDelete();
    m_modal = nullptr;
}

bool WindowControl::IsOnTop()
{
    for (Base::List::reverse_iterator iter = GetParent()->Children.rbegin();
         iter != GetParent()->Children.rend();
         ++iter)
    {
        WindowControl* window = gwk_cast<WindowControl>(*iter);

        if (!window)
            continue;

        if (window == this)
            return true;

        return false;
    }

    return false;
}

void WindowControl::Render(Skin::Base* skin)
{
    bool bIsFocussed = IsOnTop();

    if (bIsFocussed)
        m_title->SetTextColor(GetSkin()->Colors.Window.TitleActive);
    else
        m_title->SetTextColor(GetSkin()->Colors.Window.TitleInactive);

    skin->DrawWindow(this, m_titleBar->Bottom(), bIsFocussed);
}

void WindowControl::RenderUnder(Skin::Base* skin)
{
    ParentClass::RenderUnder(skin);
    skin->DrawShadow(this);
}

void WindowControl::SetTitle(Gwk::String title)
{
    m_title->SetText(title);
}

void WindowControl::SetClosable(bool closeable)
{
    m_closeButton->SetHidden(!closeable);
}

void WindowControl::SetHidden(bool hidden)
{
    if (!hidden)
        BringToFront();

    ParentClass::SetHidden(hidden);
}

void WindowControl::Touch()
{
    ParentClass::Touch();
    BringToFront();
}

void WindowControl::CloseButtonPressed(Event::Info)
{
    DestroyModal();
    onWindowClosed.Call(this);
    SetHidden(true);

    if (m_bDeleteOnClose)
        DelayedDelete();
}

void WindowControl::RenderFocus(Gwk::Skin::Base* /*skin*/)
{
}
