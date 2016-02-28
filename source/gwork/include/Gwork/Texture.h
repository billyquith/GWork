/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_TEXTURE_H
#define GWK_TEXTURE_H

#include <string>

#include <Gwork/BaseRender.h>

namespace Gwk
{
    //
    // Texture
    //
    struct Texture
    {
        typedef std::list<Texture*> List;

        String  name;
        void*   data;
        bool    failed;
        int     width;
        int     height;
        
        bool    readable;
        void*   surface;

        Texture()
        :   data(NULL)
        ,   width(4)
        ,   height(4)
        ,   failed(false)
        ,   readable(false)
        ,   surface(NULL)
        {
        }

        ~Texture()
        {
        }

        void Load(const String& str, Gwk::Renderer::Base* render, bool needReadable=false)
        {
            name = str;
            Gwk::Debug::AssertCheck(render != NULL, "No renderer!");
            
            readable = needReadable;
            render->LoadTexture(this);
        }

        void Release(Gwk::Renderer::Base* render)
        {
            render->FreeTexture(this);
        }

        bool FailedToLoad() const
        {
            return failed;
        }

    };


}
#endif // ifndef GWK_TEXTURE_H
