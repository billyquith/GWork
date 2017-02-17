/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
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
    for (auto&& h : m_handlers)
    {
        h.handler->UnRegisterCaller(this);
    }

    m_handlers.clear();
}

void Listener::Call(Controls::Base* pThis)
{
    Event::Info info(pThis);
    info.Control = pThis;
    Call(pThis, info);
}

void Listener::Call(Controls::Base* pThis, Event::Info information)
{
    Event::Info info(nullptr);
    info = information;
    info.ControlCaller = pThis;

    for (auto&& h : m_handlers)
    {
        info.Packet = &h.Packet;

        if (h.callback)
            h.callback(*h.handler, info);
    }
}

void Listener::AddInternal(Handler *handler, EventCallback const &ecb,
                           const Gwk::Event::Packet& packet)
{
    HandlerInstance h = { ecb, packet, handler };
    m_handlers.push_back(h);
    handler->RegisterCaller(this);
}

void Listener::RemoveHandler(Event::Handler* object)
{
    object->UnRegisterCaller(this);
    std::list<HandlerInstance>::iterator iter = m_handlers.begin();

    while (iter != m_handlers.end())
    {
        HandlerInstance& h = *iter;

        if (h.handler == object)
            iter = m_handlers.erase(iter);
        else
            ++iter;
    }
}

} // namespace Gwk
