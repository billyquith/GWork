/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_DRAGGER_H
#define GWK_CONTROLS_DRAGGER_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>


namespace Gwk
{
    namespace ControlsInternal
    {
        class GWK_EXPORT Dragger : public Controls::Base
        {
        public:

            GWK_CONTROL(Dragger, Controls::Base);

            virtual void OnMouseMoved(int x, int y, int deltaX, int deltaY) override;

            virtual void OnMouseClickLeft(int x, int y, bool bDown) override;
            virtual void Render(Skin::Base* skin) override;

            virtual void SetTarget(Controls::Base* base)
            {
                m_target = base;
            }

            virtual bool IsDepressed()
            {
                return m_bDepressed;
            }

            virtual void SetDoMove(bool b)
            {
                m_bDoMove = b;
            }

            virtual void OnMouseDoubleClickLeft(int x, int y) override;

            Gwk::Event::Listener onDragged;
            Gwk::Event::Listener onDragStart;

            Gwk::Event::Listener onDoubleClickLeft;

        protected:

            bool m_bDepressed;
            Gwk::Point m_holdPos;
            Controls::Base*     m_target;
            bool m_bDoMove;
        };


    }
}
#endif // ifndef GWK_CONTROLS_DRAGGER_H
