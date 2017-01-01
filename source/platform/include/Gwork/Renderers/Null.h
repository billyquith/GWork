/*
 *  Gwork
 *  Copyright (c) 2013-17 Nick Trout
*  See license in Gwork.h
 */

#pragma once

#ifndef GWK_RENDERERS_NULL_H
#define GWK_RENDERERS_NULL_H

#include <Gwork/BaseRender.h>

namespace Gwk
{
    namespace Renderer
    {
        class Null : public Gwk::Renderer::Base
        {
        public:

            Null();
            ~Null();

            void LoadFont(Gwk::Font* font) override;
            void FreeFont(Gwk::Font* font) override;
            
            void        LoadTexture(Gwk::Texture* texture) override;
            void        FreeTexture(Gwk::Texture* texture) override;

        };
    }
}
#endif // GWK_RENDERERS_NULL_H
