/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_WINDOWCONTROL_H
#define GWK_CONTROLS_WINDOWCONTROL_H

#include <Gwork/Gwork.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Controls/Dragger.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Controls/ResizableControl.h>
#include <Gwork/Controls/Modal.h>
#include <Gwork/Controls/WindowButtons.h>
#include <Gwork/Skin.h>


namespace Gwk
{
    namespace Controls
    {
        class CloseButton;

        class GWK_EXPORT WindowControl : public ResizableControl
        {
        public:

            GWK_CONTROL(WindowControl, ResizableControl);

            virtual ~WindowControl();
            void Render(Skin::Base* skin) override;
            void RenderUnder(Skin::Base* skin) override;

            virtual void SetTitle(Gwk::String title);

            virtual void SetClosable(bool closeable);

            void Touch() override;
            bool         IsOnTop() override;

            void SetHidden(bool hidden) override;

            void RenderFocus(Gwk::Skin::Base* skin) override;
            void SetDeleteOnClose(bool b)
            {
                m_bDeleteOnClose = b;
            }

            void MakeModal(bool bDrawBackground = true);
            void DestroyModal();

            Gwk::Event::Listener onWindowClosed;

        protected:

            void CloseButtonPressed(Event::Info);
            
            Gwk::ControlsInternal::Dragger*    m_titleBar;
            Gwk::Controls::Label*              m_title;
            Gwk::Controls::WindowCloseButton*  m_closeButton;

            bool m_bDeleteOnClose;

            ControlsInternal::Modal* m_modal;
        };


    }
}
#endif // ifndef GWK_CONTROLS_WINDOWCONTROL_H
