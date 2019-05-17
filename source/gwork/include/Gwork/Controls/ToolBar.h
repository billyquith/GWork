/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
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
        class ToolBarButton : public ControlClass<ToolBarButton, Gwk::Controls::Button>
        {
        public:
            ToolBarButton(Gwk::Controls::Base *parent, const Gwk::String &name=""):BaseClass(parent, name)
            {
                SetSize(20, 20);
                Dock(Position::Left);
            }

            bool ShouldDrawBackground() override
            {
                return IsHovered();
            }

        };


        class ToolBarStrip : public ControlClass<ToolBarStrip, Base>
        {
            ToolBarStrip(Gwk::Controls::Base *parent, const Gwk::String &name=""):BaseClass(parent, name)
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
