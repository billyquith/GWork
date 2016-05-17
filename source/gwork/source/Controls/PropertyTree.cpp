/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Controls/PropertyTree.h>
#include <Gwork/Skin.h>

namespace Gwk
{
namespace Controls
{
    
Properties* PropertyTree::Add(const String& text)
{
    TreeNode* node = new PropertyTreeNode(this);
    node->SetText(text);
    node->Dock(Position::Top);
    Properties* props = new Properties(node);
    props->Dock(Position::Top);
    return props;
}

Properties* PropertyTree::Find(const String& text)
{
    Controls::Base::List& children = GetChildNodes();

    for (auto&& control : children)
    {
        PropertyTreeNode* child = gwk_cast<PropertyTreeNode>(control);

        if (!child)
            continue;

        if (child->GetText() == text)
        {
            Base::List& nodechildren = child->GetChildren();

            for (auto&& subcontrol : nodechildren)
            {
                Properties* propertyChild = gwk_cast<Properties>(subcontrol);

                if (!propertyChild)
                    continue;

                return propertyChild;
            }
        }
    }

    return nullptr;
}

} // namespace Controls
} // namespace Gwk
