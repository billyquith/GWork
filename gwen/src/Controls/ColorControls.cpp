/*
 *  GWEN
 *  Copyright (c) 2010 Facepunch Studios
 *  See license in Gwen.h
 *
 *  The colourspace conversion functions al_color_hsv_to_rgb & al_color_rgb_to_hsv are
 *  from the Allegro 5 colour library.
 *  License: http://alleg.sourceforge.net/license.html
 */


#include "Gwen/Utility.h"
#include "Gwen/Controls/ColorControls.h"

using namespace Gwen;
using namespace Gwen::Controls;


static void al_color_hsv_to_rgb(float hue, float saturation, float value,
                                float *red, float *green, float *blue)
{
    int d;
    float e, a, b, c;
    hue = fmodf(hue, 360);
    if (hue < 0) hue += 360;
    d = hue / 60;
    e = hue / 60 - d;
    a = value * (1 - saturation);
    b = value * (1 - e * saturation);
    c = value * (1 - (1 - e) * saturation);
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

GWEN_CONTROL_CONSTRUCTOR(ColorLerpBox)
{
    SetColor(Gwen::Color(255, 128, 0, 255));
    SetSize(128, 128);
    SetMouseInputEnabled(true);
    m_bDepressed = false;
}

// Find a place to put this? color member?
Gwen::Color LerpColor(Gwen::Color& toColor, Gwen::Color& fromColor, float amount)
{
    Gwen::Color colorDelta = toColor-fromColor;
    colorDelta.r *= amount;
    colorDelta.g *= amount;
    colorDelta.b *= amount;
    Gwen::Color newColor = fromColor+colorDelta;
    return newColor;
}

Gwen::Color ColorLerpBox::GetSelectedColor()
{
    return GetColorAtPos(cursorPos.x, cursorPos.y);
}

void ColorLerpBox::SetColor(Gwen::Color color, bool onlyHue)
{
    HSV hsv = RGBtoHSV(color.r, color.g, color.b);
    m_Hue = hsv.h;

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
        cursorPos = CanvasPosToLocal(Gwen::Point(x, y));

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
        Gwen::MouseFocus = this;
    else
        Gwen::MouseFocus = NULL;

    OnMouseMoved(x, y, 0, 0);
}

Gwen::Color ColorLerpBox::GetColorAtPos(int x, int y)
{
    float xPercent = (float)x / (float)Width();
    float yPercent = 1.f - (float)y/(float)Height();
    Gwen::Color result = HSVToColor(m_Hue, xPercent, yPercent);
    result.a = 255;
    return result;
}

void ColorLerpBox::Render(Gwen::Skin::Base* skin)
{
    // Is there any way to move this into skin? Not for now, no idea how we'll
    // "actually" render these
    BaseClass::Render(skin);

    for (int x = 0; x < Width(); x++)
    {
        for (int y = 0; y < Height(); y++)
        {
            skin->GetRender()->SetDrawColor(GetColorAtPos(x, y));
            skin->GetRender()->DrawPixel(x, y);
        }
    }

    skin->GetRender()->SetDrawColor(Gwen::Color(0, 0, 0, 255));
    skin->GetRender()->DrawLinedRect(GetRenderBounds());
    Gwen::Color selected = GetSelectedColor();

    if ((selected.r+selected.g+selected.b)/3 < 170)
        skin->GetRender()->SetDrawColor(Gwen::Color(255, 255, 255, 255));
    else
        skin->GetRender()->SetDrawColor(Gwen::Color(0, 0, 0, 255));

    Gwen::Rect testRect = Gwen::Rect(cursorPos.x-3, cursorPos.y-3, 6, 6);
    skin->GetRender()->DrawShavedCornerRect(testRect);
}

GWEN_CONTROL_CONSTRUCTOR(ColorSlider)
{
    SetSize(32, 128);
    SetMouseInputEnabled(true);
    m_bDepressed = false;
}

void ColorSlider::Render(Gwen::Skin::Base* skin)
{
    // Is there any way to move this into skin? Not for now, no idea how we'll
    // "actually" render these

    for (int y = 0; y < Height(); y++)
    {
        float yPercent = (float)y/(float)Height();
        skin->GetRender()->SetDrawColor(HSVToColor(yPercent*360, 1, 1));
        skin->GetRender()->DrawFilledRect(Gwen::Rect(5, y, Width()-10, 1));
    }

    int drawHeight = m_iSelectedDist-3;
    // Draw our selectors
    skin->GetRender()->SetDrawColor(Gwen::Color(0, 0, 0, 255));
    skin->GetRender()->DrawFilledRect(Gwen::Rect(0, drawHeight+2, Width(), 1));
    skin->GetRender()->DrawFilledRect(Gwen::Rect(0, drawHeight, 5, 5));
    skin->GetRender()->DrawFilledRect(Gwen::Rect(Width()-5, drawHeight, 5, 5));
    skin->GetRender()->SetDrawColor(Gwen::Color(255, 255, 255, 255));
    skin->GetRender()->DrawFilledRect(Gwen::Rect(1, drawHeight+1, 3, 3));
    skin->GetRender()->DrawFilledRect(Gwen::Rect(Width()-4, drawHeight+1, 3, 3));
}

void ColorSlider::OnMouseClickLeft(int x, int y, bool bDown)
{
    m_bDepressed = bDown;

    if (bDown)
        Gwen::MouseFocus = this;
    else
        Gwen::MouseFocus = NULL;

    OnMouseMoved(x, y, 0, 0);
}

Gwen::Color ColorSlider::GetColorAtHeight(int y)
{
    float yPercent = (float)y/(float)Height();
    return HSVToColor(yPercent*360, 1, 1);
}

void ColorSlider::OnMouseMoved(int x, int y, int /*deltaX*/, int /*deltaY*/)
{
    if (m_bDepressed)
    {
        Gwen::Point cursorPos = CanvasPosToLocal(Gwen::Point(x, y));

        if (cursorPos.y < 0)
            cursorPos.y = 0;

        if (cursorPos.y > Height())
            cursorPos.y = Height();

        m_iSelectedDist = cursorPos.y;
        onSelectionChanged.Call(this);
    }
}

void ColorSlider::SetColor(Gwen::Color color)
{
    HSV hsv = RGBtoHSV(color.r, color.g, color.b);
    m_iSelectedDist = hsv.h/360*Height();
    onSelectionChanged.Call(this);
}

Gwen::Color ColorSlider::GetSelectedColor()
{
    return GetColorAtHeight(m_iSelectedDist);
}
