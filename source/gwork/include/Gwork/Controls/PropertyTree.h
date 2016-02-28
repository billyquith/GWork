/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_PROPERTYTREE_H
#define GWK_CONTROLS_PROPERTYTREE_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/TreeControl.h>
#include <Gwork/Controls/Properties.h>


namespace Gwk
{
    namespace Controls
    {
        class PropertyTreeNode : public TreeNode
        {
        public:

            GWK_CONTROL_INLINE(PropertyTreeNode, TreeNode)
            {
                m_Title->SetTextColorOverride(GetSkin()->Colors.Properties.Title);
            }

            virtual void Render(Skin::Base* skin) override
            {
                skin->DrawPropertyTreeNode(this, m_InnerPanel->X(), m_InnerPanel->Y());
            }

        };


        class PropertyTree : public TreeControl
        {
        public:

            GWK_CONTROL_INLINE(PropertyTree, TreeControl)
            {
            }

            Properties* Add(const String& text);
            Properties* Find(const String& text);
        };


    }
}
#endif // ifndef GWK_CONTROLS_PROPERTYTREE_H
