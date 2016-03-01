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

std::list<BaseHook*> g_hookList;

void Gwk::Hook::AddHook(BaseHook* hook)
{
    g_hookList.push_back(hook);
}

void Gwk::Hook::RemoveHook(BaseHook* hook)
{
    g_hookList.remove(hook);
}

HookList& Gwk::Hook::GetHookList()
{
    return g_hookList;
}

#endif // if GWK_HOOKSYSTEM
