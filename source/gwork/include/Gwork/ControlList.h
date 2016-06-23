/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLLIST_H
#define GWK_CONTROLLIST_H


namespace Gwk
{
    struct Point;

    namespace Controls
    {
        class Base;
    }

    namespace Event
    {
        class Handler;
        struct Information;
        struct Packet;
        struct Info;
    }

    template <typename TYPE>
    class TEasyList
    {
    public:

        typedef std::list<TYPE> List;

        void Add(TYPE control)
        {
            if (Contains(control))
                return;

            list.push_back(control);
        }

        void Remove(TYPE control)
        {
            list.remove(control);
        }

        void Add(const List& list)
        {
            for (typename List::const_iterator it = list.begin(); it != list.end(); ++it)
            {
                Add(*it);
            }
        }

        void Add(const TEasyList<TYPE>& list)
        {
            Add(list.list);
        }

        bool Contains(TYPE control) const
        {
            typename List::const_iterator it = std::find(list.begin(), list.end(), control);
            return it != list.end();
        }

        inline void Clear()
        {
            list.clear();
        }

        List list;
    };


    class ControlList : public TEasyList<Gwk::Controls::Base*>
    {
    public:

        void Enable();
        void Disable();

        void Show();
        void Hide();

        Gwk::String GetValue();
        void             SetValue(const Gwk::String& value);

        template <typename T>
        void SetAction(Gwk::Event::Handler* ob,
                       void ( T::*f )(Gwk::Event::Info),
                       const Gwk::Event::Packet& packet)
        {
            SetActionInternal(ob,
                              static_cast<void(Gwk::Event::Handler::*) (Gwk::Event::Info)>(f),
                              packet);
        }

        void MoveBy(const Gwk::Point& point);

        void DoAction();

    protected:

        void SetActionInternal(Gwk::Event::Handler*object,
                               void ( Gwk::Event::Handler::*f )(Gwk::Event::Info),
                               const Gwk::Event::Packet&packet);
    };


}

#endif // ifndef GWK_CONTROLLIST_H
