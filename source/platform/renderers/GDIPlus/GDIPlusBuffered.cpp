
#include <windows.h>
#include <gdiplus.h>

#include <Gwork/Renderers/GDIPlus.h>

#include <Gwork/Utility.h>
#include <Gwork/Font.h>
#include <Gwork/Texture.h>

namespace Gwk
{
    namespace Renderer
    {
        GDIPlusBuffered::GDIPlusBuffered(HWND hWND) : GDIPlus(hWND)
        {
            m_bitmap = nullptr;
        }

        GDIPlusBuffered::~GDIPlusBuffered()
        {
            DestroyBackbuffer();
        }

        void GDIPlusBuffered::CreateBackbuffer()
        {
            // Check the Client Gwk::Rect size (of supplied HWND)
            // If it has changed, we need to recreate the backbuffer
            {
                RECT rect;
                GetClientRect(m_hWND, &rect);
                int width = rect.right-rect.left;
                int height = rect.bottom-rect.top;

                if (m_width != width)
                    DestroyBackbuffer();

                if (m_height != height)
                    DestroyBackbuffer();

                m_width = width;
                m_height = height;
            }

            if (m_bitmap)
                return;

            Gdiplus::Graphics gfx(m_hDC);
            m_bitmap = new Gdiplus::Bitmap(m_width, m_height, &gfx);
            graphics = Gdiplus::Graphics::FromImage(m_bitmap);
        }

        void GDIPlusBuffered::DestroyBackbuffer()
        {
            if (m_bitmap)
            {
                delete m_bitmap;
                m_bitmap = nullptr;
            }

            if (graphics)
            {
                delete graphics;
                graphics = nullptr;
            }
        }

        void GDIPlusBuffered::Begin()
        {
            m_hDC = GetDC(m_hWND);
            CreateBackbuffer();
        }

        void GDIPlusBuffered::End()
        {
            Gdiplus::Graphics gfx(m_hDC);
            gfx.DrawImage(m_bitmap, 0, 0);
            ReleaseDC(m_hWND, m_hDC);
            m_hDC = nullptr;
        }

    }
}
