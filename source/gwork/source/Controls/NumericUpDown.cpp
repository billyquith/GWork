/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
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
    m_lastNumber = 0;
    SetText("0");
}

void NumericUpDown::OnButtonUp(Base* /*control*/)
{
    SetIntValue(GetIntValueUnclamped() + 1);
}

void NumericUpDown::OnButtonDown(Base* /*control*/)
{
    SetIntValue(GetIntValueUnclamped() - 1);
}

int NumericUpDown::GetIntValueUnclamped()
{
    return static_cast<int>(std::lround(GetFloatFromText()));
}

int NumericUpDown::GetIntValue()
{
    return Clamp(GetIntValueUnclamped(), m_min, m_max);
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

    SetText(Utility::ToString(i));

    if (m_lastNumber != i)
    {
        m_lastNumber = i;
        OnChange();
    }
}

void NumericUpDown::OnChange()
{
    onChanged.Call(this);
}

void NumericUpDown::OnEnter()
{
    SetIntValue(GetIntValueUnclamped());
    ParentClass::OnEnter();
}
