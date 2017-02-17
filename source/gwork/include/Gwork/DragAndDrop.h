/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_DRAGANDDROP_H
#define GWK_DRAGANDDROP_H

#include <sstream>

#include <Gwork/Skin.h>
#include <Gwork/UiTypes.h>

namespace Gwk
{
    namespace DragAndDrop
    {
        extern GWK_EXPORT Package* CurrentPackage;
        extern GWK_EXPORT Gwk::Controls::Base*    SourceControl;
        extern GWK_EXPORT Gwk::Controls::Base*    HoveredControl;

        bool GWK_EXPORT Start(Gwk::Controls::Base* control, Package* package);

        bool GWK_EXPORT OnMouseButton(Gwk::Controls::Base* hoveredControl, int x, int y,
                                       bool bDown);
        void GWK_EXPORT OnMouseMoved(Gwk::Controls::Base* hoveredControl, int x, int y);

        void GWK_EXPORT RenderOverlay(Gwk::Controls::Canvas* canvas, Skin::Base* skin);

        void GWK_EXPORT ControlDeleted(Gwk::Controls::Base* control);
    }

}
#endif // ifndef GWK_DRAGANDDROP_H
