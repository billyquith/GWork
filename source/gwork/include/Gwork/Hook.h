/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_HOOK_H
#define GWK_HOOK_H

#include <Gwork/Controls/Base.h>

// Enable the hook system.
#define GWK_HOOKSYSTEM 1

#if GWK_HOOKSYSTEM

#include <list>

namespace Gwk
{
    namespace Hook
    {
        class GWK_EXPORT BaseHook
        {
        public:

            virtual bool OnControlClicked(Gwk::Controls::Base*, int /*iMouseX*/, int /*iMouseY*/)
            {
                return false;
            }

        };


        typedef std::list<BaseHook*> HookList;

        GWK_EXPORT HookList& GetHookList();

        GWK_EXPORT void AddHook(BaseHook* hook);
        GWK_EXPORT void RemoveHook(BaseHook* hook);

        template <typename fnc>
        bool CallHook(fnc f)
        {
            for (auto&& hook : GetHookList())
            {
                if ((hook->*f)())
                    return true;
            }

            return false;
        }

        template <typename fnc, typename AA>
        bool CallHook(fnc f, AA a)
        {
            for (auto&& hook : GetHookList())
            {
                if ((hook->*f)(a))
                    return true;
            }

            return false;
        }

        template <typename fnc, typename AA, typename AB>
        bool CallHook(fnc f, AA a, AB b)
        {
            for (auto&& hook : GetHookList())
            {
                if ((hook->*f)(a, b))
                    return true;
            }

            return false;
        }

        template <typename fnc, typename AA, typename AB, typename AC>
        bool CallHook(fnc f, AA a, AB b, AC c)
        {
            for (auto&& hook : GetHookList())
            {
                if ((hook->*f)(a, b, c))
                    return true;
            }

            return false;
        }

    }

}

#endif // if GWK_HOOKSYSTEM
#endif // ifndef GWK_HOOK_H
