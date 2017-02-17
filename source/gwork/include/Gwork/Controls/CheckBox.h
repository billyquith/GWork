/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_CHECKBOX_H
#define GWK_CONTROLS_CHECKBOX_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/LabelClickable.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT CheckBox : public Button
        {
        public:

            GWK_CONTROL(CheckBox, Button);

            void Render(Skin::Base* skin) override;
            void OnPress(Event::Info) override;

            virtual void SetChecked(bool Checked);
            void Toggle() override
            {
                SetChecked(!IsChecked());
            }

            virtual bool IsChecked()
            {
                return m_bChecked;
            }

            Gwk::Event::Listener onChecked;
            Gwk::Event::Listener onUnChecked;
            Gwk::Event::Listener onCheckChanged;

        private:

            // For derived controls
            virtual bool AllowUncheck()
            {
                return true;
            }

            void OnCheckStatusChanged();

            bool m_bChecked;
        };


        class GWK_EXPORT CheckBoxWithLabel : public Base
        {
        public:

            GWK_CONTROL_INLINE(CheckBoxWithLabel, Base)
            {
                SetSize(200, 19);
                m_checkbox = new CheckBox(this);
                m_checkbox->Dock(Position::Left);
                m_checkbox->SetMargin(Margin(0, 2, 2, 2));
                m_checkbox->SetTabable(false);
                m_label = new LabelClickable(this);
                m_label->Dock(Position::Fill);
                m_label->onPress.Add(m_checkbox, &CheckBox::OnPress);
                m_label->SetTabable(false);
                SetTabable(false);
            }

            virtual CheckBox* Checkbox()
            {
                return m_checkbox;
            }

            virtual LabelClickable* Label()
            {
                return m_label;
            }

            bool OnKeySpace(bool bDown) override
            {
                if (!bDown)
                    m_checkbox->SetChecked(!m_checkbox->IsChecked());

                return true;
            }

        private:

            CheckBox*       m_checkbox;
            LabelClickable* m_label;
        };


    }
}
#endif // ifndef GWK_CONTROLS_CHECKBOX_H
