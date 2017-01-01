/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_SKINS_TEXTURING_H
#define GWK_SKINS_TEXTURING_H

#include <Gwork/Gwork.h>

namespace Gwk
{
    namespace Skin
    {
        namespace Texturing
        {
            //! Handle drawing a single textured rectangle.
            //
            struct Single
            {
                Single()
                :   m_texture(nullptr)
                {}

                void Init( Texture* texture, float x, float y, float w, float h )
                {
                    m_texture = texture;
                    float const texw = texture->width;
                    float const texh = texture->height;
                    m_uv[0] = x / texw;
                    m_uv[1] = y / texh;
                    m_uv[2] = ( x + w ) / texw;
                    m_uv[3] = ( y + h ) / texh;
                    m_width = w;
                    m_height = h;
                }

                void Draw( Gwk::Renderer::Base* render, Gwk::Rect r, const Gwk::Color & col = Gwk::Colors::White )
                {
                    if ( !m_texture )
                        return;

                    render->SetDrawColor( col );
                    render->DrawTexturedRect( m_texture, r, m_uv[0], m_uv[1], m_uv[2], m_uv[3] );
                }

                void DrawCenter( Gwk::Renderer::Base* render, Gwk::Rect r, const Gwk::Color & col = Gwk::Colors::White )
                {
                    if ( !m_texture )
                        return;

                    r.x += ( r.w - m_width )/2;
                    r.y += ( r.h - m_height )/2;
                    r.w = m_width;
                    r.h = m_height;
                    Draw( render, r, col );
                }


                Texture*    m_texture;  //!< Texture to use.
                float       m_uv[4];      //!< Rectangle texture UVs: (TL.xy, BR.xy).
                int         m_width;     //!< Width of draw rectangle.
                int         m_height;    //!< Height of draw rectangle.
            };

            //! Handle drawing a skinned textured rectangle. This consists of nine areas:
            //!
            //!  || Areas: ||
            //!  | :-: | :-: | :-: |
            //!  |  0  |  1  |  2  |
            //!  |  3  |  4  |  5  |
            //!  |  6  |  7  |  8  |
            //!
            //
            struct Bordered
            {
                Bordered()
                :   m_texture(nullptr)
                {}

                void Init( Texture* texture, float x, float y, float w, float h, Margin in_margin, float DrawMarginScale = 1.0f )
                {
                    m_texture = texture;
                    m_margin = in_margin;
                    SetRect( 0, x, y, m_margin.left, m_margin.top );
                    SetRect( 1, x + m_margin.left, y, w - m_margin.left - m_margin.right, m_margin.top );
                    SetRect( 2, ( x + w ) - m_margin.right, y, m_margin.right, m_margin.top );
                    SetRect( 3, x, y + m_margin.top, m_margin.left, h - m_margin.top - m_margin.bottom );
                    SetRect( 4, x + m_margin.left,  y + m_margin.top, w - m_margin.left - m_margin.right, h - m_margin.top - m_margin.bottom );
                    SetRect( 5, ( x + w ) - m_margin.right,  y + m_margin.top, m_margin.right, h - m_margin.top - m_margin.bottom );
                    SetRect( 6, x, ( y + h ) - m_margin.bottom, m_margin.left, m_margin.bottom );
                    SetRect( 7, x + m_margin.left, ( y + h ) - m_margin.bottom, w - m_margin.left - m_margin.right, m_margin.bottom );
                    SetRect( 8, ( x + w ) - m_margin.right, ( y + h ) - m_margin.bottom, m_margin.right, m_margin.bottom );
                    m_margin.left *= DrawMarginScale;
                    m_margin.right *= DrawMarginScale;
                    m_margin.top *= DrawMarginScale;
                    m_margin.bottom *= DrawMarginScale;
                    m_width = w - x;
                    m_height = h - y;
                }

                void SetRect( int iNum, float x, float y, float w, float h )
                {
                    float const texw = m_texture->width;
                    float const texh = m_texture->height;
                    m_rects[iNum].m_uv[0] = x / texw;
                    m_rects[iNum].m_uv[1] = y / texh;
                    m_rects[iNum].m_uv[2] = ( x + w ) / texw;
                    m_rects[iNum].m_uv[3] = ( y + h ) / texh;
                }

                //
                //! Draw a segmented, textured rectangle for skinning.
                //!
                //! \param render - Renderer to use.
                //! \param r - Rectangle to draw; outer edge.
                //! \param col - Color. Tint?
                //! \param draw - Bitfield specifying segments to draw.
                //
                void Draw( Gwk::Renderer::Base* render, Gwk::Rect r,
                           const Gwk::Color & col = Gwk::Colors::White, unsigned int draw = ~0 )
                {
                    if (!m_texture)
                        return;

                    render->SetDrawColor( col );

                    if (r.w < m_width && r.h < m_height)
                    {
                        render->DrawTexturedRect(m_texture, r,
                                                 m_rects[0].m_uv[0], m_rects[0].m_uv[1], m_rects[8].m_uv[2], m_rects[8].m_uv[3]);
                        return;
                    }

                    if (draw & (1<<0))
                        DrawRect( render, 0, r.x, r.y, m_margin.left, m_margin.top );

                    if (draw & (1<<1))
                        DrawRect( render, 1, r.x + m_margin.left, r.y, r.w - m_margin.left - m_margin.right, m_margin.top );

                    if (draw & (1<<2))
                        DrawRect( render, 2, ( r.x + r.w ) - m_margin.right, r.y, m_margin.right, m_margin.top );

                    if (draw & (1<<3))
                        DrawRect( render, 3, r.x, r.y + m_margin.top, m_margin.left, r.h - m_margin.top - m_margin.bottom );

                    if (draw & (1<<4))
                        DrawRect( render, 4, r.x + m_margin.left, r.y + m_margin.top,
                                             r.w - m_margin.left - m_margin.right, r.h - m_margin.top - m_margin.bottom );
                    
                    if (draw & (1<<5))
                        DrawRect( render, 5, ( r.x + r.w ) - m_margin.right, r.y + m_margin.top, m_margin.right, r.h - m_margin.top - m_margin.bottom );

                    if (draw & (1<<6))
                        DrawRect( render, 6, r.x, ( r.y + r.h ) - m_margin.bottom, m_margin.left, m_margin.bottom );

                    if (draw & (1<<7))
                        DrawRect( render, 7, r.x + m_margin.left, ( r.y + r.h ) - m_margin.bottom, r.w - m_margin.left - m_margin.right, m_margin.bottom );

                    if (draw & (1<<8))
                        DrawRect( render, 8, ( r.x + r.w ) - m_margin.right, ( r.y + r.h ) - m_margin.bottom, m_margin.right, m_margin.bottom );
                }

                void DrawRect( Gwk::Renderer::Base* render, int i, int x, int y, int w, int h )
                {
                    render->DrawTexturedRect( m_texture,
                                              Gwk::Rect( x, y, w, h ),
                                              m_rects[i].m_uv[0], m_rects[i].m_uv[1], m_rects[i].m_uv[2], m_rects[i].m_uv[3] );
                }

                Texture *m_texture;

                struct SubRect
                {
                    float m_uv[4];
                };

                SubRect m_rects[9];
                Margin m_margin;
                float m_width;
                float m_height;
            };
        }
    }
}
#endif
