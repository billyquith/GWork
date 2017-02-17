/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_VERTICALSLIDER_H
#define GWK_CONTROLS_VERTICALSLIDER_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Controls/Dragger.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/Slider.h>


namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT VerticalSlider : public Slider
        {
            GWK_CONTROL(VerticalSlider, Slider);

            void Layout(Skin::Base* skin) override;
            void Render(Skin::Base* skin) override;

            float CalculateValue() override;
            void  UpdateBarFromValue() override;
            void  OnMouseClickLeft(int x, int y, bool bDown) override;

        };


    }
}
#endif // ifndef GWK_CONTROLS_VERTICALSLIDER_H
