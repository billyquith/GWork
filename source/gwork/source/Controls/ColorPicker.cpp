/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Controls/ColorPicker.h>
#include <Gwork/Controls/HorizontalSlider.h>
#include <Gwork/Controls/GroupBox.h>
#include <Gwork/Controls/TextBox.h>
#include <Gwork/Utility.h>

using namespace Gwk;
using namespace Gwk::Controls;
using namespace Gwk::ControlsInternal;

static const int gc_pickChannelHeight = 40;     // As small as boxes will go without clipping.


GWK_CONTROL_CONSTRUCTOR(ColorPicker)
{
    SetMouseInputEnabled(true);
    SetSize(256, 150);
    CreateControls();
    SetColor(Gwk::Color(50, 60, 70, 255));
}

void ColorPicker::CreateColorControl(Gwk::String name, int y)
{
    const int colorSize = 10;
    
    GroupBox* colorGroup = new GroupBox(this);
    colorGroup->SetPos(10, y);
    colorGroup->SetText(name);
    colorGroup->SetSize(160, gc_pickChannelHeight);
    colorGroup->SetName(name+"groupbox");
    
    ColorDisplay* disp = new ColorDisplay(colorGroup);
    disp->SetName(name);
    disp->SetBounds(0, 0, colorSize, colorSize);
    
    TextBoxNumeric* numeric = new TextBoxNumeric(colorGroup);
    numeric->SetName(name+"Box");
    numeric->SetPos(105, 0);
    numeric->SetSize(26, 16);
    numeric->SetSelectAllOnFocus(true);
    numeric->onTextChanged.Add(this, &ColorPicker::OnNumericTyped);
    
    HorizontalSlider* slider = new HorizontalSlider(colorGroup);
    slider->SetPos(colorSize+5, 0);
    slider->SetRange(0, 255);
    slider->SetSize(80, std::max(colorSize, 15));
    slider->SetName(name+"Slider");
    slider->onValueChanged.Add(this, &ColorPicker::OnSlidersMoved);
}

void ColorPicker::OnNumericTyped(Event::Info info)
{
    TextBoxNumeric* box = gwk_cast<TextBoxNumeric>(info.ControlCaller);

    if (!box)
        return;

    if (box->GetText() == "")
        return;

    const int textValue = Clamp( atoi(box->GetText().c_str()), 0, 255 );

    if (box->GetName().find("Red") != Gwk::String::npos)
        SetRed(textValue);

    if (box->GetName().find("Green") != Gwk::String::npos)
        SetGreen(textValue);

    if (box->GetName().find("Blue") != Gwk::String::npos)
        SetBlue(textValue);

    if (box->GetName().find("Alpha") != Gwk::String::npos)
        SetAlpha(textValue);

    UpdateControls();
}

void ColorPicker::SetColor(Gwk::Color color)
{
    m_color = color;
    UpdateControls();
}

void ColorPicker::CreateControls()
{
    const int startY = 0;
    
    CreateColorControl("Red",   startY);
    CreateColorControl("Green", startY+gc_pickChannelHeight);
    CreateColorControl("Blue",  startY+gc_pickChannelHeight*2);
    CreateColorControl("Alpha", startY+gc_pickChannelHeight*3);
    
    GroupBox* finalGroup = new GroupBox(this);
    finalGroup->SetPos(180, 40);
    finalGroup->SetSize(60, 60);
    finalGroup->SetText("Result");
    finalGroup->SetName("ResultGroupBox");
    
    ColorDisplay* disp = new ColorDisplay(finalGroup);
    disp->SetName("Result");
    disp->SetBounds(0, 10, 32, 32);
    disp->SetDrawCheckers(true);
    // UpdateControls();
}

