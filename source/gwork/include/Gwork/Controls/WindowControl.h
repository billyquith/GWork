/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
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
            virtual void Render(Skin::Base* skin) override;
            virtual void RenderUnder(Skin::Base* skin) override;

            virtual void SetTitle(Gwk::String title);

            virtual void SetClosable(bool closeable);

            virtual void Touch() override;
            bool         IsOnTop() override;

            virtual void SetHidden(bool hidden) override;

            void CloseButtonPressed();
            void RenderFocus(Gwk::Skin::Base* skin) override;
            void SetDeleteOnClose(bool b)
            {
                m_bDeleteOnClose = b;
            }

            void MakeModal(bool bDrawBackground = true);
            void DestroyModal();

            Gwk::Event::Caller onWindowClosed;

        protected:

            Gwk::ControlsInternal::Dragger*    m_TitleBar;
            Gwk::Controls::Label*              m_Title;
            Gwk::Controls::WindowCloseButton*  m_CloseButton;

            bool m_bDeleteOnClose;

            ControlsInternal::Modal* m_Modal;
        };


    }
}
#endif // ifndef GWK_CONTROLS_WINDOWCONTROL_H
