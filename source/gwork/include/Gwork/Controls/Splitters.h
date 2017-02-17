/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_SPLITTERS_H
#define GWK_CONTROLS_SPLITTERS_H

#include <Gwork/Controls/Base.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT SplitterVertical : public Controls::Base
        {
            GWK_CONTROL_INLINE(SplitterVertical, Controls::Base)
            {
                m_panels[0] = new Controls::Base(this);
                m_panels[1] = new Controls::Base(this);
                m_splitter = new Controls::SplitterBar(this);
                m_splitterSize = 6;
                m_splitter->SetPos(0, 100);
                m_splitter->SetCursor(Gwk::CursorType::SizeNS);
                m_splitter->onDragged.Add(this, &ThisClass::OnSplitterMoved);
                SetScaling(false, 100);
            }

            void PostLayout(Skin::Base* skin) override
            {
                RefreshContainers();
            }

            void SetPanels(Controls::Base* pA, Controls::Base* pB)
            {
                if (pA)
                    pA->SetParent(m_panels[0]);

                if (pB)
                    pB->SetParent(m_panels[1]);
            }

            void SetScaling(bool Right, int iSize)
            {
                m_rightSided = Right;
                m_size = iSize;
            }

            virtual int SplitterPos()
            {
                return m_splitter->Y();
            }

        protected:

            virtual void RefreshContainers()
            {
                const Gwk::Rect& inner = GetInnerBounds();
                int iOffset = m_size;

                if (m_rightSided)
                    iOffset = Height()-m_size;

                m_splitter->SetSize(inner.w, m_splitterSize);
                m_splitter->SetPos(0, iOffset);
                m_panels[0]->SetPos(inner.x, inner.y);
                m_panels[0]->SetSize(inner.w, iOffset);
                m_panels[1]->SetPos(inner.x, (iOffset+m_splitterSize));
                m_panels[1]->SetSize(inner.w, (inner.y+inner.h)-(iOffset+m_splitterSize));
            }

            virtual void OnSplitterMoved(Event::Info)
            {
                if (m_rightSided)
                    m_size = Height()-m_splitter->Y();
                else
                    m_size = m_splitter->Y();

                RefreshContainers();
                Invalidate();
            }

            Controls::Base*         m_panels[2];
            bool m_rightSided;
            int m_size;
            Controls::SplitterBar*  m_splitter;
            unsigned int m_splitterSize;
        };


        class GWK_EXPORT SplitterHorizontal : public SplitterVertical
        {
            GWK_CONTROL_INLINE(SplitterHorizontal, SplitterVertical)
            {
                m_splitter->SetCursor(Gwk::CursorType::SizeWE);
                m_splitter->SetPos(100, 0);
            }

            void RefreshContainers() override
            {
                const Gwk::Rect& inner = GetInnerBounds();
                int iOffset = m_size;

                if (m_rightSided)
                    iOffset = Width()-m_size;

                m_splitter->SetSize(m_splitterSize, inner.h);
                m_splitter->SetPos(iOffset, 0);
                m_panels[0]->SetPos(inner.x, inner.y);
                m_panels[0]->SetSize(iOffset, inner.h);
                m_panels[1]->SetPos((iOffset+m_splitterSize), inner.y);
                m_panels[1]->SetSize((inner.x+inner.w)-(iOffset+m_splitterSize), inner.h);
            }

            void OnSplitterMoved(Event::Info) override
            {
                if (m_rightSided)
                    m_size = Width()-m_splitter->X();
                else
                    m_size = m_splitter->X();

                RefreshContainers();
                Invalidate();
            }

            int SplitterPos() override
            {
                return m_splitter->X();
            }

        };


    }
}

#endif // ifndef GWK_CONTROLS_SPLITTERS_H
