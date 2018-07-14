/*
 *  Gwork
 *  Copyright (c) 2013-2018 Billy Quith
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
        //
        //! \brief Null renderer that does not render!
        //!
        //! This is used for build and unit testing.
        //
        class GWK_EXPORT Null : public Gwk::Renderer::Base
        {
        public:
            Null(ResourcePaths& paths);
            virtual ~Null();

            
            Font::Status LoadFont(const Gwk::Font& font) final { return Font::Status::Loaded; }
            void FreeFont(const Gwk::Font& font) final {}
            bool EnsureFont(const Gwk::Font& font) final { return true; }

            Texture::Status LoadTexture(const Gwk::Texture& texture) final {  return Texture::Status::Loaded; }
            void FreeTexture(const Gwk::Texture& texture) final {}
            TextureData GetTextureData(const Gwk::Texture& texture) const final { return TextureData(); }
        };
    }
}
#endif // GWK_RENDERERS_NULL_H
