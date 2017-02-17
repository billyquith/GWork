/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_MENUSTRIP_H
#define GWK_CONTROLS_MENUSTRIP_H

#include <Gwork/BaseRender.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Menu.h>
#include <Gwork/Controls/MenuItem.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT MenuStrip : public Menu
        {
            GWK_CONTROL(MenuStrip, Menu);

            void Render(Skin::Base* skin) override;
            void RenderUnder(Skin::Base* /*skin*/) override {}

            void Layout(Skin::Base* skin) override;

        protected:

            void OnAddItem(MenuItem* item) override;
            bool ShouldHoverOpenMenu() override;
            void Close() override {}

        };


    }

}
#endif // ifndef GWK_CONTROLS_MENUSTRIP_H
