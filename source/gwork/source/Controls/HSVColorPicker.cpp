/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Controls/HSVColorPicker.h>
#include <Gwork/Controls/ColorControls.h>
#include <Gwork/Controls/ColorPicker.h>
#include <Gwork/Controls/TextBox.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Utility.h>

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(HSVColorPicker)
{
    SetMouseInputEnabled(true);
    SetSize(256, 64);
//    EnableCacheToTexture();   // TODO - fix texture caching.

    m_lerpBox = new Gwk::Controls::ColorLerpBox(this);
    m_lerpBox->onSelectionChanged.Add(this, &HSVColorPicker::ColorBoxChanged);
    m_lerpBox->Dock(Position::Left);

    m_colorSlider = new Gwk::Controls::ColorSlider(this);
    m_colorSlider->SetPos(m_lerpBox->Width()+15, 5);
    m_colorSlider->onSelectionChanged.Add(this, &HSVColorPicker::ColorSliderChanged);
    m_colorSlider->Dock(Position::Left);

    m_after = new Gwk::ControlsInternal::ColorDisplay(this);
    m_after->SetSize(48, 24);
    m_after->SetPos(m_colorSlider->X()+m_colorSlider->Width()+15, 5);

    m_before = new Gwk::ControlsInternal::ColorDisplay(this);
    m_before->SetSize(48, 24);
    m_before->SetPos(m_after->X(), 28);

    int x = m_before->X();
    int y = m_before->Y()+30;
    {
        Label* label = new Label(this);
        label->SetText("r:");
        label->SizeToContents();
        label->SetPos(x, y);

        TextBoxNumeric* numeric = new TextBoxNumeric(this);
        numeric->SetName("RedBox");
        numeric->SetPos(x+15, y-1);
        numeric->SetSize(26, 16);
        numeric->SetSelectAllOnFocus(true);
        numeric->onTextChanged.Add(this, &HSVColorPicker::NumericTyped);
    }
    y += 20;
    {
        Label* label = new Label(this);
        label->SetText("G:");
        label->SizeToContents();
        label->SetPos(x, y);

        TextBoxNumeric* numeric = new TextBoxNumeric(this);
        numeric->SetName("GreenBox");
        numeric->SetPos(x+15, y-1);
        numeric->SetSize(26, 16);
        numeric->SetSelectAllOnFocus(true);
        numeric->onTextChanged.Add(this, &HSVColorPicker::NumericTyped);
    }
    y += 20;
    {
        Label* label = new Label(this);
        label->SetText("B:");
        label->SizeToContents();
        label->SetPos(x, y);

        TextBoxNumeric* numeric = new TextBoxNumeric(this);
        numeric->SetName("BlueBox");
        numeric->SetPos(x+15, y-1);
        numeric->SetSize(26, 16);
        numeric->SetSelectAllOnFocus(true);
        numeric->onTextChanged.Add(this, &HSVColorPicker::NumericTyped);
    }
}

void HSVColorPicker::NumericTyped(Gwk::Controls::Base* control)
{
    TextBoxNumeric* box = gwk_cast<TextBoxNumeric>(control);

    if (!box)
        return;

    if (box->GetText() == "")
        return;

    int textValue = Clamp( atoi(box->GetText().c_str()), 0, 255 );

    Gwk::Color newColor = GetColor();

    if (box->GetName().find("Red") != Gwk::String::npos)
        newColor.r = textValue;
    else if (box->GetName().find("Green") != Gwk::String::npos)
        newColor.g = textValue;
    else if (box->GetName().find("Blue") != Gwk::String::npos)
        newColor.b = textValue;
    else if (box->GetName().find("Alpha") != Gwk::String::npos)
        newColor.a = textValue;

    SetColor(newColor);
}

void HSVColorPicker::UpdateControls(Gwk::Color color)
{
    // What in the FUCK
    TextBoxNumeric* redBox = gwk_cast<TextBoxNumeric>(FindChildByName("RedBox", false));

    // if you need to deal with pesky __int8 or char displaying as character instead of value, see:
    // https://stackoverflow.com/questions/19562103/uint8-t-cant-be-printed-with-cout/#21389821

    if (redBox)
        redBox->SetText(Gwk::Utility::ToString(+color.r), false);

    TextBoxNumeric* greenBox = gwk_cast<TextBoxNumeric>(FindChildByName("GreenBox", false));

    if (greenBox)
        greenBox->SetText(Gwk::Utility::ToString(+color.g), false);

    TextBoxNumeric* blueBox = gwk_cast<TextBoxNumeric>(FindChildByName("BlueBox", false));

    if (blueBox)
        blueBox->SetText(Gwk::Utility::ToString(+color.b), false);

    m_after->SetColor(color);
}

void HSVColorPicker::SetColor(Gwk::Color color, bool onlyHue, bool reset)
{
    UpdateControls(color);

    if (reset)
        m_before->SetColor(color);

    m_colorSlider->SetColor(color);
    m_lerpBox->SetColor(color,  onlyHue);
    m_after->SetColor(color);
}

Gwk::Color HSVColorPicker::GetColor()
{
    return m_lerpBox->GetSelectedColor();
}

void HSVColorPicker::ColorBoxChanged(Gwk::Controls::Base* /*control*/)
{
    onColorChanged.Call(this);
    UpdateControls(GetColor());
    Invalidate();
}

void HSVColorPicker::ColorSliderChanged(Gwk::Controls::Base* /*control*/)
{
    if (m_lerpBox)
        m_lerpBox->SetColor(m_colorSlider->GetSelectedColor(),  true);

    Invalidate();
}
