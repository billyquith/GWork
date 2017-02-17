/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/TreeControl.h>

using namespace Gwk;

class TreeControl : public TestUnit
{
public:

    GWK_CONTROL_INLINE(TreeControl, TestUnit)
    {
        {
            Gwk::Controls::TreeControl* ctrl = new Gwk::Controls::TreeControl(this);
            ctrl->AddNode("Node One");
            Gwk::Controls::TreeNode* node = ctrl->AddNode("Node Two");
            node->AddNode("Node Two Inside");
            node->AddNode("Eyes");
            node->AddNode("Brown")
                ->AddNode("Node Two Inside")
                ->AddNode("Eyes")
                ->AddNode("Brown");
            node->AddNode("More");
            node->AddNode("Nodes");
            ctrl->AddNode("Node Three");
            ctrl->SetBounds(30, 30, 200, 200);
            ctrl->ExpandAll();
        }
        {
            Gwk::Controls::TreeControl* ctrl = new Gwk::Controls::TreeControl(this);
            ctrl->AllowMultiSelect(true);
            ctrl->AddNode("Node One");
            Gwk::Controls::TreeNode* node = ctrl->AddNode("Node Two");
            node->AddNode("Node Two Inside");
            node->AddNode("Eyes");
            Gwk::Controls::TreeNode* nodeTwo = node->AddNode("Brown")
                ->AddNode("Node Two Inside")
                ->AddNode("Eyes");
            nodeTwo->AddNode("Brown");
            nodeTwo->AddNode("Green");
            nodeTwo->AddNode("Slime");
            nodeTwo->AddNode("Grass");
            nodeTwo->AddNode("Pipe");
            node->AddNode("More");
            node->AddNode("Nodes");
            ctrl->AddNode("Node Three");
            ctrl->SetBounds(240, 30, 200, 200);
            ctrl->ExpandAll();
        }
    }

};


DECLARE_TEST(TreeControl);
