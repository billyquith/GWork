/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_EVENTS_H
#define GWK_EVENTS_H

#include <Gwork/Config.h>
#include <Gwork/UiTypes.h>
#include <Gwork/ControlList.h>
#include <functional>

namespace Gwk
{
    namespace Controls
    {
        class Base;
    }

    namespace Event
    {
        
        class Listener;

        /**
         *  When adding an event hook you can add a Packet too.
         *  This will be passed in Event::Info when you receive an event.
         */
        struct Packet
        {
            Packet(Gwk::Controls::Base* control = nullptr)
            :   Control(control)
            {}

            // TODO - union? variant?
            Gwk::Controls::Base *Control;
            Gwk::String String;
            int Integer;
            float Float;
            unsigned long long UnsignedLongLong;
        };


        /**
         *  Passed to an event listener.
         */
        struct Info
        {
            Info(Gwk::Controls::Base* ctrl)
            :   ControlCaller(nullptr)
            ,   Packet(nullptr)
            ,   Control(ctrl)
            ,   Integer(0)
            {}

            /// This is set by the event Listener, it will always be
            /// the control which is calling the event.
            Gwk::Controls::Base* ControlCaller;

            /// This is set by the event hook, e.g. :-
            /// ~~~
            /// onDoSomething.Add( this, &ThisClass::MyFunction, Gwk::Event::Packet("Something") );
            /// ~~~
            Gwk::Event::Packet* Packet;

            /// These are set by the event and may or may not be set.
            Gwk::Controls::Base* Control;

            Gwk::String String;
            Gwk::Point Point;
            int Integer;
        };

        /**
         *  A class wanting to receive events must be derived from this.
         */
        class GWK_EXPORT Handler
        {
            PONDER_RTTI()
        public:

            virtual ~Handler();

            void RegisterCaller(Listener*);
            void UnRegisterCaller(Listener*);

        protected:

            Handler();  // Class needs to be derived.

        private:

            void CleanLinks();
            std::list<Listener*> m_listeners;
        };
        
        /**
         *  \brief Event listener.
         *
         *  Event handler callbacks are registered with this to receive event notifications.
         *
         *      Control (event) -> Listener (call) -> Handler (callback)
         *
         */
        class GWK_EXPORT Listener
        {
        public:

            typedef void (Handler::*EventListener)(Event::Info info);

            typedef std::function<void(Handler&, Event::Info)> EventCallback;
            
            Listener();
            ~Listener();

            // add function object handler
            void AddCb(Handler *handler, EventCallback const& cb,
                     const Event::Packet& packet = Event::Packet())
            {
                AddInternal(handler, cb, packet);
            }

            // add class method handler
            template <typename T>
            void Add(Handler* handler, void (T::*f)(Info),
                     const Packet& packet = Packet())
            {
                auto cb = [=](Handler &h, Info const& i) -> void {
                    (static_cast<T&>(h).*f)(i);
                };
                AddCb(handler, EventCallback(cb), packet);
            }
            
            void RemoveHandler(Event::Handler* object);

            void Call(Controls::Base* pThis);
            void Call(Controls::Base* pThis, Event::Info info);
            
        private:
            
            void AddInternal(Handler *handler, EventCallback const& ecb,
                             const Event::Packet& packet);

            void CleanLinks();

            struct HandlerInstance
            {
                EventCallback       callback;
                Event::Packet       Packet;
                Handler             *handler;
            };

            std::list<HandlerInstance> m_handlers;
        };

    } // namespace Event
} // namespace Gwk

PONDER_TYPE(Gwk::Event::Handler)

#endif // ifndef GWK_EVENTS_H
