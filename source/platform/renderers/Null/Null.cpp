/*
 *  Gwork
 *  Copyright (c) 2013-2016 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Renderers/Null.h>

namespace Gwk {
namespace Renderer {

Null::Null() {}

Null::~Null() {}
    
void Null::LoadFont(Gwk::Font* font)
{
}

void Null::FreeFont(Gwk::Font* font)
{
    if (font->data)
    {
        font->data = nullptr;
    }
}

void Null::LoadTexture(Gwk::Texture* texture)
{
}

void Null::FreeTexture(Gwk::Texture* texture)
{
}

} // Renderer
} // Gwork
