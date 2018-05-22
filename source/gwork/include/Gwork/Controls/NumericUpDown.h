/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_NUMERICUPDOWN_H
#define GWK_CONTROLS_NUMERICUPDOWN_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Controls/TextBox.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT NumericUpDownButton_Up : public Button
        {
            GWK_CONTROL_INLINE(NumericUpDownButton_Up, Button)
            {
                SetSize(7, 7);
            }

            void Render(Skin::Base* skin) override
            {
                skin->DrawNumericUpDownButton(this, IsDepressed(), true);
            }

        };


        class GWK_EXPORT NumericUpDownButton_Down : public Button
        {
            GWK_CONTROL_INLINE(NumericUpDownButton_Down, Button)
            {
                SetSize(7, 7);
            }

            void Render(Skin::Base* skin) override
            {
                skin->DrawNumericUpDownButton(this, IsDepressed(), false);
            }

        };


        class GWK_EXPORT NumericUpDown : public TextBoxNumeric
        {
        public:

            GWK_CONTROL(NumericUpDown, TextBoxNumeric);

            virtual void SetMin(int i);
            virtual void SetMax(int i);
            virtual void SetIntValue(int i);
            virtual int GetIntValue();

            Event::Caller onChanged;

        private:

            void OnEnter() override;
            virtual void OnChange();

            virtual void OnButtonUp(Base* control);
            virtual void OnButtonDown(Base* control);

            bool OnKeyUp(bool bDown) override
            {
                if (bDown)
                    OnButtonUp(nullptr);

                return true;
            }

            bool OnKeyDown(bool bDown) override
            {
                if (bDown)
                    OnButtonDown(nullptr);

                return true;
            }

            virtual int GetIntValueUnclamped();

            int m_max;
            int m_min;
            int m_lastNumber;
        };


    }
}
#endif // ifndef GWK_CONTROLS_NUMERICUPDOWN_H
