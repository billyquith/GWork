/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_TOOLBAR_H
#define GWK_CONTROLS_TOOLBAR_H

#include <Gwork/BaseRender.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/Skin.h>

namespace Gwk
{
    namespace Controls
    {
        class ToolBarButton : public Gwk::Controls::Button
        {
            GWK_CONTROL_INLINE(ToolBarButton, Gwk::Controls::Button)
            {
                SetSize(20, 20);
                Dock(Position::Left);
            }

            bool ShouldDrawBackground() override
            {
                return IsHovered();
            }

        };


        class ToolBarStrip : public Base
        {
            GWK_CONTROL_INLINE(ToolBarStrip, Base)
            {
                SetSize(25, 25);
                SetPadding(Padding(2, 2, 2, 2));
            }

            void Render(Skin::Base* skin) override
            {
                skin->DrawMenuStrip(this);
            }

            virtual ToolBarButton* Add(const String& text, const String& icon)
            {
                ToolBarButton* button = new ToolBarButton(this);
                button->SetTooltipText(text);
                button->SetImage(icon);
                return button;
            }

        };


    }

}
#endif // ifndef GWK_CONTROLS_TOOLBAR_H
