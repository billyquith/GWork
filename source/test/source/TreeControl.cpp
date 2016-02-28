/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include "Gwork/Test/Test.h"
#include "Gwork/Controls/TreeControl.h"

using namespace Gwk;

class TreeControl : public GUnit
{
public:

    GWK_CONTROL_INLINE(TreeControl, GUnit)
    {
        {
            Gwk::Controls::TreeControl* ctrl = new Gwk::Controls::TreeControl(this);
            ctrl->AddNode("Node One");
            Gwk::Controls::TreeNode* pNode = ctrl->AddNode("Node Two");
            pNode->AddNode("Node Two Inside");
            pNode->AddNode("Eyes");
            pNode->AddNode("Brown")
                ->AddNode("Node Two Inside")
                ->AddNode("Eyes")
                ->AddNode("Brown");
            pNode->AddNode("More");
            pNode->AddNode("Nodes");
            ctrl->AddNode("Node Three");
            ctrl->SetBounds(30, 30, 200, 200);
            ctrl->ExpandAll();
        }
        {
            Gwk::Controls::TreeControl* ctrl = new Gwk::Controls::TreeControl(this);
            ctrl->AllowMultiSelect(true);
            ctrl->AddNode("Node One");
            Gwk::Controls::TreeNode* pNode = ctrl->AddNode("Node Two");
            pNode->AddNode("Node Two Inside");
            pNode->AddNode("Eyes");
            Gwk::Controls::TreeNode* pNodeTwo = pNode->AddNode("Brown")
                ->AddNode("Node Two Inside")
                ->AddNode("Eyes");
            pNodeTwo->AddNode("Brown");
            pNodeTwo->AddNode("Green");
            pNodeTwo->AddNode("Slime");
            pNodeTwo->AddNode("Grass");
            pNodeTwo->AddNode("Pipe");
            pNode->AddNode("More");
            pNode->AddNode("Nodes");
            ctrl->AddNode("Node Three");
            ctrl->SetBounds(240, 30, 200, 200);
            ctrl->ExpandAll();
        }
    }

};


DEFINE_UNIT_TEST(TreeControl);
