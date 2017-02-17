/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/CheckBox.h>

using namespace Gwk;

class Checkbox : public TestUnit
{
public:

    GWK_CONTROL_INLINE(Checkbox, TestUnit)
    {
        Gwk::Controls::CheckBox* check = new Gwk::Controls::CheckBox(this);
        check->SetPos(10, 10);
        check->onChecked.Add(this, &Checkbox::OnChecked);
        check->onUnChecked.Add(this, &Checkbox::OnUnchecked);
        check->onCheckChanged.Add(this, &Checkbox::OnCheckChanged);
        Gwk::Controls::CheckBoxWithLabel* labeled = new Gwk::Controls::CheckBoxWithLabel(this);
        labeled->SetPos(10, 30);
        labeled->Label()->SetText("Labeled CheckBox");
        labeled->Checkbox()->onChecked.Add(this, &Checkbox::OnChecked);
        labeled->Checkbox()->onUnChecked.Add(this, &Checkbox::OnUnchecked);
        labeled->Checkbox()->onCheckChanged.Add(this, &Checkbox::OnCheckChanged);
        {
            Gwk::Controls::CheckBox* check2 = new Gwk::Controls::CheckBox(this);
            check2->SetPos(10, 54);
            check2->SetDisabled(true);
        }
    }

    void OnChecked(Event::Info)
    {
        OutputToLog("Checkbox Checked (using 'OnChecked' event)");
    }

    void OnUnchecked(Event::Info)
    {
        OutputToLog("Checkbox Unchecked (using 'OnUnchecked' event)");
    }

    void OnCheckChanged(Event::Info)
    {
        OutputToLog("Checkbox CheckChanged (using 'OnCheckChanged' event)");
    }

};


DECLARE_TEST(Checkbox);
