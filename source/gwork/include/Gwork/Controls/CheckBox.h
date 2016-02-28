/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_CHECKBOX_H
#define GWK_CONTROLS_CHECKBOX_H

#include "Gwork/Controls/Base.h"
#include "Gwork/Controls/Button.h"
#include "Gwork/Gwork.h"
#include "Gwork/Skin.h"
#include "Gwork/Controls/LabelClickable.h"

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT CheckBox : public Button
        {
        public:

            GWK_CONTROL(CheckBox, Button);

            virtual void Render(Skin::Base* skin) override;
            virtual void OnPress() override;

            virtual void SetChecked(bool Checked);
            virtual void Toggle() override
            {
                SetChecked(!IsChecked());
            }

            virtual bool IsChecked()
            {
                return m_bChecked;
            }

            Gwk::Event::Caller onChecked;
            Gwk::Event::Caller onUnChecked;
            Gwk::Event::Caller onCheckChanged;

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
                m_Checkbox = new CheckBox(this);
                m_Checkbox->Dock(Docking::Left);
                m_Checkbox->SetMargin(Margin(0, 2, 2, 2));
                m_Checkbox->SetTabable(false);
                m_Label = new LabelClickable(this);
                m_Label->Dock(Docking::Fill);
                m_Label->onPress.Add(m_Checkbox, &CheckBox::OnPress);
                m_Label->SetTabable(false);
                SetTabable(false);
            }

            virtual CheckBox* Checkbox()
            {
                return m_Checkbox;
            }

            virtual LabelClickable* Label()
            {
                return m_Label;
            }

            virtual bool OnKeySpace(bool bDown) override
            {
                if (!bDown)
                    m_Checkbox->SetChecked(!m_Checkbox->IsChecked());

                return true;
            }

        private:

            CheckBox*       m_Checkbox;
            LabelClickable* m_Label;
        };


    }
}
#endif // ifndef GWK_CONTROLS_CHECKBOX_H
