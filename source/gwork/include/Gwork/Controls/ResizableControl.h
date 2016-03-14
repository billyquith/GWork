/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_RESIZABLECONTROL_H
#define GWK_CONTROLS_RESIZABLECONTROL_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Controls/Dragger.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Controls/Resizer.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT ResizableControl : public Base
        {
        public:

            GWK_CONTROL(ResizableControl, Base);

            virtual void SetClampMovement(bool shouldClamp)
            {
                m_bClampMovement = shouldClamp;
            }

            virtual bool GetClampMovement()
            {
                return m_bClampMovement;
            }

            virtual void SetMinimumSize(const Gwk::Point& minSize)
            {
                m_minimumSize = minSize;
            }

            virtual Gwk::Point GetMinimumSize() override
            {
                return m_minimumSize;
            }

            virtual void DisableResizing();

            virtual bool SetBounds(int x, int y, int w, int h) override;

            virtual void OnResized()
            {
            }

            Event::Caller onResize;

            virtual ControlsInternal::Resizer* GetResizer(int iResizer)
            {
                return m_resizer[iResizer];
            }

        protected:

            void OnResizedInternal(Event::Info);

            Gwk::Point m_minimumSize;
            bool m_bClampMovement;
            bool m_bResizable;

            ControlsInternal::Resizer* m_resizer[10];


        };


    }
}
#endif // ifndef GWK_CONTROLS_RESIZABLECONTROL_H
