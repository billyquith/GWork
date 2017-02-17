/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Controls/ImagePanel.h>

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(Button)
{
    m_image = nullptr;
    m_bDepressed = false;
    m_bCenterImage = false;
    SetSize(100, 20);
    SetMouseInputEnabled(true);
    SetIsToggle(false);
    SetAlignment(Position::Center);
    SetTextPadding(Padding(3, 0, 3, 0));
    m_bToggleStatus = false;
    SetKeyboardInputEnabled(false);
    SetTabable(false);
}

void Button::Render(Skin::Base* skin)
{
    if (!ShouldDrawBackground())
        return;

    bool drawDepressed = IsDepressed() && IsHovered();

    if (IsToggle())
        drawDepressed = drawDepressed || GetToggleState();

    const bool drawHovered = IsHovered() && ShouldDrawHover();
    
    skin->DrawButton(this, drawDepressed, drawHovered, IsDisabled());
}

void Button::OnMouseClickLeft(int /*x*/, int /*y*/, bool bDown)
{
    if (IsDisabled())
        return;

    if (bDown)
    {
        SetDepressed(true);
        Gwk::MouseFocus = this;
        onDown.Call(this);
    }
    else
    {
        if (IsHovered() && m_bDepressed)
        {
            OnPress(Event::Info(this));
        }

        SetDepressed(false);
        Gwk::MouseFocus = nullptr;
        onUp.Call(this);
    }
}

void Button::OnMouseClickRight(int /*x*/, int /*y*/, bool bDown)
{
    if (IsDisabled())
        return;

    if (bDown)
    {
        SetDepressed(true);
        Gwk::MouseFocus = this;
        onDown.Call(this);
    }
    else
    {
        if (IsHovered() && m_bDepressed)
        {
            OnRightPress(Event::Info(this));
        }

        SetDepressed(false);
        Gwk::MouseFocus = nullptr;
        onUp.Call(this);
    }
}

void Button::SetDepressed(bool b)
{
    if (m_bDepressed == b)
        return;

    m_bDepressed = b;
    Redraw();
}

void Button::OnRightPress(Event::Info)
{
    onRightPress.Call(this);
}

void Button::OnPress(Event::Info)
{
    if (IsToggle())
        SetToggleState(!GetToggleState());

    onPress.Call(this);
}

void Button::SetImage(const String& strName, bool bCenter)
{
    if (strName.empty())
    {
        if (m_image)
        {
            delete m_image;
            m_image = nullptr;
        }

        return;
    }

    if (!m_image)
        m_image = new ImagePanel(this);

    m_image->SetImage(strName);
    m_image->SizeToContents();
    m_image->SetMargin(Margin(2, 0, 2, 0));
    m_bCenterImage = bCenter;
    // Ugh.
    Padding padding = GetTextPadding();
    padding.left = m_image->Right()+2;
    SetTextPadding(padding);
}

void Button::SetToggleState(bool b)
{
    if (m_bToggleStatus == b)
        return;

    m_bToggleStatus = b;
    onToggle.Call(this);

    if (m_bToggleStatus)
        onToggleOn.Call(this);
    else
        onToggleOff.Call(this);

    Redraw();
}

void Button::SizeToContents()
{
    ParentClass::SizeToContents();

    if (m_image)
    {
        int height = m_image->Height()+4;

        if (Height() < height)
            SetHeight(height);
    }
}

bool Button::OnKeySpace(bool bDown)
{
    if (bDown)
    {
        OnPress(Event::Info(this));
    }

    return true;
}

void Button::AcceleratePressed()
{
    OnPress(Event::Info(this));
}

void Button::UpdateColors()
{
    if (IsDisabled())
        return SetTextColor(GetSkin()->Colors.Button.Disabled);

    if (IsDepressed() || GetToggleState())
        return SetTextColor(GetSkin()->Colors.Button.Down);

    if (IsHovered())
        return SetTextColor(GetSkin()->Colors.Button.Hover);

    SetTextColor(GetSkin()->Colors.Button.Normal);
}

void Button::PostLayout(Skin::Base* skin)
{
    ParentClass::PostLayout(skin);

    if (m_image)
    {
        if (m_bCenterImage)
            m_image->SetPosition(Position::Center);
        else
            m_image->SetPosition(Position::Left|Position::CenterV);
    }
}

void Button::OnMouseDoubleClickLeft(int x, int y)
{
    if (IsDisabled())
        return;

    OnMouseClickLeft(x, y, true);
    onDoubleClick.Call(this);
}

void Button::SetImageAlpha(float f)
{
    if (!m_image)
        return;

    m_image->SetDrawColor(Gwk::Color(255, 255, 255, 255.0f*f));
}

void Button::SetAction(Event::Handler* object, Event::Listener::EventListener function,
                       const Gwk::Event::Packet& packet)
{
    onPress.Add(object, function, packet);
}

