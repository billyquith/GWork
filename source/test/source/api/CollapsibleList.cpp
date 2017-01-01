/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/CollapsibleList.h>

using namespace Gwk;

class CollapsibleList : public TestUnit
{
public:

    GWK_CONTROL_INLINE(CollapsibleList, TestUnit)
    {
        Gwk::Controls::CollapsibleList* control = new Gwk::Controls::CollapsibleList(this);
        control->SetSize(100, 200);
        control->SetPos(10, 10);
        {
            Gwk::Controls::CollapsibleCategory* cat = control->Add("Category One");
            cat->Add("Hello");
            cat->Add("Two");
            cat->Add("Three");
            cat->Add("Four");
        }
        {
            Gwk::Controls::CollapsibleCategory* cat = control->Add("Shopping");
            cat->Add("Special");
            cat->Add("Two Noses");
            cat->Add("Orange ears");
            cat->Add("Beer");
            cat->Add("Three Eyes");
            cat->Add("Special");
            cat->Add("Two Noses");
            cat->Add("Orange ears");
            cat->Add("Beer");
            cat->Add("Three Eyes");
            cat->Add("Special");
            cat->Add("Two Noses");
            cat->Add("Orange ears");
            cat->Add("Beer");
            cat->Add("Three Eyes");
        }
        {
            Gwk::Controls::CollapsibleCategory* cat = control->Add("Category One");
            cat->Add("Hello");
            cat->Add("Two");
            cat->Add("Three");
            cat->Add("Four");
        }
    }
};


DECLARE_TEST(CollapsibleList);
