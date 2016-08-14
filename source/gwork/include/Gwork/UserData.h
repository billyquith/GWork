/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_USERDATA_H
#define GWK_USERDATA_H

#include <map>

namespace Gwk
{
    /**
     *
     *  Allows you to store multiple and various user data
     *
     *  ~~~
     *  //
     *  // Valid
     *  //
     *  UserDataStorage.Set( 100.0f );
     *  UserDataStorage.<float>Get();
     *
     *  //
     *  // Invalid - didn't Add a float type!
     *  //
     *  UserDataStorage.Set( 100 );
     *  UserDataStorage.<float>Get();
     *
     *  //
     *  // You can pass structs as long as they can be copied safely.
     *  //
     *  UserDataStorage.Set( mystruct );
     *  UserDataStorage.<MyStruct>Get();
     *
     *  //
     *  // If you pass a pointer then whatever it's pointing to
     *  // should be valid for the duration. And it won't be freed.
     *  //
     *  UserDataStorage.Set( &mystruct );
     *  UserDataStorage.<MyStruct*>Get();
     *  ~~~
     */
    class UserDataStorage
    {
        struct ValueBase
        {
            virtual ~ValueBase() {}
            virtual void DeleteThis() = 0;
        };


        template <typename T>
        struct Value : public ValueBase
        {
            T val;

            Value(const T& v)
            :   val(v)
            {}

            // Give value a chance to clean itself up
            // Note: could avoid using smart pointers.
            virtual void DeleteThis()
            {
                delete this;
            }
        };

        typedef std::map<Gwk::String, ValueBase*> Container;
        Container m_list;

    public:

        UserDataStorage() {}
    
        ~UserDataStorage()
        {
            for (auto&& item : m_list)
            {
                item.second->DeleteThis();
            }
        }

        template <typename T>
        void Set(const Gwk::String& str, const T& var)
        {
            Value<T> *val = nullptr;
            Container::iterator it = m_list.find(str);

            if (it != m_list.end())
            {
                static_cast< Value<T>* >(it->second)->val = var;
            }
            else
            {
                val = new Value<T>(var);
                m_list[str] = val;
            }
        }

        bool Exists(const Gwk::String& str) const
        {
            return m_list.find(str) != m_list.end();
        }

        template <typename T>
        T& Get(const Gwk::String& str)
        {
            Value<T> *v = static_cast< Value<T>* >(m_list[str]);
            return v->val;
        }
    };


} // namespace Gwk

#endif // ifndef GWK_USERDATA_H
