/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_PROPERTY_COMBOBOX_H
#define GWK_CONTROLS_PROPERTY_COMBOBOX_H

#include <Gwork/Controls/Property/BaseProperty.h>
#include <Gwork/Controls/ComboBox.h>

namespace Gwk
{
    namespace Controls
    {
        namespace Property
        {
            class GWK_EXPORT ComboBox : public Property::Base
            {
            public:

                GWK_CONTROL_INLINE(ComboBox, Property::Base)
                {
                    m_comboBox = new Gwk::Controls::ComboBox(this);
                    m_comboBox->Dock(Position::Fill);
                    m_comboBox->onSelection.Add(this, &ParentClass::OnPropertyValueChanged);
                    m_comboBox->SetTabable(true);
                    m_comboBox->SetKeyboardInputEnabled(true);
                    m_comboBox->SetShouldDrawBackground(false);
                    SetHeight(18);
                }

                String GetPropertyValue() override
                {
                    Gwk::Controls::Label* control = m_comboBox->GetSelectedItem();

                    if (!control)
                        return "";

                    return control->GetName();
                }

                void SetPropertyValue(const String& val, bool bFireChangeEvents) override
                {
                    m_comboBox->SelectItemByName(val, bFireChangeEvents);
                }

                bool IsEditing() override
                {
                    return m_comboBox->IsFocussed();
                }

                bool IsHovered() const override
                {
                    return ParentClass::IsHovered() || m_comboBox->IsHovered();
                }

                Gwk::Controls::ComboBox* GetComboBox()
                {
                    return m_comboBox;
                }

            protected:

                Gwk::Controls::ComboBox* m_comboBox;
            };


        }
    }
}
#endif // ifndef GWK_CONTROLS_PROPERTY_COMBOBOX_H
