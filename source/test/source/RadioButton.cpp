/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/Test.h>
#include <Gwork/Controls/RadioButtonController.h>

using namespace Gwk;

class RadioButton : public GUnit
{
public:

    GWK_CONTROL_INLINE(RadioButton, GUnit)
    {
        Gwk::Controls::RadioButtonController* rc = new Gwk::Controls::RadioButtonController(this);
        rc->AddOption("Option 1");
        rc->AddOption("Option 2");
        rc->AddOption("Option 3");
        rc->AddOption(Utility::Narrow(L"\u0627\u0644\u0622\u0646 \u0644\u062D\u0636\u0648\u0631"));
        rc->SetBounds(30, 30, 200, 200);
        rc->onSelectionChange.Add(this, &RadioButton::OnChange);
    }

    void OnChange(Controls::Base* pControl)
    {
        Gwk::Controls::RadioButtonController* rc =
            (Gwk::Controls::RadioButtonController*)pControl;
        Gwk::Controls::LabeledRadioButton* pSelected = rc->GetSelected();
        UnitPrint(Utility::Format(
                      "RadioButton changed (using 'OnChange' event)\n Chosen Item: '%s'",
                      pSelected->GetLabel()->GetText().c_str()));
    }

};


DEFINE_UNIT_TEST(RadioButton);
