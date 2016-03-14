/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2015-2016 Billy Quith
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

            virtual void DoThink();

            virtual bool WantsQuit()
            {
                return m_bQuit;
            }

            // Gwk::WindowProvider
            virtual void* GetWindow();

            virtual bool InputQuit();

            Skin::Base* GetSkin(void);

            virtual void Render(Skin::Base* skin);

            virtual void SetPos(int x, int y);
            virtual bool IsOnTop();

            virtual void Layout(Skin::Base* skin);

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

            virtual void RenderCanvas();
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
