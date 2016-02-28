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

            Gwk::Controls::Base*   m_Control;
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
                    m_iStartSize = iStartSize;
                    m_iDelta = iEndSize-m_iStartSize;
                    m_bHide = bHide;
                }

                virtual void OnStart()
                {
                    m_Control->SetHeight(m_iStartSize);
                }

                virtual void Run(float delta)
                {
                    m_Control->SetHeight(m_iStartSize+(((float)m_iDelta)*delta));
                }

                virtual void OnFinish()
                {
                    m_Control->SetHeight(m_iStartSize+m_iDelta); m_Control->SetHidden(m_bHide);
                }

            protected:

                int m_iStartSize;
                int m_iDelta;
                bool m_bHide;
            };


            class Width : public TimedAnimation
            {
            public:

                Width(int iStartSize, int iEndSize, float fLength, bool bHide = false,
                      float fDelay = 0.0f, float fEase = -1.0f)
                : TimedAnimation(fLength, fDelay, fEase)
                {
                    m_iStartSize = iStartSize;
                    m_iDelta = iEndSize-m_iStartSize;
                    m_bHide = bHide;
                }

                virtual void OnStart()
                {
                    m_Control->SetWidth(m_iStartSize);
                }

                virtual void Run(float delta)
                {
                    m_Control->SetWidth(m_iStartSize+(((float)m_iDelta)*delta));
                }

                virtual void OnFinish()
                {
                    m_Control->SetWidth(m_iStartSize+m_iDelta); m_Control->SetHidden(m_bHide);
                }

            protected:

                int m_iStartSize;
                int m_iDelta;
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
                    m_iStartSize = iStartSize;
                    m_iDelta = iEndSize-m_iStartSize;
                    m_bHide = bHide;
                }

                virtual void OnStart()
                {
                    m_Control->SetPos(m_iStartSize, m_Control->GetPos().y);
                }

                virtual void Run(float delta)
                {
                    m_Control->SetPos(m_iStartSize+(((float)m_iDelta)*delta),
                                      m_Control->GetPos().y);
                }

                virtual void OnFinish()
                {
                    m_Control->SetPos(m_iStartSize+m_iDelta, m_Control->GetPos().y);
                    m_Control->SetHidden(m_bHide);
                }

            protected:

                int m_iStartSize;
                int m_iDelta;
                bool m_bHide;
            };


            class Y : public Anim::TimedAnimation
            {
            public:

                Y(int iStartSize, int iEndSize, float fLength, bool bHide = false,
                  float fDelay = 0.0f, float fEase = 1.0f)
                :   TimedAnimation(fLength, fDelay, fEase)
                {
                    m_iStartSize = iStartSize;
                    m_iDelta = iEndSize-m_iStartSize;
                    m_bHide = bHide;
                }

                virtual void OnStart()
                {
                    m_Control->SetPos(m_Control->GetPos().x, m_iStartSize);
                }

                virtual void Run(float delta)
                {
                    m_Control->SetPos(m_Control->GetPos().x, m_iStartSize+
                                      (((float)m_iDelta)*delta));
                }

                virtual void OnFinish()
                {
                    m_Control->SetPos(m_Control->GetPos().x, m_iStartSize+m_iDelta);
                    m_Control->SetHidden(m_bHide);
                }

            protected:

                int m_iStartSize;
                int m_iDelta;
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
                    m_Control->DelayedDelete();
                }

            };


        }

    }
}

#endif // !GWK_NO_ANIMATION
#endif // GWK_ANIM_H
