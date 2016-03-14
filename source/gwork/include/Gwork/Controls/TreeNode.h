/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_TREENODE_H
#define GWK_CONTROLS_TREENODE_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Controls/ScrollControl.h>


namespace Gwk
{
    namespace Controls
    {
        class TreeControl;

        class GWK_EXPORT TreeNode : public Base
        {
        public:

            GWK_CONTROL(TreeNode, Base);

            virtual TreeNode* AddNode(const String& strLabel);

            virtual void            SetText(const String& text);
            virtual const String&   GetText();
            virtual void            SetImage(const String& text);

            virtual void Open();
            virtual void Close();

            virtual void ExpandAll();

            virtual Button* GetButton();

            virtual void Render(Skin::Base* skin) override;
            virtual void Layout(Skin::Base* skin) override;
            virtual void PostLayout(Skin::Base* skin) override;

            virtual void SetRoot(bool b)
            {
                m_bRoot = b;
            }

            virtual void SetTreeControl(TreeControl* ctrl)
            {
                m_treeControl = ctrl;
            }

            virtual void SetSelectable(bool b)
            {
                m_bSelectable = b;
            }

            virtual bool IsSelected()
            {
                return m_bSelected;
            }

            virtual void SetSelected(bool b, bool FireEvents = true);

            virtual void DeselectAll();

            virtual Controls::Base::List& GetChildNodes();

            Event::Caller onNamePress;
            Event::Caller onRightPress;
            Event::Caller onSelectChange;
            Event::Caller onSelect;
            Event::Caller onUnselect;

            /*protected:*/

            void OnToggleButtonPress(Event::Info);
            void OnDoubleClickName(Event::Info);
            void OnRightPress(Event::Info);
            void OnClickName(Event::Info);

        protected:

            TreeControl*    m_treeControl;
            Button*         m_toggleButton;
            Button*         m_title;

            bool m_bRoot;
            bool m_bSelected;
            bool m_bSelectable;
        };


    }
}
#endif // ifndef GWK_CONTROLS_TREENODE_H
