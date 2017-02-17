/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/ScrollControl.h>
#include <Gwork/Controls/ProgressBar.h>
#include <Gwork/Anim.h>
#include <Gwork/Utility.h>

using namespace Gwk;
using namespace Gwk::Controls;

class ProgressBarThink : public Gwk::Anim::Animation
{
public:

    ProgressBarThink()
    :   m_fLastFrame(0.f)
    {}

    void Think() override
    {
        float fDiff = Platform::GetTimeInSeconds()-m_fLastFrame;
        gwk_cast<ProgressBar>(m_control)->CycleThink(Gwk::Clamp(fDiff, 0.f, 0.3f));
        m_fLastFrame = Platform::GetTimeInSeconds();
    }

    float m_fLastFrame;
};


GWK_CONTROL_CONSTRUCTOR(ProgressBar)
{
    SetMouseInputEnabled(true);
    SetBounds(Gwk::Rect(0, 0, 128, 32));
    SetTextPadding(Padding(3, 3, 3, 3));
    SetHorizontal();
    SetAlignment(Gwk::Position::Center);
    m_fProgress = 0.0f;
    m_bAutoLabel = true;
    m_fCycleSpeed = 0.0f;
    Gwk::Anim::Add(this, new ProgressBarThink());
}

void ProgressBar::SetProgress(float val)
{
    m_fProgress = Clamp(val, 0.f, 1.f);

    if (m_bAutoLabel)
    {
        const int displayVal = m_fProgress*100;
        SetText(Utility::ToString(displayVal)+"%");
    }
}

void ProgressBar::CycleThink(float fDelta)
{
    if (!Visible())
        return;

    if (m_fCycleSpeed == 0.0f)
        return;

    m_fProgress += m_fCycleSpeed*fDelta;

    if (m_fProgress < 0.0f)
        m_fProgress += 1.0f;

    if (m_fProgress > 1.0f)
        m_fProgress -= 1.0f;

    Redraw();
}

void ProgressBar::Render(Skin::Base* skin)
{
    skin->DrawProgressBar(this, m_bHorizontal, m_fProgress);
}

float ProgressBar::GetCycleSpeed()
{
    return m_fCycleSpeed;
}

void ProgressBar::SetCycleSpeed(float f)
{
    m_fCycleSpeed = f;
}
