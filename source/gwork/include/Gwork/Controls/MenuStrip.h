/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_MENUSTRIP_H
#define GWK_CONTROLS_MENUSTRIP_H

#include "Gwork/BaseRender.h"
#include "Gwork/Controls/Base.h"
#include "Gwork/Controls/Menu.h"
#include "Gwork/Controls/MenuItem.h"

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT MenuStrip : public Menu
        {
            GWK_CONTROL(MenuStrip, Menu);

            virtual void Render(Skin::Base* skin) override;
            virtual void RenderUnder(Skin::Base* /*skin*/) override {}

            virtual void Layout(Skin::Base* skin) override;

        protected:

            virtual void OnAddItem(MenuItem* item) override;
            virtual bool ShouldHoverOpenMenu() override;
            virtual void Close() override {}

        };


    }

}
#endif // ifndef GWK_CONTROLS_MENUSTRIP_H
