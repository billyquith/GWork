/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
*  See license in Gwork.h
 */

#ifndef GWK_RENDERERS_OPENGL_H
#define GWK_RENDERERS_OPENGL_H

#include <Gwork/BaseRender.h>

namespace Gwk
{
    namespace Renderer
    {
        class OpenGL : public Gwk::Renderer::Base
        {
        public:

            OpenGL(const Gwk::Rect& viewRect);
            virtual ~OpenGL();

            void Init() override;

            void Begin() override;
            void End() override;

            void SetDrawColor(Gwk::Color color) override;
            void DrawFilledRect(Gwk::Rect rect) override;

            void StartClip() override;
            void EndClip() override;

            void DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect targetRect, float u1 = 0.0f,
                                  float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f) override;
            void LoadTexture(Gwk::Texture* texture) override;
            void FreeTexture(Gwk::Texture* texture) override;
            
            Gwk::Color PixelColour(Gwk::Texture* texture,
                                   unsigned int x, unsigned int y,
                                   const Gwk::Color& col_default) override;

            void LoadFont(Gwk::Font* font) override;
            void FreeFont(Gwk::Font* font) override;
            
            void RenderText(Gwk::Font* font,
                            Gwk::Point pos,
                            const Gwk::String& text) override;
            
            Gwk::Point MeasureText(Gwk::Font* font, const Gwk::String& text) override;

        protected:

            Rect m_viewRect;
            Color m_color;
            
            static const int MaxVerts = 1024;
            struct Vertex
            {
                float x, y, z;
                float u, v;
                unsigned char r, g, b, a;
            };
            
            int m_vertNum;
            Vertex m_vertices[ MaxVerts ];

            void Flush();
            void AddVert(int x, int y, float u = 0.0f, float v = 0.0f);

        public:

            bool InitializeContext(Gwk::WindowProvider* window) override;
            bool ShutdownContext(Gwk::WindowProvider* window) override;
            bool PresentContext(Gwk::WindowProvider* window) override;
            bool ResizedContext(Gwk::WindowProvider* window, int w, int h) override;
            bool BeginContext(Gwk::WindowProvider* window) override;
            bool EndContext(Gwk::WindowProvider* window) override;

            void*   m_context;
        };


    }
}
#endif // ifndef GWK_RENDERERS_OPENGL_H
