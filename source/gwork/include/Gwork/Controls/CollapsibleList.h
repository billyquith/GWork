/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_COLLAPSIBLELIST_H
#define GWK_CONTROLS_COLLAPSIBLELIST_H

#include "Gwork/Controls/Base.h"
#include "Gwork/Controls/Button.h"
#include "Gwork/Controls/ScrollControl.h"
#include "Gwork/Gwork.h"
#include "Gwork/Skin.h"
#include "Gwork/Controls/CollapsibleCategory.h"


namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT CollapsibleList : public Gwk::Controls::ScrollControl
        {
        public:

            Gwk::Event::Caller onSelection;

        public:

            GWK_CONTROL_INLINE(CollapsibleList, Gwk::Controls::ScrollControl)
            {
                SetScroll(false, true);
                SetAutoHideBars(true);
            }

            virtual void Add(Gwk::Controls::CollapsibleCategory* pCategory)
            {
                pCategory->SetParent(this);
                pCategory->Dock(Docking::Top);
                pCategory->SetMargin(Margin(1, 0, 1, 1));
                pCategory->SetList(this);
                pCategory->onSelection.Add(this, &ThisClass::OnSelectionEvent);
            }

            virtual Gwk::Controls::CollapsibleCategory* Add(const String& name)
            {
                Gwk::Controls::CollapsibleCategory* pCategory = new CollapsibleCategory(this);
                pCategory->SetText(name);
                Add(pCategory);
                return pCategory;
            }

            virtual void Render(Skin::Base* skin) override
            {
                skin->DrawCategoryHolder(this);
            }

            virtual void UnselectAll()
            {
                Base::List& children = GetChildren();

                for (Base::List::iterator iter = children.begin(); iter != children.end(); ++iter)
                {
                    Gwk::Controls::CollapsibleCategory* pChild =
                        gwk_cast<Gwk::Controls::CollapsibleCategory>(*iter);

                    if (!pChild)
                        continue;

                    pChild->UnselectAll();
                }
            }

            virtual Gwk::Controls::Button* GetSelected()
            {
                Base::List& children = GetChildren();

                for (Base::List::iterator iter = children.begin(); iter != children.end(); ++iter)
                {
                    Gwk::Controls::CollapsibleCategory* pChild =
                        gwk_cast<Gwk::Controls::CollapsibleCategory>(*iter);

                    if (!pChild)
                        continue;

                    Gwk::Controls::Button* pSelected = pChild->GetSelected();

                    if (pSelected)
                        return pSelected;
                }

                return NULL;
            }

        protected:

            virtual void OnSelection(Gwk::Controls::CollapsibleCategory* pControl,
                                     Gwk::Controls::Button* pSelected)
            {
                onSelection.Call(this);
            }

            void OnSelectionEvent(Controls::Base* pControl)
            {
                Gwk::Controls::CollapsibleCategory* pChild =
                    gwk_cast<Gwk::Controls::CollapsibleCategory>(pControl);

                if (!pChild)
                    return;

                OnSelection(pChild, pChild->GetSelected());
            }

        };


    }
}
#endif // ifndef GWK_CONTROLS_COLLAPSIBLELIST_H
