/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2015-2016 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_WINDOWCANVAS_H
#define GWK_CONTROLS_WINDOWCANVAS_H

#include "Gwork/Controls/Base.h"
#include "Gwork/InputHandler.h"
#include "Gwork/WindowProvider.h"
#include "Gwork/Controls/Dragger.h"
#include "Gwork/Controls/Label.h"
#include "Gwork/Controls/WindowButtons.h"

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT WindowCanvas : public Canvas, public Gwk::WindowProvider
        {
        public:

            GWK_CLASS(WindowCanvas, Controls::Canvas);

            WindowCanvas(int x, int y, int w, int h, Gwk::Skin::Base* pRenderer,
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
                m_Sizer->SetHidden(!b);
            }

            virtual bool GetSizable()
            {
                return m_Sizer->Visible();
            }

        protected:

            virtual void RenderCanvas();
            virtual void DestroyWindow();

            virtual void CloseButtonPressed();
            virtual void MaximizeButtonPressed();
            virtual void MinimizeButtonPressed();

            virtual void Dragger_Start();
            virtual void Dragger_Moved();
            virtual void Sizer_Moved();
            virtual void OnTitleDoubleClicked();

            void*       m_pOSWindow;
            bool m_bQuit;

            Gwk::Skin::Base*           m_pSkinChange;

            ControlsInternal::Dragger*  m_TitleBar;
            ControlsInternal::Dragger*  m_Sizer;
            Gwk::Controls::Label*      m_Title;


            Gwk::Point m_WindowPos;
            Gwk::Point m_HoldPos;

            bool m_bCanMaximize;
            bool m_bIsMaximized;

            Gwk::Controls::WindowCloseButton*      m_pClose;
            Gwk::Controls::WindowMaximizeButton*   m_pMaximize;
            Gwk::Controls::WindowMinimizeButton*   m_pMinimize;

        };


    }
}
#endif // ifndef GWK_CONTROLS_WINDOWCANVAS_H
