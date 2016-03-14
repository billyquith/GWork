/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/RadioButtonController.h>

using namespace Gwk;

class RadioButton : public TestUnit
{
public:

    GWK_CONTROL_INLINE(RadioButton, TestUnit)
    {
        Gwk::Controls::RadioButtonController* rc = new Gwk::Controls::RadioButtonController(this);
        rc->AddOption("Option 1");
        rc->AddOption("Option 2");
        rc->AddOption("Option 3");
        rc->AddOption(Utility::Narrow(L"\u0627\u0644\u0622\u0646 \u0644\u062D\u0636\u0648\u0631"));
        rc->SetBounds(30, 30, 200, 200);
        rc->onSelectionChange.Add(this, &RadioButton::OnChange);
    }

    void OnChange(Event::Info info)
    {
        Gwk::Controls::RadioButtonController* rc =
            static_cast<Gwk::Controls::RadioButtonController*>(info.ControlCaller);
        Gwk::Controls::LabeledRadioButton* selected = rc->GetSelected();
        OutputToLog(Utility::Format(
                      "RadioButton changed (using 'OnChange' event)\n Chosen Item: '%s'",
                      selected->GetLabel()->GetText().c_str()));
    }

};


DECLARE_TEST(RadioButton);
