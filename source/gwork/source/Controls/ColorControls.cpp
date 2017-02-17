/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 *
 *  The colourspace conversion functions al_color_hsv_to_rgb & al_color_rgb_to_hsv are
 *  from the Allegro 5 colour library.
 *  License: http://alleg.sourceforge.net/license.html
 */


#include <Gwork/Utility.h>
#include <Gwork/Controls/ColorControls.h>
#include <cmath>

namespace Gwk
{
    
using namespace Gwk::Controls;

static void al_color_hsv_to_rgb(float hue, float saturation, float value,
                                float *red, float *green, float *blue)
{
    int d;
    float e, a, b, c;
    hue = std::fmod(hue, 360.f);
    if (hue < 0) hue += 360.f;
    d = hue / 60.f;
    e = hue / 60.f - d;
    a = value * (1.f - saturation);
    b = value * (1.f - e * saturation);
    c = value * (1.f - (1.f - e) * saturation);
    switch (d) {
        default:
        case 0: *red = value, *green = c,     *blue = a;     return;
        case 1: *red = b,     *green = value, *blue = a;     return;
        case 2: *red = a,     *green = value, *blue = c;     return;
        case 3: *red = a,     *green = b,     *blue = value; return;
        case 4: *red = c,     *green = a,     *blue = value; return;
        case 5: *red = value, *green = a,     *blue = b;     return;
    }
}

static void al_color_rgb_to_hsv(float red, float green, float blue,
                                float *hue, float *saturation, float *value)
{
    float a, b, c, d;
    if (red > green)
        if (red > blue)
            if (green > blue)
                a = red, b = green - blue, c = blue, d = 0;
            else
                a = red, b = green - blue, c = green, d = 0;
            else
                a = blue, b = red - green, c = green, d = 4;
            else
            {
                if (red > blue)
                    a = green, b = blue - red, c = blue, d = 2;
                else
                {
                    if (green > blue)
                        a = green, b = blue - red, c = red, d = 2;
                    else
                        a = blue, b = red - green, c = red, d = 4;
                }
            }
    
    if (a == c)
        *hue = 0;
    else
        *hue = 60 * (d + b / (a - c));
    if (*hue < 0)
        *hue += 360;
    if (*hue > 360)
        *hue -= 360;
    
    if (a == 0)
        *saturation = 0;
    else
        *saturation = (a - c) / a;
    *value = a;
}

static inline Color HSVToColor(float h, float s, float v)
{
    float r,g,b;
    al_color_hsv_to_rgb(h,s,v, &r,&g,&b);
    return Color(r*255.f, g*255.f, b*255.f, 255);
}

static inline HSV RGBtoHSV(int r, int g, int b)
{
    HSV hsv;
    al_color_rgb_to_hsv(r,g,b, &hsv.h,&hsv.s,&hsv.v);    
    return hsv;
}

GWK_CONTROL_CONSTRUCTOR(ColorLerpBox)
{
    SetColor(Gwk::Color(255, 128, 0, 255));
    SetSize(128, 128);
    SetMouseInputEnabled(true);
    m_bDepressed = false;
}

//// Find a place to put this? color member?
//static Gwk::Color LerpColor(Gwk::Color& toColor, Gwk::Color& fromColor, float amount)
//{
//    Gwk::Color colorDelta = toColor-fromColor;
//    colorDelta.r *= amount;
//    colorDelta.g *= amount;
//    colorDelta.b *= amount;
//    Gwk::Color newColor = fromColor+colorDelta;
//    return newColor;
//}

Gwk::Color ColorLerpBox::GetSelectedColor()
{
    return GetColorAtPos(cursorPos.x, cursorPos.y);
}

void ColorLerpBox::SetColor(Gwk::Color color, bool onlyHue)
{
    HSV hsv = RGBtoHSV(color.r, color.g, color.b);
    m_hue = hsv.h;

    if (!onlyHue)
    {
        cursorPos.x = hsv.s*Width();
        cursorPos.y = (1-hsv.v)*Height();
    }

    onSelectionChanged.Call(this);
}

void ColorLerpBox::OnMouseMoved(int x, int y, int /*deltaX*/, int /*deltaY*/)
{
    if (m_bDepressed)
    {
        cursorPos = CanvasPosToLocal(Gwk::Point(x, y));

        // Do we have clamp?
        cursorPos.x = Clamp(cursorPos.x, 0, Width());
        cursorPos.y = Clamp(cursorPos.y, 0, Height());

        onSelectionChanged.Call(this);
    }
}

void ColorLerpBox::OnMouseClickLeft(int x, int y, bool bDown)
{
    m_bDepressed = bDown;

    if (bDown)
        Gwk::MouseFocus = this;
    else
        Gwk::MouseFocus = nullptr;

    OnMouseMoved(x, y, 0, 0);
}

Gwk::Color ColorLerpBox::GetColorAtPos(int x, int y)
{
    float xPercent = static_cast<float>(x) / Width();
    float yPercent = 1.f - static_cast<float>(y) / Height();
    Gwk::Color result = HSVToColor(m_hue, xPercent, yPercent);
    result.a = 255;
    return result;
}

void ColorLerpBox::Render(Gwk::Skin::Base* skin)
{
    // Is there any way to move this into skin? Not for now, no idea how we'll
    // "actually" render these
    ParentClass::Render(skin);

    for (int x = 0; x < Width(); x++)
    {
        for (int y = 0; y < Height(); y++)
        {
            skin->GetRender()->SetDrawColor(GetColorAtPos(x, y));
            skin->GetRender()->DrawPixel(x, y);
        }
    }

    skin->GetRender()->SetDrawColor(Gwk::Color(0, 0, 0, 255));
    skin->GetRender()->DrawLinedRect(GetRenderBounds());
    Gwk::Color selected = GetSelectedColor();

    if ((selected.r+selected.g+selected.b)/3 < 170)
        skin->GetRender()->SetDrawColor(Gwk::Color(255, 255, 255, 255));
    else
        skin->GetRender()->SetDrawColor(Gwk::Color(0, 0, 0, 255));

    Gwk::Rect testRect = Gwk::Rect(cursorPos.x-3, cursorPos.y-3, 6, 6);
    skin->GetRender()->DrawShavedCornerRect(testRect);
}

GWK_CONTROL_CONSTRUCTOR(ColorSlider)
{
    SetSize(32, 128);
    SetMouseInputEnabled(true);
    m_bDepressed = false;
}

void ColorSlider::Render(Gwk::Skin::Base* skin)
{
    // Is there any way to move this into skin? Not for now, no idea how we'll
    // "actually" render these

    for (int y = 0; y < Height(); y++)
    {
        const float yPercent = static_cast<float>(y) / Height();
        skin->GetRender()->SetDrawColor(HSVToColor(yPercent*360, 1, 1));
        skin->GetRender()->DrawFilledRect(Gwk::Rect(5, y, Width()-10, 1));
    }

    int drawHeight = m_selectedDist-3;
    // Draw our selectors
    skin->GetRender()->SetDrawColor(Gwk::Color(0, 0, 0, 255));
    skin->GetRender()->DrawFilledRect(Gwk::Rect(0, drawHeight+2, Width(), 1));
    skin->GetRender()->DrawFilledRect(Gwk::Rect(0, drawHeight, 5, 5));
    skin->GetRender()->DrawFilledRect(Gwk::Rect(Width()-5, drawHeight, 5, 5));
    skin->GetRender()->SetDrawColor(Gwk::Color(255, 255, 255, 255));
    skin->GetRender()->DrawFilledRect(Gwk::Rect(1, drawHeight+1, 3, 3));
    skin->GetRender()->DrawFilledRect(Gwk::Rect(Width()-4, drawHeight+1, 3, 3));
}

void ColorSlider::OnMouseClickLeft(int x, int y, bool bDown)
{
    m_bDepressed = bDown;

    if (bDown)
        Gwk::MouseFocus = this;
    else
        Gwk::MouseFocus = nullptr;

    OnMouseMoved(x, y, 0, 0);
}

Gwk::Color ColorSlider::GetColorAtHeight(int y)
{
    const float yPercent = static_cast<float>(y) / Height();
    return HSVToColor(yPercent*360, 1, 1);
}

void ColorSlider::OnMouseMoved(int x, int y, int /*deltaX*/, int /*deltaY*/)
{
    if (m_bDepressed)
    {
        Gwk::Point cursorPos = CanvasPosToLocal(Gwk::Point(x, y));

        if (cursorPos.y < 0)
            cursorPos.y = 0;

        if (cursorPos.y > Height())
            cursorPos.y = Height();

        m_selectedDist = cursorPos.y;
        onSelectionChanged.Call(this);
    }
}

void ColorSlider::SetColor(Gwk::Color color)
{
    HSV hsv = RGBtoHSV(color.r, color.g, color.b);
    m_selectedDist = hsv.h/360*Height();
    onSelectionChanged.Call(this);
}

Gwk::Color ColorSlider::GetSelectedColor()
{
    return GetColorAtHeight(m_selectedDist);
}

} // namespace Gwk
