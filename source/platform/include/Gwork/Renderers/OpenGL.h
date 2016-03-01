/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
*  See license in Gwork.h
 */

#ifndef GWK_RENDERERS_OPENGL_H
#define GWK_RENDERERS_OPENGL_H

#include <Gwork/Gwork.h>
#include <Gwork/BaseRender.h>

namespace Gwk
{
    namespace Renderer
    {
        class OpenGL : public Gwk::Renderer::Base
        {
        public:

            struct Vertex
            {
                float x, y, z;
                float u, v;
                unsigned char r, g, b, a;
            };


            OpenGL();
            ~OpenGL();

            virtual void Init();

            virtual void Begin();
            virtual void End();

            virtual void SetDrawColor(Gwk::Color color);
            virtual void DrawFilledRect(Gwk::Rect rect);

            void StartClip();
            void EndClip();

            void DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect targetRect, float u1 = 0.0f,
                                  float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);
            void        LoadTexture(Gwk::Texture* texture);
            void        FreeTexture(Gwk::Texture* texture);
            Gwk::Color PixelColour(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                    const Gwk::Color& col_default);

        protected:

            static const int MaxVerts = 1024;


            void Flush();
            void AddVert(int x, int y, float u = 0.0f, float v = 0.0f);

            Gwk::Color m_color;
            int m_vertNum;
            Vertex m_vertices[ MaxVerts ];

        public:

            //
            // Self Initialization
            //

            virtual bool InitializeContext(Gwk::WindowProvider* window);
            virtual bool ShutdownContext(Gwk::WindowProvider* window);
            virtual bool PresentContext(Gwk::WindowProvider* window);
            virtual bool ResizedContext(Gwk::WindowProvider* window, int w, int h);
            virtual bool BeginContext(Gwk::WindowProvider* window);
            virtual bool EndContext(Gwk::WindowProvider* window);

            void*   m_context;
        };


    }
}
#endif // ifndef GWK_RENDERERS_OPENGL_H
