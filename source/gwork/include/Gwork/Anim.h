/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_ANIM_H
#define GWK_ANIM_H

#include <Gwork/Gwork.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/Platform.h>

#ifndef GWK_NO_ANIMATION

namespace Gwk
{
    namespace Anim
    {
        class GWK_EXPORT Animation
        {
        public:

            typedef std::list<Animation*> ChildList;
            typedef std::map<Gwk::Controls::Base*, ChildList> List;

            virtual void Think() = 0;
            virtual bool Finished()
            {
                return false;
            }

            virtual ~Animation()
            {
            }

            Gwk::Controls::Base*   m_control;
        };


        GWK_EXPORT void Add(Gwk::Controls::Base* control, Animation* animation);
        GWK_EXPORT void Cancel(Gwk::Controls::Base* control);
        GWK_EXPORT void Think();

        //
        /// Timed animation. Provides a useful base for animations.
        //
        class GWK_EXPORT TimedAnimation : public Animation
        {
        public:

            TimedAnimation(float fLength, float fDelay = 0.0f, float fEase = -1.0f);

            virtual void Think();
            virtual bool Finished();

            //
            // These are the magic functions you should be overriding
            //
            virtual void OnStart()              {}
            virtual void Run(float /*delta*/)   {}
            virtual void OnFinish()             {}

        protected:

            bool m_bStarted;
            bool m_bFinished;
            float m_fStart;
            float m_fEnd;
            float m_fEase;
        };


        namespace Size
        {
            class GWK_EXPORT Height : public TimedAnimation
            {
            public:

                Height(int iStartSize, int iEndSize, float fLength, bool bHide = false,
                       float fDelay = 0.0f, float fEase = -1.0f)
                : TimedAnimation(fLength, fDelay, fEase)
                {
                    m_startSize = iStartSize;
                    m_delta = iEndSize-m_startSize;
                    m_bHide = bHide;
                }

                virtual void OnStart()
                {
                    m_control->SetHeight(m_startSize);
                }

                virtual void Run(float delta)
                {
                    m_control->SetHeight(m_startSize+(((float)m_delta)*delta));
                }

                virtual void OnFinish()
                {
                    m_control->SetHeight(m_startSize+m_delta); m_control->SetHidden(m_bHide);
                }

            protected:

                int m_startSize;
                int m_delta;
                bool m_bHide;
            };


            class Width : public TimedAnimation
            {
            public:

                Width(int iStartSize, int iEndSize, float fLength, bool bHide = false,
                      float fDelay = 0.0f, float fEase = -1.0f)
                : TimedAnimation(fLength, fDelay, fEase)
                {
                    m_startSize = iStartSize;
                    m_delta = iEndSize-m_startSize;
                    m_bHide = bHide;
                }

                virtual void OnStart()
                {
                    m_control->SetWidth(m_startSize);
                }

                virtual void Run(float delta)
                {
                    m_control->SetWidth(m_startSize+(((float)m_delta)*delta));
                }

                virtual void OnFinish()
                {
                    m_control->SetWidth(m_startSize+m_delta); m_control->SetHidden(m_bHide);
                }

            protected:

                int m_startSize;
                int m_delta;
                bool m_bHide;
            };


        }

        namespace Pos
        {
            class GWK_EXPORT X : public Anim::TimedAnimation
            {
            public:

                X(int iStartSize, int iEndSize, float fLength, bool bHide = false,
                  float fDelay = 0.0f, float fEase = 1.0f)
                : TimedAnimation(fLength, fDelay, fEase)
                {
                    m_startSize = iStartSize;
                    m_delta = iEndSize-m_startSize;
                    m_bHide = bHide;
                }

                virtual void OnStart()
                {
                    m_control->SetPos(m_startSize, m_control->GetPos().y);
                }

                virtual void Run(float delta)
                {
                    m_control->SetPos(m_startSize+(((float)m_delta)*delta),
                                      m_control->GetPos().y);
                }

                virtual void OnFinish()
                {
                    m_control->SetPos(m_startSize+m_delta, m_control->GetPos().y);
                    m_control->SetHidden(m_bHide);
                }

            protected:

                int m_startSize;
                int m_delta;
                bool m_bHide;
            };


            class Y : public Anim::TimedAnimation
            {
            public:

                Y(int iStartSize, int iEndSize, float fLength, bool bHide = false,
                  float fDelay = 0.0f, float fEase = 1.0f)
                :   TimedAnimation(fLength, fDelay, fEase)
                {
                    m_startSize = iStartSize;
                    m_delta = iEndSize-m_startSize;
                    m_bHide = bHide;
                }

                virtual void OnStart()
                {
                    m_control->SetPos(m_control->GetPos().x, m_startSize);
                }

                virtual void Run(float delta)
                {
                    m_control->SetPos(m_control->GetPos().x, m_startSize+
                                      (((float)m_delta)*delta));
                }

                virtual void OnFinish()
                {
                    m_control->SetPos(m_control->GetPos().x, m_startSize+m_delta);
                    m_control->SetHidden(m_bHide);
                }

            protected:

                int m_startSize;
                int m_delta;
                bool m_bHide;
            };


        }

        namespace Tools
        {
            class Remove : public TimedAnimation
            {
            public:

                Remove(float fDelay = 0.0f)
                : TimedAnimation(0.0f, fDelay)
                {
                }

                virtual void OnFinish()
                {
                    m_control->DelayedDelete();
                }

            };


        }

    }
}

#endif // !GWK_NO_ANIMATION
#endif // GWK_ANIM_H