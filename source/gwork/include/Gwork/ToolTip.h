/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */


#pragma once
#ifndef GWK_TOOLTIP_H
#define GWK_TOOLTIP_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>

namespace Gwk {
    namespace ToolTip
    {
        GWK_EXPORT bool TooltipActive();

        GWK_EXPORT void Enable(Controls::Base* control);
        GWK_EXPORT void Disable(Controls::Base* control);

        GWK_EXPORT void ControlDeleted(Controls::Base* control);

        GWK_EXPORT void RenderToolTip(Skin::Base* skin);
    }
}

#endif // ifndef GWK_TOOLTIP_H
