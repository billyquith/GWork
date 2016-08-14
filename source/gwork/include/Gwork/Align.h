/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_ALIGN_H
#define GWK_ALIGN_H
#include <Gwork/Controls/Base.h>

namespace Gwk
{
    struct Align
    {
        static inline void PlaceBelow(Controls::Base* ctrl, Controls::Base* below, int iBorder = 0)
        {
            ctrl->SetPos(ctrl->X(), below->Bottom()+iBorder);
        }

    };
}
#endif // ifndef GWK_ALIGN_H
