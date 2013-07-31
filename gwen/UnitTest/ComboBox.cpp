#include "Gwen/UnitTest/UnitTest.h"
#include "Gwen/Controls/ComboBox.h"

using namespace Gwen;

class ComboBox : public GUnit
{
public:

    GWEN_CONTROL_INLINE(ComboBox, GUnit)
    {
        {
            Gwen::Controls::ComboBox* combo = new Gwen::Controls::ComboBox(this);
            combo->SetPos(50, 50);
            combo->SetWidth(200);
            combo->AddItem("Option One", "one");
            combo->AddItem("Number Two", "two");
            combo->AddItem("Door Three", "three");
            combo->AddItem("Four Legs", "four");
            combo->AddItem("Five Birds", "five");
            combo->onSelection.Add(this, &ComboBox::OnComboSelect);
        }
        {
            // Empty..
            Gwen::Controls::ComboBox* combo = new Gwen::Controls::ComboBox(this);
            combo->SetPos(50, 80);
            combo->SetWidth(200);
        }
        {
            // Empty..
            Gwen::Controls::ComboBox* combo = new Gwen::Controls::ComboBox(this);
            combo->SetPos(50, 110);
            combo->SetWidth(200);

            for (int i = 0; i < 500; i++)
            {
                combo->AddItem("Lots Of Options");
            }
        }
    }

    void OnComboSelect(Gwen::Controls::Base* pControl)
    {
        Gwen::Controls::ComboBox* combo = (Gwen::Controls::ComboBox*)pControl;
        UnitPrint(Utility::Format("Combo Changed: %s",
                                  combo->GetSelectedItem()->GetText().c_str()));
    }

};


DEFINE_UNIT_TEST(ComboBox, "ComboBox");
