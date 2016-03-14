/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_EVENTS_H
#define GWK_EVENTS_H

#include <list>
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

            Gwk::Controls::Base   *Control;
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
            Information(Gwk::Controls::Base* ctrl = nullptr)
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

            Gwk::ControlList ControlList;
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

            virtual ~Handler();

            void RegisterCaller(Caller*);
            void UnRegisterCaller(Caller*);

        protected:

            Handler();  // Class needs to be derived.

            void CleanLinks();
            std::list<Caller*> m_callers;

        public:

//            typedef void (Handler::*Function)                   (Gwk::Controls::Base* fromPanel);
            typedef void (Handler::*FunctionBlank)              ();
            typedef void (Handler::*FunctionWithInformation)    (Gwk::Event::Info info);

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
            void Call(Controls::Base* pThis, Gwk::Event::Info info);

//            template <typename T>
//            void Add(Event::Handler* ob, T f)
//            {
//                AddInternal(ob, static_cast<Handler::Function>(f));
//            }

            template <typename T>
            void Add(Event::Handler* ob, void (T::*f)(Gwk::Event::Info))
            {
                AddInternal(ob, static_cast<Handler::FunctionWithInformation>(f));
            }

            template <typename T>
            void Add(Event::Handler* ob,
                     void (T::*f)(Gwk::Event::Info),
                     const Gwk::Event::Packet& packet)
            {
                AddInternal(ob, static_cast<Handler::FunctionWithInformation>(f), packet);
            }

            template <typename T>
            void Add(Event::Handler* ob, void (T::*f)())
            {
                AddInternal(ob, static_cast<Handler::FunctionBlank>(f));
            }

            void RemoveHandler(Event::Handler* object);

        private:

            void CleanLinks();
//            void AddInternal(Event::Handler* object, Handler::Function function);
            void AddInternal(Event::Handler* object, Handler::FunctionWithInformation function);
            void AddInternal(Event::Handler* object, Handler::FunctionWithInformation function,
                             const Gwk::Event::Packet& packet);
            void AddInternal(Event::Handler* object, Handler::FunctionBlank function);

            struct HandlerInstance
            {
                HandlerInstance()
//                :   fnFunction(nullptr)
                :   fnFunctionInfo(nullptr)
                ,   fnFunctionBlank(nullptr)
                ,   object(nullptr)
                {}

//                Handler::Function fnFunction;
                Handler::FunctionWithInformation fnFunctionInfo;
                Handler::FunctionBlank fnFunctionBlank;

                Event::Handler*         object;
                Gwk::Event::Packet Packet;
            };

            std::list<HandlerInstance> m_handlers;
        };


    }

}

PONDER_TYPE(Gwk::Event::Handler)

#endif // ifndef GWK_EVENTS_H
