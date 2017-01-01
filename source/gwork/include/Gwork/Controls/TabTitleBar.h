/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_TABTITLEBAR_H
#define GWK_CONTROLS_TABTITLEBAR_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Controls/TabButton.h>
#include <Gwork/DragAndDrop.h>
#include <Gwork/Skin.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT TabTitleBar : public Label
        {
        public:

            GWK_CONTROL_INLINE(TabTitleBar, Label)
            {
                SetMouseInputEnabled(true);
                SetTextPadding(Gwk::Padding(5, 2, 5, 2));
                SetPadding(Gwk::Padding(1, 2, 1, 2));
                DragAndDrop_SetPackage(true, "TabWindowMove");
            }

            void Render(Skin::Base* skin) override
            {
                skin->DrawTabTitleBar(this);
            }

            void DragAndDrop_StartDragging(Gwk::DragAndDrop::Package* package, int x, int y) override
            {
                DragAndDrop::SourceControl = GetParent();
                DragAndDrop::SourceControl->DragAndDrop_StartDragging(package, x, y);
            }

            void UpdateFromTab(TabButton* button)
            {
                SetText(button->GetText());
                SizeToContents();
            }

        private:

        };


    }
}
#endif // ifndef GWK_CONTROLS_TABTITLEBAR_H
