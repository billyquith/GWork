/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_LABELCLICKABLE_H
#define GWK_CONTROLS_LABELCLICKABLE_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Button.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT LabelClickable : public Button
        {
        public:

            GWK_CONTROL(LabelClickable, Button);

            void Render(Skin::Base* skin) override;

        };


    }
}
#endif // ifndef GWK_CONTROLS_LABELCLICKABLE_H
