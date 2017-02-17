/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/ListBox.h>

using namespace Gwk;

class ListBox : public TestUnit
{
public:

    GWK_CONTROL_INLINE(ListBox, TestUnit)
    {
        {
            Gwk::Controls::ListBox* ctrl = new Gwk::Controls::ListBox(this);
            ctrl->SetBounds(10, 10, 100, 200);
            ctrl->AddItem("First");
            ctrl->AddItem("Blue");
            ctrl->AddItem("Yellow");
            ctrl->AddItem("Orange");
            ctrl->AddItem("Brown");
            ctrl->AddItem("Black");
            ctrl->AddItem("Green");
            ctrl->AddItem("Dog");
            ctrl->AddItem("Cat Blue");
            ctrl->AddItem("Shoes");
            ctrl->AddItem("Shirts");
            ctrl->AddItem("Chair");
            ctrl->AddItem("Last");
            ctrl->SelectByString("Bl*", true);
            ctrl->SetAllowMultiSelect(true);
            ctrl->SetKeyboardInputEnabled(true);
            ctrl->onRowSelected.Add(this, &ThisClass::RowSelected);
        }
        {
            Gwk::Controls::ListBox* ctrl = new Gwk::Controls::ListBox(this);
            ctrl->SetBounds(120, 10, 200, 200);
            ctrl->SetColumnCount(3);
            ctrl->SetAllowMultiSelect(true);
            ctrl->onRowSelected.Add(this, &ThisClass::RowSelected);
            {
                Gwk::Controls::Layout::TableRow* row = ctrl->AddItem("Baked Beans");
                row->SetCellText(1, "Heinz");
                row->SetCellText(2, "£3.50");
            }
            {
                Gwk::Controls::Layout::TableRow* row = ctrl->AddItem("Bananas");
                row->SetCellText(1, "Trees");
                row->SetCellText(2, "$1.27");
            }
            {
                Gwk::Controls::Layout::TableRow* row = ctrl->AddItem("Chicken");
                row->SetCellText(1, Gwk::Utility::Narrow(L"\u5355\u5143\u6D4B\u8BD5"));
                row->SetCellText(2, Gwk::Utility::Narrow(L"\u20AC8.95"));
            }
        }
    }


    void RowSelected(Event::Info info)
    {
        Gwk::Controls::ListBox* ctrl = static_cast<Gwk::Controls::ListBox*>(info.ControlCaller);
        OutputToLog(Utility::Format("Listbox Item Selected: %s",
                                    ctrl->GetSelectedRow()->GetText(0).c_str()));
    }

    Gwk::Font m_font;
};


DECLARE_TEST(ListBox);
