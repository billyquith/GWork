/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_WINDOWS_BUTTONS_H
#define GWK_CONTROLS_WINDOWS_BUTTONS_H

#include <Gwork/Gwork.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Skin.h>


namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT WindowCloseButton : public Button
        {
            GWK_CONTROL_INLINE(WindowCloseButton, Button)
            {
                m_window = nullptr;
                SetSize(31, 31);
                SetText("");
            }

            void Render(Skin::Base* skin) override
            {
                if (!m_window)
                    return;

                skin->DrawWindowCloseButton(this,
                                            IsDepressed() && IsHovered(),
                                            IsHovered() && ShouldDrawHover(),
                                            IsDisabled());
            }

            void SetWindow(Gwk::Controls::Base* p)
            {
                m_window = p;
            }

        protected:

            Controls::Base* m_window;
        };


        class GWK_EXPORT WindowMaximizeButton : public WindowCloseButton
        {
            GWK_CONTROL_INLINE(WindowMaximizeButton, WindowCloseButton)
            {
                m_bMaximized = false;
            }

            void Render(Skin::Base* skin) override
            {
                if (!m_window)
                    return;

                skin->DrawWindowMaximizeButton(this,
                                               IsDepressed() && IsHovered(),
                                               IsHovered() && ShouldDrawHover(),
                                               IsDisabled(),
                                               m_bMaximized);
            }

            virtual void SetMaximized(bool b)
            {
                m_bMaximized = b;
            }

        protected:

            bool m_bMaximized;
        };


        class GWK_EXPORT WindowMinimizeButton : public WindowCloseButton
        {
            GWK_CONTROL_INLINE(WindowMinimizeButton, WindowCloseButton)
            {
            }

            void Render(Skin::Base* skin) override
            {
                if (!m_window)
                    return;

                skin->DrawWindowMinimizeButton(this,
                                               IsDepressed() && IsHovered(),
                                               IsHovered() && ShouldDrawHover(),
                                               IsDisabled());
            }

        };


    }
}

#endif // ifndef GWK_CONTROLS_WINDOWS_BUTTONS_H
