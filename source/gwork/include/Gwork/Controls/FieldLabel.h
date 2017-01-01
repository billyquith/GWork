/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_FIELDLABEL_H
#define GWK_CONTROLS_FIELDLABEL_H

#include <Gwork/BaseRender.h>
#include <Gwork/Controls/Label.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT FieldLabel : public Controls::Label
        {
        public:

            static inline FieldLabel* Setup(Controls::Base* control, const Gwk::String& text)
            {
                FieldLabel* plbl = new FieldLabel(control->GetParent());
                plbl->SetText(text);
                plbl->SetSize(control->Width(), control->Height());
                plbl->Dock(control->GetDock());
                plbl->SetField(control);
                return plbl;
            }

        public:

            GWK_CONTROL_INLINE(FieldLabel, Controls::Label)
            {
                m_field = nullptr;
                SetMargin(Margin(0, 1, 0, 1));
                SetAlignment(Position::CenterV|Position::Left);
            }

            void SetField(Controls::Base* field)
            {
                field->SetParent(this);
                field->Dock(Position::Right);
                m_field = field;
            }

            void Layout(Gwk::Skin::Base* pskin) override
            {
                m_field->SetWidth(Width()-70);
                ParentClass::Layout(pskin);
            }

        protected:

            Controls::Base* m_field;

        };


    }
}

#endif // ifndef GWK_CONTROLS_FIELDLABEL_H
