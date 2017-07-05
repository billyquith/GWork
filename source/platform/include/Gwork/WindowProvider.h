/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_WINDOW_PROVIDER_H
#define GWK_WINDOW_PROVIDER_H

namespace Gwk
{
    class WindowProvider
    {
    public:

        virtual void* GetWindow() = 0;
    };


}

#endif // ifndef GWK_WINDOW_PROVIDER_H
