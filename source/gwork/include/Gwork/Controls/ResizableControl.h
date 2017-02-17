/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
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
#include <array>

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

            Gwk::Point GetMinimumSize() override
            {
                return m_minimumSize;
            }

            virtual void DisableResizing();

            bool SetBounds(int x, int y, int w, int h) override;

            virtual void OnResized()
            {
            }

            Event::Listener onResize;
            
            virtual ControlsInternal::Resizer* GetResizer(int iResizer) // TODO - remove this?
            {
                return m_resizers[iResizer];
            }

        protected:

            void OnResizedInternal(Event::Info);

            Gwk::Point m_minimumSize;
            bool m_bClampMovement;
            bool m_bResizable;

            std::array<ControlsInternal::Resizer*, 8> m_resizers;
        };


    }
}
#endif // ifndef GWK_CONTROLS_RESIZABLECONTROL_H
