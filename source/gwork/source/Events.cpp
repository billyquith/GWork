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
    std::list<Listener*>::iterator iter = m_listeners.begin();

    while (iter != m_listeners.end())
    {
        Listener* Listener = *iter;
        UnRegisterCaller(Listener);
        Listener->RemoveHandler(this);
        iter = m_listeners.begin();
    }
}

void Handler::RegisterCaller(Listener* Listener)
{
    m_listeners.push_back(Listener);
}

void Handler::UnRegisterCaller(Listener* Listener)
{
    m_listeners.remove(Listener);
}

Listener::Listener()
{
}

Listener::~Listener()
{
    CleanLinks();
}

void Listener::CleanLinks()
{
    for (auto& h : m_handlers)
    {
        h.listener->UnRegisterCaller(this);
    }

    m_handlers.clear();
}

void Listener::Call(Controls::Base* pThis)
{
    Event::Information info(pThis);
    info.Control = pThis;
    Call(pThis, info);
}

void Listener::Call(Controls::Base* pThis, Event::Info information)
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

void Listener::AddInternal(Event::Handler* listener,
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

void Listener::RemoveHandler(Event::Handler* object)
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
