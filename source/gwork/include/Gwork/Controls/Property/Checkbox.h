/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_PROPERTY_CHECKBOX_H
#define GWK_CONTROLS_PROPERTY_CHECKBOX_H

#include <Gwork/Controls/Property/BaseProperty.h>
#include <Gwork/Controls/CheckBox.h>

namespace Gwk
{
    namespace Controls
    {
        namespace Property
        {
            class GWK_EXPORT Checkbox : public Property::Base
            {
            public:

                GWK_CONTROL_INLINE(Checkbox, Property::Base)
                {
                    m_checkbox = new Gwk::Controls::CheckBox(this);
                    m_checkbox->SetShouldDrawBackground(false);
                    m_checkbox->onCheckChanged.Add(this, &ParentClass::OnPropertyValueChanged);
                    m_checkbox->SetTabable(true);
                    m_checkbox->SetKeyboardInputEnabled(true);
                    m_checkbox->SetPos(2, 1);
                    SetHeight(18);
                }

                String GetPropertyValue() override
                {
                    return m_checkbox->IsChecked() ? "1" : "0";
                }

                void SetPropertyValue(const String& v, bool bFireChangeEvents) override
                {
                    if (v == "1" || v == "true" || v == "TRUE" || v == "yes" || v == "YES")
                        return m_checkbox->SetChecked(true);

                    return m_checkbox->SetChecked(false);
                }

                bool IsEditing() override
                {
                    return m_checkbox->IsFocussed();
                }

                bool IsHovered() const override
                {
                    return ParentClass::IsHovered() || m_checkbox->IsHovered();
                }

                Gwk::Controls::CheckBox* m_checkbox;
            };


        }
    }
}
#endif // ifndef GWK_CONTROLS_PROPERTY_CHECKBOX_H
