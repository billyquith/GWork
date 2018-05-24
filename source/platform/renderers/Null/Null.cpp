/*
 *  Gwork
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Renderers/Null.h>

namespace Gwk {
namespace Renderer {

class NullResourceLoader : public ResourceLoader
{
public:
    Font::Status LoadFont(Font& font) final { return Font::Status::Loaded; }
    void FreeFont(Font& font) final {}

    Texture::Status LoadTexture(Texture& texture) final {  return Texture::Status::Loaded; }
    void FreeTexture(Texture& texture) final {}
};

static NullResourceLoader g_loader;

Null::Null()
    :   Base(g_loader)
{}

Null::~Null() {}

} // Renderer
} // Gwork
