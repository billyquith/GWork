/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include "Gwork/Test/Test.h"
#include "Gwork/Controls/Properties.h"
#include "Gwork/Controls/PropertyTree.h"
#include "Gwork/Controls/Property/ColorSelector.h"
#include "Gwork/Controls/Property/Checkbox.h"
#include "Gwork/Controls/Property/ComboBox.h"

using namespace Gwk;

class Properties : public GUnit
{
public:

    GWK_CONTROL_INLINE(Properties, GUnit)
    {
        {
            Gwk::Controls::Properties* props = new Gwk::Controls::Properties(this);
            props->SetBounds(10, 10, 150, 300);
            {
                {
                    Gwk::Controls::PropertyRow* pRow = props->Add("First Name");
                    pRow->onChange.Add(this, &Properties::OnFirstNameChanged);
                }
                props->Add("Middle Name");
                props->Add("Last Name");
            }
        }
        {
            Gwk::Controls::PropertyTree* ptree = new Gwk::Controls::PropertyTree(this);
            ptree->SetBounds(200, 10, 200, 200);
            {
                Gwk::Controls::Properties* props = ptree->Add("Item One");
                props->Add("Middle Name");
                props->Add("Last Name");
                props->Add("Four");
            }
            {
                Gwk::Controls::Properties* props = ptree->Add("Item Two");
                props->Add("More Items");
                props->Add("Checkbox", new Gwk::Controls::Property::Checkbox(props), "1");
                props->Add("To Fill");
                props->Add("ColorSelector", new Gwk::Controls::Property::ColorSelector(
                               props), "255 0 0");
                props->Add("Out Here");
                // Combo Box Test
                {
                    Gwk::Controls::Property::ComboBox* pCombo =
                        new Gwk::Controls::Property::ComboBox(props);
                    pCombo->GetComboBox()->AddItem("Option One", "one");
                    pCombo->GetComboBox()->AddItem("Number Two", "two");
                    pCombo->GetComboBox()->AddItem("Door Three", "three");
                    pCombo->GetComboBox()->AddItem("Four Legs", "four");
                    pCombo->GetComboBox()->AddItem("Five Birds", "five");
                    Gwk::Controls::PropertyRow* pRow = props->Add("ComboBox", pCombo, "1");
                    pRow->onChange.Add(this, &Properties::OnFirstNameChanged);
                }
            }
            ptree->ExpandAll();
        }
    }

    void OnFirstNameChanged(Controls::Base* pControl)
    {
        Gwk::Controls::PropertyRow* pRow = (Gwk::Controls::PropertyRow*)pControl;
        UnitPrint(Utility::Format("First Name Changed: %s",
                                  pRow->GetProperty()->GetPropertyValue().c_str()));
    }

};


DEFINE_UNIT_TEST(Properties);
