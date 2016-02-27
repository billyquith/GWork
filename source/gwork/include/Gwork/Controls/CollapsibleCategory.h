/*
 *  GWEN
 *  Copyright (c) 2010 Facepunch Studios
 *  See license in Gwen.h
 */

#pragma once
#ifndef GWEN_CONTROLS_COLLAPSIBLECATEGORY_H
#define GWEN_CONTROLS_COLLAPSIBLECATEGORY_H

#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/Button.h"
#include "Gwen/Gwen.h"
#include "Gwen/Skin.h"


namespace Gwen
{
    namespace Controls
    {
        class CollapsibleList;

        class GWEN_EXPORT CollapsibleCategory : public Gwen::Controls::Base
        {
        public:

            GWEN_CONTROL(CollapsibleCategory, Gwen::Controls::Base);

            virtual void Render(Skin::Base* skin) override;

            virtual void SetText(const String& text);

            virtual Button* Add(const String& name);

            virtual void PostLayout(Skin::Base* /*skin*/) override;

            virtual void SetList(Controls::CollapsibleList* p)
            {
                m_pList = p;
            }

            virtual void    UnselectAll();
            virtual Button* GetSelected();
            
            /// Set the expanded state of the category.
            /// @param expanded : New state.
            void SetExpanded(bool expanded);
            /// Get the current expanded state of the category.
            /// @return Is expanded?
            bool IsExpanded() const;

        public:

            Gwen::Event::Caller onSelection;

        protected:

            virtual void OnSelection(Controls::Base* control);

            Controls::Button*           m_pButton;
            Controls::CollapsibleList*  m_pList;
        };


    }
}
#endif // ifndef GWEN_CONTROLS_COLLAPSIBLECATEGORY_H
