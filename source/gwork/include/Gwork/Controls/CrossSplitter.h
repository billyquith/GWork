/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_CROSSSPLITTER_H
#define GWK_CONTROLS_CROSSSPLITTER_H

#include "Gwork/Gwork.h"
#include "Gwork/Controls/Base.h"
#include "Gwork/Controls/SplitterBar.h"

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT CrossSplitter : public Controls::Base
        {
        public:

            GWK_CONTROL(CrossSplitter, Controls::Base);

            void Layout(Skin::Base* skin) override;

            virtual float CalculateValueVertical();
            virtual float CalculateValueHorizontal();
            virtual void  CalculateValueCenter();
            virtual void  UpdateHSplitter();
            virtual void  UpdateVSplitter();
            virtual void  UpdateCSplitter();
            virtual void  OnVerticalMoved(Controls::Base* control);
            virtual void  OnHorizontalMoved(Controls::Base* control);
            virtual void  OnCenterMoved(Controls::Base* control);

            virtual void            SetPanel(int i, Controls::Base* pPanel);
            virtual Controls::Base* GetPanel(int i);

            virtual bool IsZoomed()
            {
                return m_iZoomedSection != -1;
            }

            virtual void Zoom(int section);
            virtual void UnZoom();
            virtual void ZoomChanged();
            virtual void CenterPanels()
            {
                m_fHVal = 0.5f;
                m_fVVal = 0.5f;
                Invalidate();
            }

            virtual void SetSplitterSize(int size)
            {
                m_fBarSize = size;
            }

        private:

            SplitterBar* m_VSplitter;
            SplitterBar* m_HSplitter;
            SplitterBar* m_CSplitter;

            Controls::Base* m_Sections[4];

            float m_fHVal;
            float m_fVVal;
            int m_fBarSize;

            char m_iZoomedSection;

            Gwk::Event::Caller onZoomed;
            Gwk::Event::Caller onUnZoomed;
            Gwk::Event::Caller onZoomChange;
        };


    }
}
#endif // ifndef GWK_CONTROLS_CROSSSPLITTER_H
