/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_PROPERTY_COLORSELECTOR_H
#define GWK_CONTROLS_PROPERTY_COLORSELECTOR_H

#include <Gwork/Controls/Properties.h>
#include <Gwork/Controls/Menu.h>
#include <Gwork/Controls/HSVColorPicker.h>
#include <Gwork/Controls/Canvas.h>
#include <stdio.h>

namespace Gwk
{
    namespace Controls
    {
        namespace Internal
        {
            class ColorButton : public Button
            {
                GWK_CONTROL_INLINE(ColorButton, Button)
                {
                    m_color = Colors::Black;
                    SetText("");
                }

                void Render(Skin::Base* skin) override
                {
                    skin->GetRender()->SetDrawColor(m_color);
                    skin->GetRender()->DrawFilledRect(GetRenderBounds());
                }

                void SetColor(const Gwk::Color& col)
                {
                    m_color = col;
                }

                Gwk::Color m_color;
            };


        }

        namespace Property
        {
            class ColorSelector : public Property::Text
            {
                GWK_CONTROL_INLINE(ColorSelector, Property::Text)
                {
                    m_button = new Controls::Internal::ColorButton(m_textBox);
                    m_button->Dock(Position::Right);
                    m_button->SetWidth(20);
                    m_button->onPress.Add(this, &ThisClass::OnButtonPress);
                    m_button->SetMargin(Margin(1, 1, 1, 2));
                }

                void OnButtonPress(Event::Info info)
                {
                    Gwk::Controls::Menu* menu = new Menu(GetCanvas());
                    menu->SetSize(256, 180);
                    menu->SetDeleteOnClose(true);
                    menu->SetDisableIconMargin(true);
                    
                    Gwk::Controls::HSVColorPicker* picker = new Gwk::Controls::HSVColorPicker(menu);
                    picker->Dock(Position::Fill);
                    picker->SetSize(256, 128);
                    float defaultColor[3];
                    Gwk::Utility::Strings::To::Floats(m_textBox->GetText(), defaultColor, 3);
                    picker->SetColor(Gwk::Color(defaultColor[0], defaultColor[1], defaultColor[2], 255),
                                     false, true);
                    picker->onColorChanged.Add(this, &ThisClass::ColorChanged);
                    
                    menu->Open(Position::Right | Position::Top);
                }

                void ColorChanged(Event::Info info)
                {
                    Gwk::Controls::HSVColorPicker* picker =
                        gwk_cast<Gwk::Controls::HSVColorPicker>(info.ControlCaller);
                    Color col = picker->GetColor();
                    m_textBox->SetText(Utility::Format("%u,%u,%u", col.r, col.g, col.b));
                    DoChanged();
                }

                String GetPropertyValue() override
                {
                    return m_textBox->GetText();
                }

                void SetPropertyValue(const String& v, bool bFireChangeEvents) override
                {
                    m_textBox->SetText(v, bFireChangeEvents);
                }

                bool IsEditing() override
                {
                    return m_textBox == Gwk::KeyboardFocus;
                }

                void DoChanged() override
                {
                    ParentClass::DoChanged();
                    unsigned int r,g,b;
#if defined(_WIN32) && !defined(__MINGW32__)
                    if (sscanf_s(m_textBox->GetText().c_str(), "%u,%u,%u", &r, &g, &b) == 3)
#else
                    if (sscanf(m_textBox->GetText().c_str(), "%u,%u,%u", &r, &g, &b) == 3)
#endif
                    {
                        m_button->SetColor(Gwk::Color(r,g,b));
                    }
                }

                Controls::Internal::ColorButton*       m_button;
            };


        }
    }
}
#endif // ifndef GWK_CONTROLS_PROPERTY_COLORSELECTOR_H
