/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_TREECONTROL_H
#define GWK_CONTROLS_TREECONTROL_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/TreeNode.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT TreeControl : public TreeNode
        {
        public:

            GWK_CONTROL(TreeControl, TreeNode);

            void Render(Skin::Base* skin) override;

            void OnChildBoundsChanged(Gwk::Rect oldChildBounds, Base* child) override;

            ScrollControl* Scroller()
            {
                return m_scrollControl;
            }

            virtual void Clear();

            void Layout(Skin::Base* skin) override;
            void PostLayout(Skin::Base* skin) override;

            virtual void AllowMultiSelect(bool b)
            {
                m_bAllowMultipleSelection = b;
            }

            virtual void OnNodeAdded(TreeNode* node);

        private:

            void OnNodeSelection(Event::Info info);

            ScrollControl*      m_scrollControl;
            bool m_bAllowMultipleSelection;

        };


    }
}
#endif // ifndef GWK_CONTROLS_TREECONTROL_H
