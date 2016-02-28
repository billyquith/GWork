/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include "Gwork/Controls/HSVColorPicker.h"
#include "Gwork/Controls/ColorControls.h"
#include "Gwork/Controls/ColorPicker.h"
#include "Gwork/Controls/TextBox.h"
#include "Gwork/Controls/Label.h"
#include "Gwork/Utility.h"

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(HSVColorPicker)
{
    SetMouseInputEnabled(true);
    SetSize(256, 64);
    EnableCacheToTexture();
    
    m_LerpBox = new Gwk::Controls::ColorLerpBox(this);
    m_LerpBox->onSelectionChanged.Add(this, &HSVColorPicker::ColorBoxChanged);
    m_LerpBox->Dock(Docking::Left);
    
    m_ColorSlider = new Gwk::Controls::ColorSlider(this);
    m_ColorSlider->SetPos(m_LerpBox->Width()+15, 5);
    m_ColorSlider->onSelectionChanged.Add(this, &HSVColorPicker::ColorSliderChanged);
    m_ColorSlider->Dock(Docking::Left);
    
    m_After = new Gwk::ControlsInternal::ColorDisplay(this);
    m_After->SetSize(48, 24);
    m_After->SetPos(m_ColorSlider->X()+m_ColorSlider->Width()+15, 5);
    
    m_Before = new Gwk::ControlsInternal::ColorDisplay(this);
    m_Before->SetSize(48, 24);
    m_Before->SetPos(m_After->X(), 28);
    
    int x = m_Before->X();
    int y = m_Before->Y()+30;
    {
        Label* label = new Label(this);
        label->SetText("R:");
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

    if (redBox)
        redBox->SetText(Gwk::Utility::ToString((int)color.r), false);

    TextBoxNumeric* greenBox = gwk_cast<TextBoxNumeric>(FindChildByName("GreenBox", false));

    if (greenBox)
        greenBox->SetText(Gwk::Utility::ToString((int)color.g), false);

    TextBoxNumeric* blueBox = gwk_cast<TextBoxNumeric>(FindChildByName("BlueBox", false));

    if (blueBox)
        blueBox->SetText(Gwk::Utility::ToString((int)color.b), false);

    m_After->SetColor(color);
}

void HSVColorPicker::SetColor(Gwk::Color color, bool onlyHue, bool reset)
{
    UpdateControls(color);

    if (reset)
        m_Before->SetColor(color);

    m_ColorSlider->SetColor(color);
    m_LerpBox->SetColor(color,  onlyHue);
    m_After->SetColor(color);
}

Gwk::Color HSVColorPicker::GetColor()
{
    return m_LerpBox->GetSelectedColor();
}

void HSVColorPicker::ColorBoxChanged(Gwk::Controls::Base* /*pControl*/)
{
    onColorChanged.Call(this);
    UpdateControls(GetColor());
    Invalidate();
}

void HSVColorPicker::ColorSliderChanged(Gwk::Controls::Base* /*pControl*/)
{
    if (m_LerpBox)
        m_LerpBox->SetColor(m_ColorSlider->GetSelectedColor(),  true);

    Invalidate();
}
