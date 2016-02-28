/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_SCROLLBARBAR_H
#define GWK_CONTROLS_SCROLLBARBAR_H

#include "Gwork/Controls/Dragger.h"
#include "Gwork/Gwork.h"
#include "Gwork/Skin.h"

namespace Gwk
{
    namespace ControlsInternal
    {
        class GWK_EXPORT ScrollBarBar : public ControlsInternal::Dragger
        {
        public:

            GWK_CONTROL(ScrollBarBar, ControlsInternal::Dragger);

            virtual void Render(Skin::Base* skin) override;
            virtual void Layout(Skin::Base* skin) override;

            virtual void OnMouseMoved(int x, int y, int deltaX, int deltaY) override;
            virtual void OnMouseClickLeft(int x, int y, bool bDown) override;

            virtual void MoveTo(int x, int y) override;

            virtual void SetHorizontal()
            {
                m_bHorizontal = true;
            }

            virtual void SetVertical()
            {
                m_bHorizontal = false;
            }

            virtual bool IsVertical()
            {
                return !m_bHorizontal;
            }

            virtual bool IsHorizontal()
            {
                return m_bHorizontal;
            }

            virtual bool IsDepressed()
            {
                return m_bDepressed;
            }

        protected:

            bool m_bHorizontal;

        };


    }
}
#endif // ifndef GWK_CONTROLS_SCROLLBARBAR_H
