/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_COLLAPSIBLECATEGORY_H
#define GWK_CONTROLS_COLLAPSIBLECATEGORY_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>


namespace Gwk
{
    namespace Controls
    {
        class CollapsibleList;

        class GWK_EXPORT CollapsibleCategory : public Gwk::Controls::Base
        {
        public:

            GWK_CONTROL(CollapsibleCategory, Gwk::Controls::Base);

            void Render(Skin::Base* skin) override;

            virtual void SetText(const String& text);

            virtual Button* Add(const String& name);

            void PostLayout(Skin::Base* /*skin*/) override;

            virtual void SetList(Controls::CollapsibleList* p)
            {
                m_list = p;
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

            Gwk::Event::Listener onSelection;

        protected:

            virtual void OnSelection(Event::Info info);

            Controls::Button*           m_button;
            Controls::CollapsibleList*  m_list;
        };


    }
}
#endif // ifndef GWK_CONTROLS_COLLAPSIBLECATEGORY_H
