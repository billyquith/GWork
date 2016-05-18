/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Anim.h>
#include <Gwork/Utility.h>
#include <cmath>

using namespace Gwk;

#if GWK_ANIMATE

static Gwk::Anim::Animation::List g_animations;
static Gwk::Anim::Animation::ChildList g_animationsListed;

void Gwk::Anim::Add(Gwk::Controls::Base* control, Animation* animation)
{
    animation->m_control = control;
    g_animations[control].push_back(animation);
}

void Gwk::Anim::Cancel(Gwk::Controls::Base* control)
{
    /* cannot use std::list iterator with algorithms based on pointers
     * struct AnimDeletePredicate
     * {
     *  AnimDeletePredicate( Gwk::Controls::Base* control )
     *  {
     *      this->control = control;
     *  }
     *
     *  bool operator() ( Gwk::Anim::Animation* anim )
     *  {
     *      return anim->m_control == control;
     *  }
     *
     *  Gwk::Controls::Base* control;
     * };
     *
     * std::remove_if ( g_animations.begin(), g_animations.end(),
     * AnimDeletePredicate( control ) );
     */
    Gwk::Anim::Animation::List::iterator iAnimations;

    if ((iAnimations = g_animations.find(control)) != g_animations.end())
    {
        Gwk::Anim::Animation::ChildList& ChildAnimationsForControl = iAnimations->second;
        Gwk::Anim::Animation::ChildList::iterator iAnimationChild =
            ChildAnimationsForControl.begin();

        if (iAnimationChild != ChildAnimationsForControl.end())
        {
            do
            {
                delete(*iAnimationChild);
            }
            while (++iAnimationChild != ChildAnimationsForControl.end());
        }

        g_animations.erase(iAnimations);
    }
}

void Gwk::Anim::Think()
{
    Gwk::Anim::Animation::List::iterator it = g_animations.begin();

    if (it != g_animations.end())
    {
        Gwk::Anim::Animation::ChildList::iterator itChild;
        Gwk::Anim::Animation* anim;

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
        while (++it != g_animations.end());
    }
}

Gwk::Anim::TimedAnimation::TimedAnimation(float fLength, float fDelay, float fEase)
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
        return -fTime/2*(std::cos(3.14159f*fTime)-1);

    return std::pow(fTime, fEase);
}

void Gwk::Anim::TimedAnimation::Think()
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

bool Gwk::Anim::TimedAnimation::Finished()
{
    return m_bFinished;
}

#endif // if GWK_ANIMATE
