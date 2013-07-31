#include "Gwen/UnitTest/UnitTest.h"
#include "Gwen/Controls/TextBox.h"

using namespace Gwen;

class TextBox : public GUnit
{
public:

    GWEN_CONTROL_INLINE(TextBox, GUnit)
    {
        {
            Gwen::Controls::TextBox* label = new Gwen::Controls::TextBox(this);
            label->SetText("");
            label->SetPos(10, 10);
            label->onTextChanged.Add(this, &ThisClass::OnEdit);
            label->onReturnPressed.Add(this, &ThisClass::OnSubmit);
        }
        {
            Gwen::Controls::TextBox* label = new Gwen::Controls::TextBox(this);
            label->SetText("Normal Everyday Label");
            label->SetPos(10, 10+25);
        }
        {
            Gwen::Controls::TextBox* label = new Gwen::Controls::TextBox(this);
            label->SetText("Select All Text On Focus");
            label->SetPos(10, 10+25*2);
            label->SetSelectAllOnFocus(true);
        }
        {
            Gwen::Controls::TextBox* label = new Gwen::Controls::TextBox(this);
            label->SetText("Different Coloured Text, for some reason");
            label->SetTextColor(Gwen::Color(255, 0, 255, 255));
            label->SetPos(10, 10+25*3);
        }
        {
            Gwen::Controls::TextBoxNumeric* label = new Gwen::Controls::TextBoxNumeric(this);
            label->SetText("2004");
            label->SetTextColor(Gwen::Color(255, 0, 255, 255));
            label->SetPos(10, 10+25*4);
        }
        {
            m_Font.facename = "Impact";
            m_Font.size = 50;
            Gwen::Controls::TextBox* label = new Gwen::Controls::TextBox(this);
            label->SetText("Different Font");
            label->SetPos(10, 10+25*5);
            label->SetFont(&m_Font);
            label->SetSize(200, 55);
        }
        {
            Gwen::Controls::TextBoxMultiline* label = new Gwen::Controls::TextBoxMultiline(this);
            label->SetText(
                "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ultrices pharetra scelerisque. Ut posuere velit ligula suscipit ut lobortis ligula congue. Aliquam lacinia cursus est, quis aliquam nisl scelerisque vitae. Nunc porta eros sem, nec rhoncus eros. Integer elementum, quam vitae egestas dictum, mi quam gravida augue, non fringilla lacus nisi sit amet nunc. Maecenas dolor tellus, consequat sed sodales ut, aliquam ac enim. Nulla facilisi. Maecenas eleifend, velit a lobortis vehicula, nunc lacus egestas leo, volutpat egestas augue.");
            label->SetPos(220, 10);
            label->SetSize(200, 180);
        }
    }

    void OnEdit(Gwen::Controls::Base* pControl)
    {
        Gwen::Controls::TextBox* textbox = (Gwen::Controls::TextBox*)(pControl);
        UnitPrint(Utility::Format("Textbox Edit: [%s]\n",
                                  textbox->GetText().c_str()));
    }

    void OnSubmit(Gwen::Controls::Base* pControl)
    {
        Gwen::Controls::TextBox* textbox = (Gwen::Controls::TextBox*)(pControl);
        UnitPrint(Utility::Format("Textbox Submit: [%s]\n",
                                  textbox->GetText().c_str()));
    }

    Gwen::Font m_Font;
};


DEFINE_UNIT_TEST(TextBox, "TextBox");
