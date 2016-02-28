/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Hook.h>

#if GWK_HOOKSYSTEM

using namespace Gwk;
using namespace Gwk::Hook;

std::list<BaseHook*> g_HookList;

void Gwk::Hook::AddHook(BaseHook* pHook)
{
    g_HookList.push_back(pHook);
}

void Gwk::Hook::RemoveHook(BaseHook* pHook)
{
    g_HookList.remove(pHook);
}

HookList& Gwk::Hook::GetHookList()
{
    return g_HookList;
}

#endif // if GWK_HOOKSYSTEM
