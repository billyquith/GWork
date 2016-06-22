/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Events.h>

namespace Gwk
{
    
using namespace Gwk::Event;

Handler::Handler()
{
}

Handler::~Handler()
{
    CleanLinks();
}

void Handler::CleanLinks()
{
    // Tell all the callers that we're dead
    std::list<Caller*>::iterator iter = m_callers.begin();

    while (iter != m_callers.end())
    {
        Caller* caller = *iter;
        UnRegisterCaller(caller);
        caller->RemoveHandler(this);
        iter = m_callers.begin();
    }
}

void Handler::RegisterCaller(Caller* caller)
{
    m_callers.push_back(caller);
}

void Handler::UnRegisterCaller(Caller* caller)
{
    m_callers.remove(caller);
}

Caller::Caller()
{
}

Caller::~Caller()
{
    CleanLinks();
}

void Caller::CleanLinks()
{
    for (auto& h : m_handlers)
    {
        h.listener->UnRegisterCaller(this);
    }

    m_handlers.clear();
}

void Caller::Call(Controls::Base* pThis)
{
    Event::Information info(pThis);
    info.Control = pThis;
    Call(pThis, info);
}

void Caller::Call(Controls::Base* pThis, Event::Info information)
{
    Event::Information info(nullptr);
    info = information;
    info.ControlCaller = pThis;
    std::list<HandlerInstance>::iterator iter;

    for (auto& h : m_handlers)
    {
        info.Packet = &h.Packet;

        if (h.callback)
            h.callback(*h.listener, info);
    }
}

void Caller::AddInternal(Event::Handler* listener,
                         EventListener function,
                         const Gwk::Event::Packet& packet)
{
    HandlerInstance h;
    h.callback  = function;
    h.listener  = listener;
    h.Packet    = packet;
    m_handlers.push_back(h);
    listener->RegisterCaller(this);
}

void Caller::RemoveHandler(Event::Handler* object)
{
    object->UnRegisterCaller(this);
    std::list<HandlerInstance>::iterator iter = m_handlers.begin();

    while (iter != m_handlers.end())
    {
        HandlerInstance& h = *iter;

        if (h.listener == object)
            iter = m_handlers.erase(iter);
        else
            ++iter;
    }
}

} // namespace Gwk
