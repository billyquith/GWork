/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
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

            Event::Listener onChanged;

        private:

            void OnEnter() override;
            virtual void OnChange();
            void OnTextChanged() override;

            virtual void OnButtonUp(Event::Info);
            virtual void OnButtonDown(Event::Info);

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

            virtual void SyncTextFromNumber();
            virtual void SyncNumberFromText();


            int m_number;
            int m_max;
            int m_min;

        };


    }
}
#endif // ifndef GWK_CONTROLS_NUMERICUPDOWN_H
