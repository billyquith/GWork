/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_PROPERTY_TEXT_H
#define GWK_CONTROLS_PROPERTY_TEXT_H

#include <Gwork/Controls/Property/BaseProperty.h>
#include <Gwork/Controls/TextBox.h>

namespace Gwk
{
    namespace Controls
    {
        namespace Property
        {
            class GWK_EXPORT Text : public Property::Base
            {
            public:

                GWK_CONTROL_INLINE(Text, Property::Base)
                {
                    m_textBox = new TextBox(this);
                    m_textBox->Dock(Position::Fill);
                    m_textBox->SetShouldDrawBackground(false);
                    m_textBox->onTextChanged.Add(this, &ParentClass::OnPropertyValueChanged);
                }

                String GetPropertyValue() override
                {
                    return m_textBox->GetText();
                }

                void SetPropertyValue(const String& v, bool bFireChangeEvents) override
                {
                    m_textBox->SetText(v, bFireChangeEvents);
                }

                bool IsEditing() override
                {
                    return m_textBox->IsFocussed();
                }

                bool IsHovered() const override
                {
                    return ParentClass::IsHovered() || m_textBox->IsHovered();
                }

                TextBox* m_textBox;
            };


        }
    }
}
#endif // ifndef GWK_CONTROLS_PROPERTY_TEXT_H
