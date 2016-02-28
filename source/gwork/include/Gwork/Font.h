/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_FONT_H
#define GWK_FONT_H

#include <string>
#include <list>

#include <Gwork/BaseRender.h>

namespace Gwk
{
    struct Font
    {
        typedef std::list<Font*> List;

        Font()
        :   data(NULL)
        ,   facename("Arial")
        ,   size(10)
        ,   realsize(0)
        ,   bold(false)
        {
        }

        String facename;
        float size;
        bool bold;

        /// This should be set by the renderer
        /// if it tries to use a font where it's
        /// NULL.
        void*   data;

        /// This is the real font size, after it's
        /// been scaled by Render->Scale()
        float realsize;

    };


} // namespace Gwk
#endif // ifndef GWK_FONT_H
