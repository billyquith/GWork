/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
*  See license in Gwork.h
 */

#ifndef GWK_RENDERERS_OPENGL_DEBUGFONT_H
#define GWK_RENDERERS_OPENGL_DEBUGFONT_H

#include <Gwork/Gwork.h>
#include <Gwork/Renderers/OpenGL.h>

namespace Gwk
{
    namespace Renderer
    {
        class OpenGL_DebugFont : public Gwk::Renderer::OpenGL
        {
        public:

            OpenGL_DebugFont();
            ~OpenGL_DebugFont();

            void Init();

            void RenderText(Gwk::Font* font, Gwk::Point pos,
                            const Gwk::String& text);
            Gwk::Point MeasureText(Gwk::Font* font, const Gwk::String& text);

        protected:

            void CreateDebugFont();
            void DestroyDebugFont();

            Gwk::Texture*  m_fontTexture;
            float m_fFontScale[2];
            float m_fLetterSpacing;

        };


    }
}
#endif // ifndef GWK_RENDERERS_OPENGL_DEBUGFONT_H
