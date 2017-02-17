/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_SPLITTERBAR_H
#define GWK_CONTROLS_SPLITTERBAR_H

#include <Gwork/Gwork.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Dragger.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT SplitterBar : public ControlsInternal::Dragger
        {
        public:

            GWK_CONTROL(SplitterBar, ControlsInternal::Dragger);

            void Layout(Skin::Base* skin) override;
        };


    }
}

#endif // ifndef GWK_CONTROLS_SPLITTERBAR_H
