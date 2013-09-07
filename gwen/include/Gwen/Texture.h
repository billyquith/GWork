/*
 *  GWEN
 *  Copyright (c) 2010 Facepunch Studios
 *  See license in Gwen.h
 */

#pragma once
#ifndef GWEN_TEXTURE_H
#define GWEN_TEXTURE_H

#include <string>

#include "Gwen/BaseRender.h"

namespace Gwen
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

        void Load(const String& str, Gwen::Renderer::Base* render, bool needReadable=false)
        {
            name = str;
            Gwen::Debug::AssertCheck(render != NULL, "No renderer!");
            
            readable = needReadable;
            render->LoadTexture(this);
        }

        void Release(Gwen::Renderer::Base* render)
        {
            render->FreeTexture(this);
        }

        bool FailedToLoad() const
        {
            return failed;
        }

    };


}
#endif // ifndef GWEN_TEXTURE_H
