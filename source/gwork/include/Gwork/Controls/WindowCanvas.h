/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_WINDOWCANVAS_H
#define GWK_CONTROLS_WINDOWCANVAS_H

#include <Gwork/Controls/Base.h>
#include <Gwork/InputHandler.h>
#include <Gwork/WindowProvider.h>
#include <Gwork/Controls/Dragger.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Controls/WindowButtons.h>
#include <Gwork/Controls/Canvas.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT WindowCanvas : public Canvas, public Gwk::WindowProvider
        {
        public:

            GWK_CLASS(WindowCanvas, Controls::Canvas);

            WindowCanvas(int x, int y, int w, int h, Gwk::Skin::Base* renderer,
                         const Gwk::String& strWindowTitle = "");
            ~WindowCanvas();

            void DoThink() override;

            virtual bool WantsQuit()
            {
                return m_bQuit;
            }

            // Gwk::WindowProvider
            void* GetWindow() override;

            bool InputQuit() override;

            Skin::Base* GetSkin(void) override;

            void Render(Skin::Base* skin) override;

            void SetPos(int x, int y) override;
            bool IsOnTop() override;

            void Layout(Skin::Base* skin) override;

            virtual bool CanMaximize()
            {
                return m_bCanMaximize;
            }

            virtual void SetCanMaximize(bool b);
            virtual void SetMaximize(bool b);

            virtual void SetSizable(bool b)
            {
                m_sizer->SetHidden(!b);
            }

            virtual bool GetSizable()
            {
                return m_sizer->Visible();
            }

        protected:

            void RenderCanvas() override;
            virtual void DestroyWindow();

            virtual void CloseButtonPressed(Event::Info);
            virtual void MaximizeButtonPressed(Event::Info);
            virtual void MinimizeButtonPressed(Event::Info);

            virtual void Dragger_Start(Event::Info);
            virtual void Dragger_Moved(Event::Info);
            virtual void Sizer_Moved(Event::Info);
            virtual void OnTitleDoubleClicked(Event::Info);

            void*       m_oSWindowSWindow;
            bool m_bQuit;

            Gwk::Skin::Base*           m_skinChange;

            ControlsInternal::Dragger*  m_titleBar;
            ControlsInternal::Dragger*  m_sizer;
            Gwk::Controls::Label*      m_title;


            Gwk::Point m_windowPos;
            Gwk::Point m_holdPos;

            bool m_bCanMaximize;
            bool m_bIsMaximized;

            Gwk::Controls::WindowCloseButton*      m_close;
            Gwk::Controls::WindowMaximizeButton*   m_maximize;
            Gwk::Controls::WindowMinimizeButton*   m_minimize;

        };


    }
}
#endif // ifndef GWK_CONTROLS_WINDOWCANVAS_H
