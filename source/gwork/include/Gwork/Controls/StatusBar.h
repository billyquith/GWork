/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
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
                Dock(Position::Bottom);
                SetPadding(Padding(2, 2, 2, 2));
                SetText("");
                SetAlignment(Position::Left|Position::CenterV);
            }

            virtual void AddControl(Controls::Base* ctrl, bool bRight)
            {
                ctrl->SetParent(this);
                ctrl->Dock(bRight ? Position::Right : Position::Left);
            }

            void Render(Skin::Base* skin) override
            {
                skin->DrawStatusBar(this);
            }

        };


    }
}
#endif // ifndef GWK_CONTROLS_STATUSBAR_H
