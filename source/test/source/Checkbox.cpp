/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/Test.h>
#include <Gwork/Controls/CheckBox.h>

using namespace Gwk;

class Checkbox : public GUnit
{
public:

    GWK_CONTROL_INLINE(Checkbox, GUnit)
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
            Gwk::Controls::CheckBox* check = new Gwk::Controls::CheckBox(this);
            check->SetPos(10, 54);
            check->SetDisabled(true);
        }
    }

    void OnChecked(Controls::Base* control)
    {
        UnitPrint("Checkbox Checked (using 'OnChecked' event)");
    }

    void OnUnchecked(Controls::Base* control)
    {
        UnitPrint("Checkbox Unchecked (using 'OnUnchecked' event)");
    }

    void OnCheckChanged(Controls::Base* control)
    {
        UnitPrint("Checkbox CheckChanged (using 'OnCheckChanged' event)");
    }

};


DEFINE_UNIT_TEST(Checkbox);