void ColorPicker::UpdateColorControls(Gwk::String name, Gwk::Color col, int sliderVal)
{
    ColorDisplay* disp = gwk_cast<ColorDisplay>(FindChildByName(name, true));
    disp->SetColor(col);
    
    HorizontalSlider* slider = gwk_cast<HorizontalSlider>(FindChildByName(name+"Slider", true));
    slider->SetFloatValue(sliderVal);
    
    TextBoxNumeric* box = gwk_cast<TextBoxNumeric>(FindChildByName(name+"Box", true));
    box->SetText(Gwk::Utility::ToString(sliderVal));
}

void ColorPicker::UpdateControls()
{
    // This is a little weird, but whatever for now
    UpdateColorControls("Red",     Color(GetColor().r, 0, 0, 255), GetColor().r);
    UpdateColorControls("Green",   Color(0, GetColor().g, 0, 255), GetColor().g);
    UpdateColorControls("Blue",    Color(0, 0, GetColor().b, 255), GetColor().b);
    UpdateColorControls("Alpha",   Color(255, 255, 255, GetColor().a), GetColor().a);
    
    ColorDisplay* disp = gwk_cast<ColorDisplay>(FindChildByName("Result", true));
    disp->SetColor(Color(GetColor().r, GetColor().g, GetColor().b, GetColor().a));
    onColorChanged.Call(this);
}

void ColorPicker::OnSlidersMoved(Event::Info info)
{
    /*
     * HorizontalSlider* redSlider		= gwk_cast<HorizontalSlider>(	FindChildByName(
     * "RedSlider",   true ) );
     * HorizontalSlider* greenSlider	= gwk_cast<HorizontalSlider>(	FindChildByName(
     * "GreenSlider", true ) );
     * HorizontalSlider* blueSlider	= gwk_cast<HorizontalSlider>(	FindChildByName(
     * "BlueSlider",  true ) );
     * HorizontalSlider* alphaSlider	= gwk_cast<HorizontalSlider>(	FindChildByName(
     * "AlphaSlider", true ) );
     */
    HorizontalSlider* slider = gwk_cast<HorizontalSlider>(info.ControlCaller);

    if (slider)
        SetColorByName(GetColorFromName(slider->GetName()), slider->GetFloatValue());

    UpdateControls();
    // SetColor( Gwk::Color( redSlider->GetValue(), greenSlider->GetValue(),
    // blueSlider->GetValue(), alphaSlider->GetValue() ) );
}

void ColorPicker::Layout(Skin::Base* skin)
{
    ParentClass::Layout(skin);
    SizeToChildren(false, true);
    SetSize(Width(), Height()+5);
    GroupBox* groupBox = gwk_cast<GroupBox>(FindChildByName("ResultGroupBox", true));

    if (groupBox)
        groupBox->SetPos(groupBox->X(), Height()/2 - groupBox->Height()/2);

    UpdateControls();
}

int ColorPicker::GetColorByName(Gwk::String colorName)
{
    if (colorName == "Red")
        return GetColor().r;
    else if (colorName == "Green")
        return GetColor().g;
    else if (colorName == "Blue")
        return GetColor().b;
    else if (colorName == "Alpha")
        return GetColor().a;
    else
        return 0;
}

Gwk::String ColorPicker::GetColorFromName(Gwk::String name)
{
    if (name.find("Red") != Gwk::String::npos)
        return "Red";

    if (name.find("Green") != Gwk::String::npos)
        return "Green";

    if (name.find("Blue") != Gwk::String::npos)
        return "Blue";

    if (name.find("Alpha") != Gwk::String::npos)
        return "Alpha";
    else
        return "";
}

void ColorPicker::SetColorByName(Gwk::String colorName, int colorValue)
{
    if (colorName == "Red")
        SetRed(colorValue);
    else if (colorName == "Green")
        SetGreen(colorValue);
    else if (colorName == "Blue")
        SetBlue(colorValue);
    else if (colorName == "Alpha")
        SetAlpha(colorValue);
}

void ColorPicker::SetAlphaVisible(bool visible)
{
    GroupBox* groupBox = gwk_cast<GroupBox>(FindChildByName("Alphagroupbox", true));
    groupBox->SetHidden(!visible);
    Invalidate();
}
