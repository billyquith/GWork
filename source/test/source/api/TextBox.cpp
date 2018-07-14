/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/TextBox.h>

namespace Gwk {
namespace Test {

class TextBox : public TestUnit
{
public:

    GWK_CONTROL_INLINE(TextBox, TestUnit)
    {
        {
            Gwk::Controls::TextBox* label = new Gwk::Controls::TextBox(this);
            label->SetText("");
            label->SetPos(10, 10);
            label->onTextChanged.Add(this, &ThisClass::OnEdit);
            label->onReturnPressed.Add(this, &ThisClass::OnSubmit);
        }
        {
            Gwk::Controls::TextBox* label = new Gwk::Controls::TextBox(this);
            label->SetText("Normal Everyday Label");
            label->SetPos(10, 10+25);
        }
        {
            Gwk::Controls::TextBox* label = new Gwk::Controls::TextBox(this);
            label->SetText("Select All Text On Focus");
            label->SetPos(10, 10+25*2);
            label->SetSelectAllOnFocus(true);
        }
        {
            Gwk::Controls::TextBox* label = new Gwk::Controls::TextBox(this);
            label->SetText("Different Colored Text, for some reason");
            label->SetTextColor(Gwk::Color(255, 0, 255, 255));
            label->SetPos(10, 10+25*3);
        }
        {
            Gwk::Controls::TextBox* label = new Gwk::Controls::TextBox(this);
            label->SetText("Max Length 15");
            label->SetMaxTextLength(15);
            label->SetPos(10, 10+25*4);
        }
        {
            Gwk::Controls::TextBoxNumeric* label = new Gwk::Controls::TextBoxNumeric(this);
            label->SetText("2004");
            label->SetTextColor(Gwk::Color(255, 0, 255, 255));
            label->SetPos(10, 10+25*5);
        }
        {
            Gwk::Controls::TextBoxNumeric* label = new Gwk::Controls::TextBoxNumeric(this);
            label->SetText("3.14159265");
            label->SetTextColor(Gwk::Color(255, 0, 255, 255));
            label->SetPos(10, 10+25*6);
            label->SetMaxTextLength(10);
        }
        {
            m_font.facename = "Impact";
            m_font.size = 50;
            Gwk::Controls::TextBox* label = new Gwk::Controls::TextBox(this);
            label->SetText("Different Font");
            label->SetPos(10, 10+25*7);
            label->SetFont(m_font);
            label->SetSize(200, 55);
        }
        {
            Gwk::Controls::TextBoxMultiline* label = new Gwk::Controls::TextBoxMultiline(this);
            label->SetText(
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ultrices pharetra "
            "scelerisque. Ut posuere velit ligula suscipit ut lobortis ligula congue."
            "Aliquam lacinia cursus est, quis aliquam nisl scelerisque vitae. Nunc porta eros "
            "sem, nec rhoncus eros. Integer elementum, quam vitae egestas dictum, mi quam gravida"
            " augue, non fringilla lacus nisi sit amet nunc. Maecenas dolor tellus, consequat sed "
            "sodales ut, aliquam ac enim. Nulla facilisi. Maecenas eleifend, velit a lobortis "
            "vehicula, nunc lacus egestas leo, volutpat egestas augue.");
            label->SetPos(220, 10);
            label->SetSize(200, 180);
        }
        {
            Gwk::Controls::TextBoxMultiline* label = new Gwk::Controls::TextBoxMultiline(this);
            label->SetText("Max Length 20");
            label->SetMaxTextLength(20);
            label->SetText("Max Length 20 -- Set text longer than max length");
            label->SetPos(220, 195);
            label->SetSize(200, 45);
        }
    }

    void OnEdit(Event::Info info)
    {
        auto textbox = static_cast<Gwk::Controls::TextBox*>(info.ControlCaller);
        OutputToLog(Utility::Format("Textbox Edit: [%s]\n",
                                    textbox->GetText().c_str()));
    }

    void OnSubmit(Event::Info info)
    {
        auto textbox = static_cast<Gwk::Controls::TextBox*>(info.ControlCaller);
        OutputToLog(Utility::Format("Textbox Submit: [%s]\n",
                                    textbox->GetText().c_str()));
    }

    Gwk::Font m_font;
};

GWK_REGISTER_TEST(TextBox);

}}

