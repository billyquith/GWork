#include "Gwen/UnitTest/UnitTest.h"
#include "Gwen/Controls/Properties.h"
#include "Gwen/Controls/PropertyTree.h"
#include "Gwen/Controls/Property/ColorSelector.h"
#include "Gwen/Controls/Property/Checkbox.h"
#include "Gwen/Controls/Property/ComboBox.h"

using namespace Gwen;

class Properties : public GUnit
{
public:

    GWEN_CONTROL_INLINE(Properties, GUnit)
    {
        {
            Gwen::Controls::Properties* props = new Gwen::Controls::Properties(this);
            props->SetBounds(10, 10, 150, 300);
            {
                {
                    Gwen::Controls::PropertyRow* pRow = props->Add("First Name");
                    pRow->onChange.Add(this, &Properties::OnFirstNameChanged);
                }
                props->Add("Middle Name");
                props->Add("Last Name");
            }
        }
        {
            Gwen::Controls::PropertyTree* ptree = new Gwen::Controls::PropertyTree(this);
            ptree->SetBounds(200, 10, 200, 200);
            {
                Gwen::Controls::Properties* props = ptree->Add("Item One");
                props->Add("Middle Name");
                props->Add("Last Name");
                props->Add("Four");
            }
            {
                Gwen::Controls::Properties* props = ptree->Add("Item Two");
                props->Add("More Items");
                props->Add("Checkbox", new Gwen::Controls::Property::Checkbox(props), "1");
                props->Add("To Fill");
                props->Add("ColorSelector", new Gwen::Controls::Property::ColorSelector(
                               props), "255 0 0");
                props->Add("Out Here");
                // Combo Box Test
                {
                    Gwen::Controls::Property::ComboBox* pCombo =
                        new Gwen::Controls::Property::ComboBox(props);
                    pCombo->GetComboBox()->AddItem("Option One", "one");
                    pCombo->GetComboBox()->AddItem("Number Two", "two");
                    pCombo->GetComboBox()->AddItem("Door Three", "three");
                    pCombo->GetComboBox()->AddItem("Four Legs", "four");
                    pCombo->GetComboBox()->AddItem("Five Birds", "five");
                    Gwen::Controls::PropertyRow* pRow = props->Add("ComboBox", pCombo, "1");
                    pRow->onChange.Add(this, &Properties::OnFirstNameChanged);
                }
            }
            ptree->ExpandAll();
        }
    }

    void OnFirstNameChanged(Controls::Base* pControl)
    {
        Gwen::Controls::PropertyRow* pRow = (Gwen::Controls::PropertyRow*)pControl;
        UnitPrint(Utility::Format("First Name Changed: %s",
                                  pRow->GetProperty()->GetPropertyValue().c_str()));
    }

};


DEFINE_UNIT_TEST(Properties, "Properties");
