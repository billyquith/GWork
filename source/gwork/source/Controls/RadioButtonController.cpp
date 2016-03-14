/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Controls/RadioButtonController.h>
#include <Gwork/Controls/RadioButton.h>
#include <Gwork/Utility.h>

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(RadioButtonController)
{
    m_selected = nullptr;
    SetTabable(false);
    SetKeyboardInputEnabled(false);
}

void RadioButtonController::OnRadioClicked(Event::Info info)
{
    auto fromPanel = info.ControlCaller;
    
    RadioButton* checkedRadioButton = gwk_cast<RadioButton>(fromPanel);

    // Iterate through all other buttons and set them to false;
    for (Base::List::iterator iter = Children.begin(); iter != Children.end(); ++iter)
    {
        Base* child = *iter;
        LabeledRadioButton* lRB = gwk_cast<LabeledRadioButton>(child);

        if (lRB)
        {
            RadioButton* childRadioButton = lRB->GetRadioButton();

            if (childRadioButton == checkedRadioButton)
                m_selected = lRB;
            else
                lRB->GetRadioButton()->SetChecked(false);
        }
    }

    OnChange();
}

void RadioButtonController::OnChange()
{
    onSelectionChange.Call(this);
}

LabeledRadioButton* RadioButtonController::AddOption(const Gwk::String& strText,
                                                     const Gwk::String& strOptionName)
{
    LabeledRadioButton* lrb = new LabeledRadioButton(this);
    lrb->SetName(strOptionName);
    lrb->GetLabel()->SetText(strText);
    lrb->GetRadioButton()->onChecked.Add(this, &RadioButtonController::OnRadioClicked);
    lrb->Dock(Position::Top);
    lrb->SetMargin(Margin(0, 1, 0, 1));
    lrb->SetKeyboardInputEnabled(false);
    lrb->SetTabable(false);
    Invalidate();
    return lrb;
}
