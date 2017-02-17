/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/Properties.h>
#include <Gwork/Controls/PropertyTree.h>
#include <Gwork/Controls/Property/ColorSelector.h>
#include <Gwork/Controls/Property/Checkbox.h>
#include <Gwork/Controls/Property/ComboBox.h>

using namespace Gwk;

class Properties : public TestUnit
{
public:

    GWK_CONTROL_INLINE(Properties, TestUnit)
    {
        {
            Gwk::Controls::Properties* props = new Gwk::Controls::Properties(this);
            props->SetBounds(10, 10, 150, 300);
            {
                {
                    Gwk::Controls::PropertyRow* row = props->Add("First Name");
                    row->onChange.Add(this, &Properties::OnFirstNameChanged);
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
                    Gwk::Controls::Property::ComboBox* combo =
                        new Gwk::Controls::Property::ComboBox(props);
                    combo->GetComboBox()->AddItem("Option One", "one");
                    combo->GetComboBox()->AddItem("Number Two", "two");
                    combo->GetComboBox()->AddItem("Door Three", "three");
                    combo->GetComboBox()->AddItem("Four Legs", "four");
                    combo->GetComboBox()->AddItem("Five Birds", "five");
                    Gwk::Controls::PropertyRow* row = props->Add("ComboBox", combo, "1");
                    row->onChange.Add(this, &Properties::OnFirstNameChanged);
                }
            }
            ptree->ExpandAll();
        }
    }

    void OnFirstNameChanged(Event::Info info)
    {
        auto row = static_cast<Gwk::Controls::PropertyRow*>(info.ControlCaller);
        OutputToLog(Utility::Format("First Name Changed: %s",
                                  row->GetProperty()->GetPropertyValue().c_str()));
    }

};


DECLARE_TEST(Properties);
