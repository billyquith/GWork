/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_RESIZER_H
#define GWK_CONTROLS_RESIZER_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/Dragger.h>


namespace Gwk
{
    namespace ControlsInternal
    {
        class GWK_EXPORT Resizer : public Dragger
        {
        public:

            GWK_CONTROL(Resizer, Dragger);

            virtual void OnMouseMoved(int x, int y, int deltaX, int deltaY) override;
            virtual void SetResizeDir(Position dir);

            Event::Listener onResize;

        protected:

            Position m_resizeDir;

        };


    }
}
#endif // ifndef GWK_CONTROLS_RESIZER_H
