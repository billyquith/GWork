/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
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
            virtual void Render(Skin::Base* skin) override;

        private:

            // From CheckBox
            virtual bool AllowUncheck() override
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
                m_RadioButton = new RadioButton(this);
                m_RadioButton->Dock(Docking::Left);
                m_RadioButton->SetMargin(Margin(0, 2, 2, 2));
                m_RadioButton->SetTabable(false);
                m_RadioButton->SetKeyboardInputEnabled(false);
                m_Label = new LabelClickable(this);
                m_Label->SetAlignment(Docking::CenterV | Docking::Left);
                m_Label->SetText("Radio Button");
                m_Label->Dock(Docking::Fill);
                m_Label->onPress.Add(m_RadioButton, &CheckBox::OnPress);
                m_Label->SetTabable(false);
                m_Label->SetKeyboardInputEnabled(false);
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
                return m_RadioButton;
            }

            virtual LabelClickable* GetLabel()
            {
                return m_Label;
            }

            virtual bool OnKeySpace(bool bDown) override
            {
                if (bDown)
                    m_RadioButton->SetChecked(!m_RadioButton->IsChecked());

                return true;
            }

            virtual void Select()
            {
                m_RadioButton->SetChecked(true);
            }

        private:

            RadioButton*        m_RadioButton;
            LabelClickable*     m_Label;
        };


    }
}
#endif // ifndef GWK_CONTROLS_RADIOBUTTON_H
