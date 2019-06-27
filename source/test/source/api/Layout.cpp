/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>

#include <Gwork/Controls/WindowControl.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Controls/GroupBox.h>
#include <Gwork/Controls/RadioButton.h>
#include <Gwork/Controls/Layout/Layout.h>

namespace Gwk {
namespace Test {

class Layout: public TestUnit
{
public:

    GWK_CONTROL_INLINE(Layout, TestUnit)
    {
        {
            Controls::Button* button=new Controls::Button(this);
            button->SetText("Layout Window");
            button->onPress.Add(this, &ThisClass::OpenWindow);
            button->SetPos(0, 0);
        }
    }

    void OpenWindow(Event::Info)
    {
        Controls::WindowControl *window=new Controls::WindowControl(GetCanvas());

        window->SetTitle("Layout Window");
        window->SetSize(200+rand()%100, 200+rand()%100);
        window->SetPos(rand()%700, rand()%400);
        window->SetDeleteOnClose(true);

        Controls::GroupBox *verticalGroupBox1=new Controls::GroupBox(window);

        verticalGroupBox1->Dock(Position::Fill);
        verticalGroupBox1->SetName("VerticalGroup");
        verticalGroupBox1->SetText("Veritcal Group");

        Controls::VerticalLayout *verticalLayout1=new Controls::VerticalLayout();

        verticalGroupBox1->SetLayout(verticalLayout1);

        Controls::Label *vb1_label=new Controls::Label(verticalGroupBox1);

        vb1_label->SetName("VerticalLabel");
        vb1_label->SetText("Vertical Label");

        Controls::Button *vb1_button1=new Controls::Button(verticalGroupBox1);

        vb1_button1->SetName("Button1");
        vb1_button1->SetText("Button 1");
        
        Controls::Button *vb1_button2=new Controls::Button(verticalGroupBox1);

        vb1_button2->SetName("Button2");
        vb1_button2->SetText("Button 2");

        Controls::HorizontalLayout *horizontalLayout1=new Controls::HorizontalLayout();

        verticalLayout1->AddLayout(horizontalLayout1);

        Controls::Button *vb1_hbutton1=new Controls::Button(verticalGroupBox1);

        vb1_hbutton1->SetName("HButton1");
        vb1_hbutton1->SetText("HButton 1");
        horizontalLayout1->AddControl(vb1_hbutton1);

        Controls::Button *vb1_hbutton2=new Controls::Button(verticalGroupBox1);

        vb1_hbutton2->SetName("HButton2");
        vb1_hbutton2->SetText("HButton 2");
        horizontalLayout1->AddControl(vb1_hbutton2);

        Controls::Button *vb1_hbutton3=new Controls::Button(verticalGroupBox1);

        vb1_hbutton3->SetName("HButton3");
        vb1_hbutton3->SetText("HButton 3");
        horizontalLayout1->AddControl(vb1_hbutton3);

        Controls::GroupBox *vb1_group=new Controls::GroupBox(verticalGroupBox1);
        
        vb1_group->SetName("RadioGroup");
        vb1_group->SetText("Radio Group");

        Controls::VerticalLayout *vb1_group_loyout=new Controls::VerticalLayout();

        vb1_group->SetLayout(vb1_group_loyout);
 
        Controls::LabeledRadioButton *vb1_option1=new Controls::LabeledRadioButton(vb1_group);

        vb1_option1->SetName("Option1");
        vb1_option1->SetText("Option 1");

        Controls::LabeledRadioButton *vb1_option2=new Controls::LabeledRadioButton(vb1_group);

        vb1_option2->SetName("Option2");
        vb1_option2->SetText("Option 2");

        Controls::LabeledRadioButton *vb1_option3=new Controls::LabeledRadioButton(vb1_group);

        vb1_option3 ->SetName("Option3");
        vb1_option3->SetText("Option 3");

        Controls::GroupBox *hb1_group=new Controls::GroupBox(verticalGroupBox1);

        hb1_group->SetName("HRadioGroup");
        hb1_group->SetText("HRadio Group");

        Controls::HorizontalLayout *hb1_group_loyout=new Controls::HorizontalLayout();

        hb1_group->SetLayout(hb1_group_loyout);

        Controls::LabeledRadioButton *hb1_option1=new Controls::LabeledRadioButton(hb1_group);

        hb1_option1->SetName("HOption1");
        hb1_option1->SetText("HOption 1");

        Controls::LabeledRadioButton *hb1_option2=new Controls::LabeledRadioButton(hb1_group);

        hb1_option2->SetName("HOption2");
        hb1_option2->SetText("HOption 2");

        Controls::LabeledRadioButton *hb1_option3=new Controls::LabeledRadioButton(hb1_group);

        hb1_option3->SetName("HOption3");
        hb1_option3->SetText("HOption 3");

        Controls::Label* vb1_multilineLabel=new Controls::Label(verticalGroupBox1);
        
        vb1_multilineLabel->SetName("MultilineLabel");
        vb1_multilineLabel->SetText(
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ultrices pharetra scelerisque. "
            "Ut posuere velit ligula suscipit ut lobortis ligula congue. Aliquam lacinia cursus est, quis "
            "aliquam nisl scelerisque vitae. Nunc porta eros sem, nec rhoncus eros. Integer elementum, "
            "quam vitae egestas dictum, mi quam gravida augue, non fringilla lacus nisi sit amet nunc. "
            "Maecenas dolor tellus, consequat sed sodales ut, aliquam ac enim. Nulla facilisi. Maecenas "
            "eleifend, velit a lobortis vehicula, nunc lacus egestas leo, volutpat egestas augue.");
        vb1_multilineLabel->SetWrap(true);
        vb1_multilineLabel->SetAlignment(Position::Left|Position::Top);
        vb1_multilineLabel->SetSizeFlags({SizeFlag::Elastic, SizeFlag::Bloat});
    }

    Font m_font;
};

GWK_REGISTER_TEST(Layout);

}}

