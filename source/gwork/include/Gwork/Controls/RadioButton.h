/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_RADIOBUTTON_H
#define GWK_CONTROLS_RADIOBUTTON_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/CheckBox.h>
#include <Gwork/Controls/LabelClickable.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT RadioButton : public CheckBox
        {
            GWK_CONTROL(RadioButton, CheckBox);
            void Render(Skin::Base* skin) override;

        private:

            // From CheckBox
            bool AllowUncheck() override
            {
                return false;
            }

        };


        class GWK_EXPORT LabeledRadioButton : public Base
        {
        public:

            GWK_CONTROL_INLINE(LabeledRadioButton, Base)
            {
                SetSize(200, 19);
                m_radioButton = new RadioButton(this);
                m_radioButton->Dock(Position::Left);
                m_radioButton->SetMargin(Margin(0, 2, 2, 2));
                m_radioButton->SetTabable(false);
                m_radioButton->SetKeyboardInputEnabled(false);
                m_label = new LabelClickable(this);
                m_label->SetAlignment(Position::CenterV | Position::Left);
                m_label->SetText("Radio Button");
                m_label->Dock(Position::Fill);
                m_label->onPress.Add(m_radioButton, &CheckBox::OnPress);
                m_label->SetTabable(false);
                m_label->SetKeyboardInputEnabled(false);
            }

            void RenderFocus(Gwk::Skin::Base* skin) override
            {
                if (Gwk::KeyboardFocus != this)
                    return;

                if (!IsTabable())
                    return;

                skin->DrawKeyboardHighlight(this, GetRenderBounds(), 0);
            }

            virtual RadioButton* GetRadioButton()
            {
                return m_radioButton;
            }

            virtual LabelClickable* GetLabel()
            {
                return m_label;
            }

            bool OnKeySpace(bool bDown) override
            {
                if (bDown)
                    m_radioButton->SetChecked(!m_radioButton->IsChecked());

                return true;
            }

            virtual void Select()
            {
                m_radioButton->SetChecked(true);
            }

        private:

            RadioButton*        m_radioButton;
            LabelClickable*     m_label;
        };


    }
}
#endif // ifndef GWK_CONTROLS_RADIOBUTTON_H
