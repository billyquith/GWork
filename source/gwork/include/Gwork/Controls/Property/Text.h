/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
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
                    m_TextBox = new TextBox(this);
                    m_TextBox->Dock(Docking::Fill);
                    m_TextBox->SetShouldDrawBackground(false);
                    m_TextBox->onTextChanged.Add(this, &ParentClass::OnPropertyValueChanged);
                }

                virtual String GetPropertyValue() override
                {
                    return m_TextBox->GetText();
                }

                virtual void SetPropertyValue(const String& v, bool bFireChangeEvents) override
                {
                    m_TextBox->SetText(v, bFireChangeEvents);
                }

                virtual bool IsEditing() override
                {
                    return m_TextBox->HasFocus();
                }

                virtual bool IsHovered() override
                {
                    return ParentClass::IsHovered() || m_TextBox->IsHovered();
                }

                TextBox* m_TextBox;
            };


        }
    }
}
#endif // ifndef GWK_CONTROLS_PROPERTY_TEXT_H
