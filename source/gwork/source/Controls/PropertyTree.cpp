/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include "Gwork/Gwork.h"
#include "Gwork/Controls/PropertyTree.h"
#include "Gwork/Skin.h"

namespace Gwk
{
    namespace Controls
    {
        Properties* PropertyTree::Add(const String& text)
        {
            TreeNode* node = new PropertyTreeNode(this);
            node->SetText(text);
            node->Dock(Docking::Top);
            Properties* props = new Properties(node);
            props->Dock(Docking::Top);
            return props;
        }

        Properties* PropertyTree::Find(const String& text)
        {
            Controls::Base::List& children = GetChildNodes();

            for (Base::List::iterator iter = children.begin(); iter != children.end(); ++iter)
            {
                PropertyTreeNode* pChild = gwk_cast<PropertyTreeNode>(*iter);

                if (!pChild)
                    continue;

                if (pChild->GetText() == text)
                {
                    Base::List& nodechildren = pChild->GetChildren();

                    for (Base::List::iterator iter = nodechildren.begin();
                         iter != nodechildren.end();
                         ++iter)
                    {
                        Properties* pPropertyChild = gwk_cast<Properties>(*iter);

                        if (!pPropertyChild)
                            continue;

                        return pPropertyChild;
                    }
                }
            }

            return NULL;
        }

    }
}
