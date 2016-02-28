/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_PROPERTY_COLORSELECTOR_H
#define GWK_CONTROLS_PROPERTY_COLORSELECTOR_H

#include "Gwork/Controls/Properties.h"
#include "Gwork/Controls/Menu.h"
#include "Gwork/Controls/HSVColorPicker.h"

#include <stdio.h>

namespace Gwk
{
    namespace Controls
    {
        namespace Internal
        {
            class ColourButton : public Button
            {
                GWK_CONTROL_INLINE(ColourButton, Button)
                {
                    m_Color = Colors::Black;
                    SetText("");
                }

                void Render(Skin::Base* skin) override
                {
                    skin->GetRender()->SetDrawColor(m_Color);
                    skin->GetRender()->DrawFilledRect(GetRenderBounds());
                }

                void SetColor(const Gwk::Color& col)
                {
                    m_Color = col;
                }

                Gwk::Color m_Color;
            };


        }

        namespace Property
        {
            class ColorSelector : public Property::Text
            {
                GWK_CONTROL_INLINE(ColorSelector, Property::Text)
                {
                    m_Button = new Controls::Internal::ColourButton(m_TextBox);
                    m_Button->Dock(Docking::Right);
                    m_Button->SetWidth(20);
                    m_Button->onPress.Add(this, &ThisClass::OnButtonPress);
                    m_Button->SetMargin(Margin(1, 1, 1, 2));
                }

                void OnButtonPress(Controls::Base* control)
                {
                    Gwk::Controls::Menu* pMenu = new Menu(GetCanvas());
                    pMenu->SetSize(256, 180);
                    pMenu->SetDeleteOnClose(true);
                    pMenu->SetDisableIconMargin(true);
                    
                    Gwk::Controls::HSVColorPicker* picker = new Gwk::Controls::HSVColorPicker(pMenu);
                    picker->Dock(Docking::Fill);
                    picker->SetSize(256, 128);
                    float defaultColor[3];
                    Gwk::Utility::Strings::To::Floats(m_TextBox->GetText(), defaultColor, 3);
                    picker->SetColor(Gwk::Color(defaultColor[0], defaultColor[1], defaultColor[2], 255),
                                     false, true);
                    picker->onColorChanged.Add(this, &ThisClass::ColorChanged);
                    
                    pMenu->Open(Docking::Right|Docking::Top);
                }

                void ColorChanged(Controls::Base* control)
                {
                    Gwk::Controls::HSVColorPicker* picker =
                        gwk_cast<Gwk::Controls::HSVColorPicker>(control);
                    Color col = picker->GetColor();
                    m_TextBox->SetText(Utility::Format("%u,%u,%u", col.r, col.g, col.b));
                    DoChanged();
                }

                virtual String GetPropertyValue() override
                {
                    return m_TextBox->GetText();
                }

                virtual void SetPropertyValue(const String& v, bool bFireChangeEvents) override
                {
                    m_TextBox->SetText(v, bFireChangeEvents);
                }

                virtual bool IsEditing() override
                {
                    return m_TextBox == Gwk::KeyboardFocus;
                }

                virtual void DoChanged() override
                {
                    ParentClass::DoChanged();
                    unsigned int r,g,b;
#ifdef _WIN32
                    if (sscanf_s(m_TextBox->GetText().c_str(), "%u,%u,%u", &r, &g, &b) == 3)
#else
                    if (sscanf(m_TextBox->GetText().c_str(), "%u,%u,%u", &r, &g, &b) == 3)
#endif
                    {
                        m_Button->SetColor(Gwk::Color(r,g,b));
                    }
                }

                Controls::Internal::ColourButton*       m_Button;
            };


        }
    }
}
#endif // ifndef GWK_CONTROLS_PROPERTY_COLORSELECTOR_H
