/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_SCROLLBARBOTTON_H
#define GWK_CONTROLS_SCROLLBARBOTTON_H

#include <Gwork/Controls/Button.h>

namespace Gwk
{
    namespace ControlsInternal
    {
        class GWK_EXPORT ScrollBarButton : public Controls::Button
        {
        public:

            GWK_CONTROL(ScrollBarButton, Controls::Button);

            void Render(Skin::Base* skin) override;

            void SetDirectionUp();
            void SetDirectionDown();
            void SetDirectionLeft();
            void SetDirectionRight();

        protected:

            Position m_direction;
            
        };

    }
}
#endif // ifndef GWK_CONTROLS_SCROLLBARBOTTON_H
