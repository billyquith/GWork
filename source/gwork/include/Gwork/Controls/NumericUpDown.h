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
        class GWK_EXPORT NumericUpDownButton_Up : public ControlClass<NumericUpDownButton_Up, Button>
        {
        public:
            NumericUpDownButton_Up(Gwk::Controls::Base *parent, const Gwk::String &name=""):BaseClass(parent, name)
            {
                SetSize(7, 7);
            }

            void Render(Skin::Base* skin) override
            {
                skin->DrawNumericUpDownButton(this, IsDepressed(), true);
            }

        };


        class GWK_EXPORT NumericUpDownButton_Down : public ControlClass<NumericUpDownButton_Down, Button>
        {
        public:
            NumericUpDownButton_Down(Gwk::Controls::Base *parent, const Gwk::String &name=""):BaseClass(parent, name)
            {
                SetSize(7, 7);
            }

            void Render(Skin::Base* skin) override
            {
                skin->DrawNumericUpDownButton(this, IsDepressed(), false);
            }

        };


        class GWK_EXPORT NumericUpDown : public ControlClass<NumericUpDown, TextBoxNumeric>
        {
        public:

            NumericUpDown(Gwk::Controls::Base *parent, const Gwk::String &name="");

            virtual void SetMin(int i);
            virtual void SetMax(int i);
            virtual void SetIntValue(int i);
            virtual int GetIntValue();

            Event::Listener onChanged;

        private:

            void OnEnter() override;
            virtual void OnChange();

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

            virtual int GetIntValueUnclamped();

            int m_max;
            int m_min;
            int m_lastNumber;
        };


    }
}
#endif // ifndef GWK_CONTROLS_NUMERICUPDOWN_H
