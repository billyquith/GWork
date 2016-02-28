/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_DRAGANDDROP_H
#define GWK_DRAGANDDROP_H

#include <sstream>

#include "Gwork/Skin.h"
#include "Gwork/Structures.h"

namespace Gwk
{
    namespace DragAndDrop
    {
        extern GWK_EXPORT Package* CurrentPackage;
        extern GWK_EXPORT Gwk::Controls::Base*    SourceControl;
        extern GWK_EXPORT Gwk::Controls::Base*    HoveredControl;

        bool GWK_EXPORT Start(Gwk::Controls::Base* pControl, Package* pPackage);

        bool GWK_EXPORT OnMouseButton(Gwk::Controls::Base* pHoveredControl, int x, int y,
                                       bool bDown);
        void GWK_EXPORT OnMouseMoved(Gwk::Controls::Base* pHoveredControl, int x, int y);

        void GWK_EXPORT RenderOverlay(Gwk::Controls::Canvas* pCanvas, Skin::Base* skin);

        void GWK_EXPORT ControlDeleted(Gwk::Controls::Base* pControl);
    }

}
#endif // ifndef GWK_DRAGANDDROP_H
