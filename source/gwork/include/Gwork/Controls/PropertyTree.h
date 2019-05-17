/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
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
        class PropertyTreeNode : public ControlClass<PropertyTreeNode, TreeNode>
        {
        public:

            PropertyTreeNode(Gwk::Controls::Base *parent, const Gwk::String &name=""):BaseClass(parent, name)
            {
                m_title->SetTextColorOverride(GetSkin()->Colors.Properties.Title);
            }

            void Render(Skin::Base* skin) override
            {
                skin->DrawPropertyTreeNode(this, m_innerPanel->X(), m_innerPanel->Y());
            }

        };


        class PropertyTree : public ControlClass<PropertyTree, TreeControl>
        {
        public:

            PropertyTree(Gwk::Controls::Base *parent, const Gwk::String &name=""):BaseClass(parent, name)
            {
            }

            Properties* Add(const String& text);
            Properties* Find(const String& text);
        };


    }
}
#endif // ifndef GWK_CONTROLS_PROPERTYTREE_H
