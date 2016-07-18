/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_CANVAS_H
#define GWK_CONTROLS_CANVAS_H

#include <Gwork/Controls/Base.h>
#include <Gwork/InputHandler.h>
#include <Gwork/InputEventListener.h>
#include <set>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT Canvas : public Base, public IInputEventListener
        {
        public:

            typedef Controls::Base ParentClass;

            Canvas(Skin::Base* skin);
            virtual ~Canvas();

            /// For additional initialization
            /// (which is sometimes not appropriate in the constructor)
            virtual void Initialize() {}

            /// You should call this to render your canvas.
            virtual void RenderCanvas();

            /// Call this whenever you want to process input. This
            /// is usually once a frame..
            virtual void DoThink();

            /// In most situations you will be rendering the canvas
            /// every frame. But in some situations you will only want
            /// to render when there have been changes. You can do this
            /// by checking NeedsRedraw().
            virtual bool NeedsRedraw() { return m_bNeedsRedraw; }

            virtual void Redraw() override { m_bNeedsRedraw = true; }

            // Internal. Do not call directly.
            virtual void Render(Skin::Base* render) override;

            /// Child panels call parent->GetCanvas() until they get to
            /// this top level function.
            virtual Controls::Canvas* GetCanvas() override
            {
                return this;
            }

            virtual void  SetScale(float f);
            virtual float Scale() const
            {
                return m_fScale;
            }

            virtual void OnBoundsChanged(Gwk::Rect oldBounds) override;

            /// Delete all children (this is done called in the destructor too)
            virtual void ReleaseChildren();

            /// Delayed deletes
            virtual void AddDelayedDelete(Controls::Base* control);
            virtual void ProcessDelayedDeletes();

            Controls::Base* FirstTab;
            Controls::Base* NextTab;

            /// \sect{Input}            
            bool InputMouseMoved(int x, int y, int deltaX, int deltaY) override;
            bool InputMouseButton(int iButton, bool bDown) override;            
            /// A modifier key was inputted.
            /// \param key - Key code
            /// \param down - Key state. Pressed?
            /// \return Event consumed?
            bool InputModifierKey(int key, bool down) override;
            /// A character was entered on the keyboard.
            /// \param chr - Unicode character code.
            /// \return Event consumed?
            bool InputCharacter(Gwk::UnicodeChar chr) override;
            
            bool InputMouseWheel(int val) override;
            bool InputQuit() override
            {
                return true;
            }
            /// \}

            // Background
            virtual void SetBackgroundColor(const Gwk::Color& color)
            {
                m_backgroundColor = color;
            }

            virtual void SetDrawBackground(bool bShouldDraw)
            {
                m_bDrawBackground = bShouldDraw;
            }

        protected:

            bool m_bNeedsRedraw;
            bool m_bAnyDelete;
            float m_fScale;

            Controls::Base::List m_deleteList;
            std::set<Controls::Base*> m_deleteSet;
            friend class Controls::Base;
            void PreDeleteCanvas(Controls::Base*);

            bool m_bDrawBackground;
            Gwk::Color m_backgroundColor;

        };


    }
}
#endif // ifndef GWK_CONTROLS_CANVAS_H
