/*
 *  GWEN
 *  Copyright (c) 2010 Facepunch Studios
 *  See license in Gwen.h
 */

#pragma once
#ifndef GWEN_CONTROLS_PROPERTY_CHECKBOX_H
#define GWEN_CONTROLS_PROPERTY_CHECKBOX_H

#include "Gwen/Controls/Property/BaseProperty.h"
#include "Gwen/Controls/CheckBox.h"

namespace Gwen
{
    namespace Controls
    {
        namespace Property
        {
            class GWEN_EXPORT Checkbox : public Property::Base
            {
            public:

                GWEN_CONTROL_INLINE(Checkbox, Property::Base)
                {
                    m_Checkbox = new Gwen::Controls::CheckBox(this);
                    m_Checkbox->SetShouldDrawBackground(false);
                    m_Checkbox->onCheckChanged.Add(this, &BaseClass::OnPropertyValueChanged);
                    m_Checkbox->SetTabable(true);
                    m_Checkbox->SetKeyboardInputEnabled(true);
                    m_Checkbox->SetPos(2, 1);
                    SetHeight(18);
                }

                virtual String GetPropertyValue()
                {
                    return m_Checkbox->IsChecked() ? "1" : "0";
                }

                virtual void SetPropertyValue(const String& v, bool bFireChangeEvents)
                {
                    if (v == "1" || v == "true" || v == "TRUE" || v == "yes" || v == "YES")
                        return m_Checkbox->SetChecked(true);

                    return m_Checkbox->SetChecked(false);
                }

                virtual bool IsEditing()
                {
                    return m_Checkbox->HasFocus();
                }

                virtual bool IsHovered()
                {
                    return BaseClass::IsHovered() || m_Checkbox->IsHovered();
                }

                Gwen::Controls::CheckBox* m_Checkbox;
            };


        }
    }
}
#endif // ifndef GWEN_CONTROLS_PROPERTY_CHECKBOX_H
