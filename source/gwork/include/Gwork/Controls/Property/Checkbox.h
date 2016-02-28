/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
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
                    m_Checkbox = new Gwk::Controls::CheckBox(this);
                    m_Checkbox->SetShouldDrawBackground(false);
                    m_Checkbox->onCheckChanged.Add(this, &ParentClass::OnPropertyValueChanged);
                    m_Checkbox->SetTabable(true);
                    m_Checkbox->SetKeyboardInputEnabled(true);
                    m_Checkbox->SetPos(2, 1);
                    SetHeight(18);
                }

                virtual String GetPropertyValue() override
                {
                    return m_Checkbox->IsChecked() ? "1" : "0";
                }

                virtual void SetPropertyValue(const String& v, bool bFireChangeEvents) override
                {
                    if (v == "1" || v == "true" || v == "TRUE" || v == "yes" || v == "YES")
                        return m_Checkbox->SetChecked(true);

                    return m_Checkbox->SetChecked(false);
                }

                virtual bool IsEditing() override
                {
                    return m_Checkbox->HasFocus();
                }

                virtual bool IsHovered() override
                {
                    return ParentClass::IsHovered() || m_Checkbox->IsHovered();
                }

                Gwk::Controls::CheckBox* m_Checkbox;
            };


        }
    }
}
#endif // ifndef GWK_CONTROLS_PROPERTY_CHECKBOX_H
