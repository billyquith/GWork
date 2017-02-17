/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Utility.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/NumericUpDown.h>
#include <cmath>

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(NumericUpDown)
{
    SetSize(100, 20);
    Controls::Base* splitter = new Controls::Base(this);
    splitter->Dock(Position::Right);
    splitter->SetWidth(13);
    NumericUpDownButton_Up* buttonUp = new NumericUpDownButton_Up(splitter);
    buttonUp->onPress.Add(this, &NumericUpDown::OnButtonUp);
    buttonUp->SetTabable(false);
    buttonUp->Dock(Position::Top);
    buttonUp->SetHeight(10);
    NumericUpDownButton_Down* buttonDown = new NumericUpDownButton_Down(splitter);
    buttonDown->onPress.Add(this, &NumericUpDown::OnButtonDown);
    buttonDown->SetTabable(false);
    buttonDown->Dock(Position::Fill);
    buttonUp->SetPadding(Padding(0, 1, 1, 0));
    m_max = 100;
    m_min = 0;
    m_number = 0;
    SetText("0");
}

void NumericUpDown::OnButtonUp(Event::Info)
{
    SyncNumberFromText();
    SetIntValue(m_number+1);
}

void NumericUpDown::OnButtonDown(Event::Info)
{
    SyncNumberFromText();
    SetIntValue(m_number-1);
}

void NumericUpDown::SyncTextFromNumber()
{
    SetText(Utility::ToString(m_number));
}

void NumericUpDown::SyncNumberFromText()
{
    SetIntValue(static_cast<int>(std::lround(GetFloatFromText())));
}

void NumericUpDown::SetMin(int i)
{
    m_min = i;
}

void NumericUpDown::SetMax(int i)
{
    m_max = i;
}

void NumericUpDown::SetIntValue(int i)
{
    i = Clamp(i, m_min, m_max);

    if (m_number == i)
        return;

    m_number = i;
    // Don't update the text if we're typing in it..
    // Undone - any reason why not?
    // if ( !IsFocussed() )
    {
        SyncTextFromNumber();
    }
    OnChange();
}

void NumericUpDown::OnChange()
{
    onChanged.Call(this);
}

void NumericUpDown::OnTextChanged()
{
    ParentClass::OnTextChanged();
    SyncNumberFromText();
}

void NumericUpDown::OnEnter()
{
    SyncNumberFromText();
    SyncTextFromNumber();
    ParentClass::OnEnter();
}
