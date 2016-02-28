/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_STATUSBAR_H
#define GWK_CONTROLS_STATUSBAR_H

#include <Gwork/Gwork.h>
#include <Gwork/Controls/Label.h>

namespace Gwk
{
    namespace Controls
    {
        class StatusBar : public Controls::Label
        {
        public:

            GWK_CONTROL_INLINE(StatusBar, Controls::Label)
            {
                SetHeight(22);
                Dock(Docking::Bottom);
                SetPadding(Padding(2, 2, 2, 2));
                SetText("");
                SetAlignment(Docking::Left|Docking::CenterV);
            }

            virtual void AddControl(Controls::Base* pCtrl, bool bRight)
            {
                pCtrl->SetParent(this);
                pCtrl->Dock(bRight ? Docking::Right : Docking::Left);
            }

            virtual void Render(Skin::Base* skin) override
            {
                skin->DrawStatusBar(this);
            }

        };


    }
}
#endif // ifndef GWK_CONTROLS_STATUSBAR_H
