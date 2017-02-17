/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_HIGHLIGHT_H
#define GWK_CONTROLS_HIGHLIGHT_H

#include <Gwork/BaseRender.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/Skin.h>

namespace Gwk
{
    namespace ControlsInternal
    {
        class GWK_EXPORT Highlight : public Controls::Base
        {
        public:

            GWK_CONTROL_INLINE(Highlight, Controls::Base)
            {
            }

            void Render(Skin::Base* skin) override
            {
                skin->DrawHighlight(this);
            }

        };


    }

}
#endif // ifndef GWK_CONTROLS_HIGHLIGHT_H
