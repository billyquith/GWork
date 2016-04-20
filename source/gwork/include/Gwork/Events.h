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

namespace Gwk
{
    namespace Controls
    {
        class Base;
    }

    namespace Event
    {
        class Caller;

        /**
         *  When adding an event hook you can add a Packet too.
         *  This will be passed in Event::Info when you receive an event.
         */
        struct Packet
        {
            Packet(Gwk::Controls::Base* control = nullptr)
            :   Control(control)
            {}

            Gwk::Controls::Base *Control;
            Gwk::String String;
            int Integer;
            float Float;
            unsigned long long UnsignedLongLong;
        };


        /**
         *  Passed to an event hook
         */
        struct Information
        {
            Information(Gwk::Controls::Base* ctrl)
            :   ControlCaller(nullptr)
            ,   Packet(nullptr)
            ,   Control(ctrl)
            ,   Integer(0)
            {}

            /// This is set by the event caller, it will always be
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


        typedef const Gwk::Event::Information& Info;

        //
        /// A class wanting to receive events must be derived from this.
        //
        class GWK_EXPORT Handler
        {
            PONDER_RTTI()
        public:

            typedef void (Handler::*FunctionWithInformation)(Event::Info info);

            virtual ~Handler();

            void RegisterCaller(Caller*);
            void UnRegisterCaller(Caller*);

        protected:

            Handler();  // Class needs to be derived.

            void CleanLinks();
            std::list<Caller*> m_callers;

        };
        
        //
        /// Event callback management.
        //
        class GWK_EXPORT Caller
        {
        public:

            Caller();
            ~Caller();

            void Call(Controls::Base* pThis);
            void Call(Controls::Base* pThis, Event::Info info);

            template <typename T>
            void Add(Event::Handler* ob, void (T::*f)(Event::Info))
            {
                AddInternal(ob, static_cast<Handler::FunctionWithInformation>(f));
            }

            template <typename T>
            void Add(Event::Handler* ob, void (T::*f)(Event::Info), const Event::Packet& packet)
            {
                AddInternal(ob, static_cast<Handler::FunctionWithInformation>(f), packet);
            }

            void RemoveHandler(Event::Handler* object);

        private:

            void AddInternal(Event::Handler* object, Handler::FunctionWithInformation function);
            void AddInternal(Event::Handler* object, Handler::FunctionWithInformation function,
                             const Event::Packet& packet);

            void CleanLinks();

            struct HandlerInstance
            {
                HandlerInstance()
                :   fnFunctionInfo(nullptr)
                ,   object(nullptr)
                {}

                Handler::FunctionWithInformation fnFunctionInfo;
                Event::Handler*     object;
                Event::Packet       Packet;
            };

            std::list<HandlerInstance> m_handlers;
        };


    }

}

PONDER_TYPE(Gwk::Event::Handler)

#endif // ifndef GWK_EVENTS_H
