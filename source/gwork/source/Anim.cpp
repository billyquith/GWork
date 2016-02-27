/*
 *  GWEN
 *  Copyright (c) 2010 Facepunch Studios
 *  See license in Gwen.h
 */


#include "Gwen/Anim.h"
#include "Gwen/Utility.h"
#include <math.h>

using namespace Gwen;

#ifndef GWEN_NO_ANIMATION

static Gwen::Anim::Animation::List g_Animations;
static Gwen::Anim::Animation::ChildList g_AnimationsListed;

void Gwen::Anim::Add(Gwen::Controls::Base* control, Animation* animation)
{
    animation->m_Control = control;
    g_Animations[control].push_back(animation);
}

void Gwen::Anim::Cancel(Gwen::Controls::Base* control)
{
    /* cannot use std::list iterator with algorithms based on pointers
     * struct AnimDeletePredicate
     * {
     *  AnimDeletePredicate( Gwen::Controls::Base* control )
     *  {
     *      this->control = control;
     *  }
     *
     *  bool operator() ( Gwen::Anim::Animation* anim )
     *  {
     *      return anim->m_Control == control;
     *  }
     *
     *  Gwen::Controls::Base* control;
     * };
     *
     * std::remove_if ( g_Animations.begin(), g_Animations.end(),
     * AnimDeletePredicate( control ) );
     */
    Gwen::Anim::Animation::List::iterator iAnimations;

    if ((iAnimations = g_Animations.find(control)) != g_Animations.end())
    {
        Gwen::Anim::Animation::ChildList& ChildAnimationsForControl = iAnimations->second;
        Gwen::Anim::Animation::ChildList::iterator iAnimationChild =
            ChildAnimationsForControl.begin();

        if (iAnimationChild != ChildAnimationsForControl.end())
        {
            do
            {
                delete(*iAnimationChild);
            }
            while (++iAnimationChild != ChildAnimationsForControl.end());
        }

        g_Animations.erase(iAnimations);
    }
}

void Gwen::Anim::Think()
{
    Gwen::Anim::Animation::List::iterator it = g_Animations.begin();

    if (it != g_Animations.end())
    {
        Gwen::Anim::Animation::ChildList::iterator itChild;
        Gwen::Anim::Animation* anim;

        do
        {
            if ((itChild = it->second.begin()) != it->second.end())
            {
                do
                {
                    anim = *itChild;
                    anim->Think();

                    if (anim->Finished())
                    {
                        itChild = it->second.erase(itChild);
                        delete anim;
                    }
                    else
                    {
                        ++itChild;
                    }
                }
                while (itChild != it->second.end());
            }
        }
        while (++it != g_Animations.end());
    }
}

Gwen::Anim::TimedAnimation::TimedAnimation(float fLength, float fDelay, float fEase)
{
    m_fStart = Platform::GetTimeInSeconds()+fDelay;
    m_fEnd = m_fStart+fLength;
    m_fEase = fEase;
    m_bStarted = false;
    m_bFinished = false;
}

static float GetEased(float fTime, float fEase)
{
    // Ease in and Out if ease is < 0
    if (fEase < 0)
        return -fTime/2*(cosf(3.14159f*fTime)-1);

    return powf(fTime, fEase);
}

void Gwen::Anim::TimedAnimation::Think()
{
    if (m_bFinished)
        return;

    float fCurrent = Platform::GetTimeInSeconds();
    float fSecondsIn = fCurrent-m_fStart;

    if (fSecondsIn < 0.0f)
        return;

    if (!m_bStarted)
    {
        m_bStarted = true;
        OnStart();
    }

    float fDelta = Clamp(fSecondsIn/(m_fEnd-m_fStart), 0.f, 1.f);

    float fEased = GetEased(fDelta, m_fEase);
    Run(fEased);

    if (fDelta == 1.0f)
    {
        m_bFinished = true;
        OnFinish();
    }
}

bool Gwen::Anim::TimedAnimation::Finished()
{
    return m_bFinished;
}

#endif // ifndef GWEN_NO_ANIMATION
