#include "Gwen/UnitTest/UnitTest.h"
#include "Gwen/Controls/TreeControl.h"

using namespace Gwen;

class TreeControl : public GUnit
{
public:

    GWEN_CONTROL_INLINE(TreeControl, GUnit)
    {
        {
            Gwen::Controls::TreeControl* ctrl = new Gwen::Controls::TreeControl(this);
            ctrl->AddNode("Node One");
            Gwen::Controls::TreeNode* pNode = ctrl->AddNode("Node Two");
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
            Gwen::Controls::TreeControl* ctrl = new Gwen::Controls::TreeControl(this);
            ctrl->AllowMultiSelect(true);
            ctrl->AddNode("Node One");
            Gwen::Controls::TreeNode* pNode = ctrl->AddNode("Node Two");
            pNode->AddNode("Node Two Inside");
            pNode->AddNode("Eyes");
            Gwen::Controls::TreeNode* pNodeTwo = pNode->AddNode("Brown")
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


DEFINE_UNIT_TEST(TreeControl, "TreeControl");
