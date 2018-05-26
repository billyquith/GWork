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
            Null();
            virtual ~Null();
        };
    }
}
#endif // GWK_RENDERERS_NULL_H
