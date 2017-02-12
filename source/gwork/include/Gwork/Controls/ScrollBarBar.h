/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_SCROLLBARBAR_H
#define GWK_CONTROLS_SCROLLBARBAR_H

#include <Gwork/Controls/Dragger.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>

namespace Gwk
{
    namespace ControlsInternal
    {
        class GWK_EXPORT ScrollBarBar : public ControlsInternal::Dragger
        {
        public:

            GWK_CONTROL(ScrollBarBar, ControlsInternal::Dragger);

            void Render(Skin::Base* skin) override;
            void Layout(Skin::Base* skin) override;

            void OnMouseMoved(int x, int y, int deltaX, int deltaY) override;
            void OnMouseClickLeft(int x, int y, bool bDown) override;

            void MoveTo(int x, int y) override;

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

            bool IsDepressed() const override
            {
                return m_bDepressed;
            }

        protected:

            bool m_bHorizontal;

        };


    }
}
#endif // ifndef GWK_CONTROLS_SCROLLBARBAR_H
