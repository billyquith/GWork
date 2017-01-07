/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_BASERENDER_H
#define GWK_BASERENDER_H

#include <Gwork/PlatformTypes.h>

namespace Gwk
{    
    struct Font;
    struct Texture;
    class WindowProvider;

    namespace Renderer
    {
        class Base;

        class ICacheToTexture
        {
        public:
            virtual ~ICacheToTexture() {}
            
            typedef void* CacheHandle;

            virtual void Initialize() = 0;
            virtual void ShutDown() = 0;
            virtual void SetupCacheTexture(CacheHandle control) = 0;
            virtual void FinishCacheTexture(CacheHandle control) = 0;
            virtual void DrawCachedControlTexture(CacheHandle control) = 0;
            virtual void CreateControlCacheTexture(CacheHandle control, const Point& size) = 0;
            virtual void UpdateControlCacheTexture(CacheHandle control) = 0;
            virtual void SetRenderer(Gwk::Renderer::Base* renderer) = 0;

        };

        //
        /// Base class for all renderer implementations.
        ///
        /// @note We never instance this directly, only the derived implementations.
        //
        class GWK_EXPORT Base
        {
        protected:
            
            Base(); // We only instance subclasses of this.

        public:

            virtual ~Base();

            virtual void Init()         {}
            virtual void Begin()        {}
            virtual void End()          {}

            virtual void SetDrawColor(Color color) {}

            virtual void DrawFilledRect(Gwk::Rect rect) {}

            virtual void StartClip()    {}
            virtual void EndClip()      {}

            virtual void LoadTexture(Gwk::Texture* texture)   {}
            virtual void FreeTexture(Gwk::Texture* texture)   {}

            virtual void DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect targetRect,
                                          float u1 = 0.0f, float v1 = 0.0f,
                                          float u2 = 1.0f, float v2 = 1.0f) {}

            virtual void DrawMissingImage(Gwk::Rect targetRect);

            virtual Gwk::Color PixelColor(Gwk::Texture* texture,
                                           unsigned int x, unsigned int y,
                                           const Gwk::Color& col_default =
                                                                Gwk::Color(255,255,255,255))
            {
                return col_default;
            }

            virtual ICacheToTexture* GetCTT()
            {
                return m_RTT;
            }

            virtual void LoadFont(Gwk::Font* font)    {}
            virtual void FreeFont(Gwk::Font* font)    {}

            virtual void RenderText(Gwk::Font* font,
                                    Gwk::Point pos,
                                    const Gwk::String& text);
            
            virtual Gwk::Point MeasureText(Gwk::Font* font,
                                           const Gwk::String& text);

            /// \sect{Render Specialisation}
            ///     No need to implement these functions in your derived class, but
            ///     if you can do them faster than the default implementation it's a
            ///     good idea to.
            //
            virtual void DrawLinedRect(Gwk::Rect rect);
            virtual void DrawPixel(int x, int y);
            virtual void DrawShavedCornerRect(Gwk::Rect rect, bool bSlight = false);
            /// \}

            /// \sect{Translate}
            ///     Translate a panel's local drawing coordinate
            ///     into view space, taking Offsets into account.
            //
            void Translate(int& x, int& y);
            void Translate(Gwk::Rect& rect);
            /// \}

            //
            /// Set the rendering offset. You shouldn't have to touch these, ever.
            //
            void SetRenderOffset(const Gwk::Point& offset)
            {
                m_renderOffset = offset;
            }

            void AddRenderOffset(const Gwk::Rect& offset)
            {
                m_renderOffset.x += offset.x;
                m_renderOffset.y += offset.y;
            }

            const Gwk::Point& GetRenderOffset() const
            {
                return m_renderOffset;
            }

            void                SetClipRegion(Gwk::Rect rect);
            void                AddClipRegion(Gwk::Rect rect);
            bool                ClipRegionVisible();
            const Gwk::Rect&    ClipRegion() const;

            void SetScale(float fScale)     { m_fScale = fScale; }
            float Scale() const             { return m_fScale; }

            //
            // Self Initialization, shutdown
            //
            virtual bool InitializeContext(Gwk::WindowProvider* window)
            {
                return false;
            }

            virtual bool ShutdownContext(Gwk::WindowProvider* window)
            {
                return false;
            }

            virtual bool ResizedContext(Gwk::WindowProvider* window, int w, int h)
            {
                return false;
            }

            virtual bool BeginContext(Gwk::WindowProvider* window)
            {
                return false;
            }

            virtual bool EndContext(Gwk::WindowProvider* window)
            {
                return false;
            }

            virtual bool PresentContext(Gwk::WindowProvider* window)
            {
                return false;
            }
            
        protected:
            
            float m_fScale;
            
        private:
            
            Gwk::Point m_renderOffset;
            
            Gwk::Rect m_rectClipRegion;
            ICacheToTexture* m_RTT;            
        };

    }
}
#endif // ifndef GWK_BASERENDER_H
