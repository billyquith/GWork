/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_FIELDLABEL_H
#define GWK_CONTROLS_FIELDLABEL_H

#include "Gwork/BaseRender.h"
#include "Gwork/Controls/Label.h"

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT FieldLabel : public Controls::Label
        {
        public:

            static inline FieldLabel* Setup(Controls::Base* pControl, const Gwk::String& text)
            {
                FieldLabel* plbl = new FieldLabel(pControl->GetParent());
                plbl->SetText(text);
                plbl->SetSize(pControl->Width(), pControl->Height());
                plbl->Dock(pControl->GetDock());
                plbl->SetField(pControl);
                return plbl;
            }

        public:

            GWK_CONTROL_INLINE(FieldLabel, Controls::Label)
            {
                m_pField = NULL;
                SetMargin(Margin(0, 1, 0, 1));
                SetAlignment(Docking::CenterV|Docking::Left);
            }

            void SetField(Controls::Base* pField)
            {
                pField->SetParent(this);
                pField->Dock(Docking::Right);
                m_pField = pField;
            }

            void Layout(Gwk::Skin::Base* pskin) override
            {
                m_pField->SetWidth(Width()-70);
                ParentClass::Layout(pskin);
            }

        protected:

            Controls::Base* m_pField;

        };


    }
}

#endif // ifndef GWK_CONTROLS_FIELDLABEL_H
