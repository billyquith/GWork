#include "Gwen/UnitTest/UnitTest.h"
#include "Gwen/Controls/ListBox.h"

using namespace Gwen;

class ListBox : public GUnit
{
public:

    GWEN_CONTROL_INLINE(ListBox, GUnit)
    {
        {
            Gwen::Controls::ListBox* ctrl = new Gwen::Controls::ListBox(this);
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
            Gwen::Controls::ListBox* ctrl = new Gwen::Controls::ListBox(this);
            ctrl->SetBounds(120, 10, 200, 200);
            ctrl->SetColumnCount(3);
            ctrl->SetAllowMultiSelect(true);
            ctrl->onRowSelected.Add(this, &ThisClass::RowSelected);
            {
                Gwen::Controls::Layout::TableRow* pRow = ctrl->AddItem("Baked Beans");
                pRow->SetCellText(1, "Heinz");
                pRow->SetCellText(2, "£3.50");
            }
            {
                Gwen::Controls::Layout::TableRow* pRow = ctrl->AddItem("Bananas");
                pRow->SetCellText(1, "Trees");
                pRow->SetCellText(2, "$1.27");
            }
            {
                Gwen::Controls::Layout::TableRow* pRow = ctrl->AddItem("Chicken");
                pRow->SetCellText(1, Gwen::Utility::Narrow(L"\u5355\u5143\u6D4B\u8BD5"));
                pRow->SetCellText(2, Gwen::Utility::Narrow(L"\u20AC8.95"));
            }
        }
    }


    void RowSelected(Gwen::Controls::Base* pControl)
    {
        Gwen::Controls::ListBox* ctrl = (Gwen::Controls::ListBox*)pControl;
        UnitPrint(Utility::Format("Listbox Item Selected: %s",
                                  ctrl->GetSelectedRow()->GetText(0).c_str()));
    }

    Gwen::Font m_Font;
};


DEFINE_UNIT_TEST(ListBox, "ListBox");
