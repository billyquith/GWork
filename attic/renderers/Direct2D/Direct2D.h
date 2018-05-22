/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#ifndef GWK_RENDERERS_DIRECT2D_H
#define GWK_RENDERERS_DIRECT2D_H

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include <Gwork/BaseRender.h>

namespace Gwk
{
    namespace Renderer
    {
        //! Default resource loader for Direct2D.
        class Direct2DResourceLoader : public ResourceLoader
        {
            ResourcePaths& m_paths;
            ID2D1RenderTarget* m_rT;
            IDWriteFactory* m_dWriteFactory;
            IWICImagingFactory* m_wICFactory;

            Gwk::Texture::List m_textureList, m_texturesLost;
            Gwk::Font::List m_fontList;

        public:
            Direct2DResourceLoader(ResourcePaths& paths,
                                   ID2D1RenderTarget* rT,
                                   IDWriteFactory* dWriteFactory,
                                   IWICImagingFactory* wICFactory)
                :   m_paths(paths)
                ,   m_rT(rT)
                ,   m_dWriteFactory(dWriteFactory)
                ,   m_wICFactory(wICFactory)
            {}

            Font::Status LoadFont(Font& font) override;
            void FreeFont(Font& font) override;

            Texture::Status LoadTexture(Texture& texture) override;
            void FreeTexture(Texture& texture) override;

            void Notify(NotificationType msg) override;
        };

        //
        //! Renderer for [Direct2D](https://msdn.microsoft.com/en-us/library/windows/desktop/dd370990(v=vs.85).aspx).
        //
        class GWK_EXPORT Direct2D : public Gwk::Renderer::Base
        {
        public:

            Direct2D(ResourceLoader& loader, ID2D1RenderTarget* device, IDWriteFactory* dWriteFactory);
            virtual ~Direct2D();

            void Begin() override;
            void End() override;
            virtual void Release();

            void SetDrawColor(Gwk::Color color) override;

            void DrawFilledRect(Gwk::Rect rect) override;

            void RenderText(Gwk::Font* font, Gwk::Point pos, const Gwk::String& text) override;
            Gwk::Point MeasureText(Gwk::Font* font, const Gwk::String& text) override;

            virtual void DeviceLost();
            virtual void DeviceAcquired(ID2D1RenderTarget* rT);

            void StartClip();
            void EndClip();

            void DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect targetRect, float u1 = 0.0f,
                                  float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f) override;

            Gwk::Color PixelColor(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                  const Gwk::Color& col_default) override;

            void DrawLinedRect(Gwk::Rect rect) override;
            void DrawShavedCornerRect(Gwk::Rect rect, bool bSlight = false) override;

            bool InitializeContext(Gwk::WindowProvider* window);
            bool ShutdownContext(Gwk::WindowProvider* window);
            bool PresentContext(Gwk::WindowProvider* window);
            bool ResizedContext(Gwk::WindowProvider* window, int w, int h);
            bool BeginContext(Gwk::WindowProvider* window);
            bool EndContext(Gwk::WindowProvider* window);

        private:

            bool InternalCreateDeviceResources();
            void InternalReleaseDeviceResources();

            ID2D1Factory*   m_d2DFactory;
            HWND m_hWND;

            ID2D1RenderTarget*  m_rT;
            IDWriteFactory*     m_dWriteFactory;

            ID2D1SolidColorBrush* m_solidColorBrush;

            D2D1::ColorF m_color;
        };

    }
}
#endif // ifndef GWK_RENDERERS_DIRECT2D_H
