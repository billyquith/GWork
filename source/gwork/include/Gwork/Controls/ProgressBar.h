/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_PROGRESSBAR_H
#define GWK_CONTROLS_PROGRESSBAR_H
#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>


namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT ProgressBar : public Label
        {
        public:

            GWK_CONTROL(ProgressBar, Label);

            void Render(Skin::Base* skin) override;

            virtual void SetVertical()
            {
                m_bHorizontal = false;
            }

            virtual void SetHorizontal()
            {
                m_bHorizontal = true;
            }

            virtual void  SetProgress(float val);
            virtual float GetProgress() const
            {
                return m_fProgress;
            }

            virtual void SetAutoLabel(bool b)
            {
                m_bAutoLabel = b;
            }

            virtual float GetCycleSpeed();
            virtual void  SetCycleSpeed(float f);

            virtual void CycleThink(float fDelta);

        protected:

            float m_fProgress;
            float m_fCycleSpeed;

            bool m_bHorizontal;
            bool m_bAutoLabel;
        };


    }
}
#endif // ifndef GWK_CONTROLS_PROGRESSBAR_H
