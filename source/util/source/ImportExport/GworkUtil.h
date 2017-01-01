/*
 *  GworkUtil.h - Utilites for Gwork ImportExport.
 *
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 *
 *  Note: This is done to remove the dependency on Bootil and keep the Gwork and the editor self
 *        contained. Bootil contains a lot of other stuff that we don't want. Import needs to be
 *        able to be used by the client app, and Gwork "dropped in".
 */

#include <string>
#include <list>

#pragma once
#ifndef IMPORTEXPORT_UIDATA_H
#define IMPORTEXPORT_UIDATA_H

namespace GwkUtil
{
    typedef std::string     BString;
    typedef std::wstring    WString;

#define GWKUTIL_FOREACH(varname, arrayname, arraytype) \
    for (arraytype::iterator varname = arrayname.begin(); varname != arrayname.end(); ++varname)

#define GWKUTIL_FOREACH_CONST(varname, arrayname, arraytype) \
    for (arraytype::const_iterator varname = arrayname.begin(); \
         varname != arrayname.end(); \
         ++varname)

    namespace String
    {
        namespace Convert
        {
            BString FromWide(const WString& strIn);
            WString ToWide(const BString& strIn);
        }

        namespace Format
        {
            BString Print(const char *fmt, ...);
            WString Print(const wchar_t *fmt, ...);

            BString NiceFloat(float f);
            BString NiceDouble(double f);
            BString BinaryToHex(const void* data, unsigned int iLength);
            BString UInt64(unsigned long long iBytes);
            BString Int(int iBytes);
        }

        namespace To
        {
            int                Int(const BString& str);
            float              Float(const BString& str);
            double             Double(const BString& str);
            bool               Floats(const BString& str, float* f, int iCount);
            bool               Bool(const BString& str); // Converts true, false, yes, no, 0, 1
            unsigned long long UInt64(const BString& str);
        }
    }

    namespace File
    {
        bool Read(const BString& strFileName, BString& strOut);
        bool Write(const BString& strFileName, const BString& strOut);
    }

    namespace Data
    {
        namespace Value
        {
            template <typename TValue>
            inline BString ToString(TValue var);
            
            template <typename TValue>
            inline TValue ToValue(const BString& str);
            
            //! Get Value id for type.
            //! @Note Changing these could break saved data.
            template <typename TValue>
            inline unsigned char ValueId();
            
            // String (generic value)
            template <>
            inline unsigned char ValueId<GwkUtil::BString>() { return 1; }
            
            // Float
            template <>
            inline unsigned char ValueId<float>() { return 2; }
            
            template <>
            inline GwkUtil::BString ToString(float var)
            {
                return GwkUtil::String::Format::NiceFloat(var);
            }
            
            template <>
            inline float ToValue(const GwkUtil::BString& var)
            {
                return GwkUtil::String::To::Float(var);
            }
            
            // int
            template <>
            inline unsigned char ValueId<int>() { return 3; }
            
            template <>
            inline GwkUtil::BString ToString(int var)
            {
                return GwkUtil::String::Format::Int(var);
            }
            
            template <>
            inline int ToValue(const GwkUtil::BString& var)
            {
                return GwkUtil::String::To::Int(var);
            }
            
            // Bool
            template <>
            inline unsigned char ValueId<bool>() { return 4; }
            
            template <>
            inline GwkUtil::BString ToString(bool var)
            {
                return var ? "true" : "false";
            }
            
            template <>
            inline bool ToValue(const GwkUtil::BString& var)
            {
                return GwkUtil::String::To::Bool(var);
            }
            
            // Double
            template <>
            inline unsigned char ValueId<double>() { return 5; }
            
            template <>
            inline GwkUtil::BString ToString(double var)
            {
                return GwkUtil::String::Format::NiceDouble(var);
            }
            
            template <>
            inline double ToValue(const GwkUtil::BString& var)
            {
                return GwkUtil::String::To::Double(var);
            }
        }
        
        //! The tree is a simple recursive Name-Value data structure.
        //! Each node can have a key, a value and multiple child nodes.
        class Tree
        {
        public:

            typedef typename std::list<Tree>   List;

            Tree()
            :   m_info(0)
            {
            }

            const BString& Name() const;
            void           Name(const BString& name);

            const BString& Value() const;
            void           Value(const BString& value);

            //!
            //! Returns true if we have some children
            //!
            //!     GWKUTIL_FOREACH[_CONST]( child, mytree.Children(), Bootil::Data::Tree::List )
            //!     {
            //!         // Do stuff to 'child'
            //!     }
            //!
            bool HasChildren() const;

            //! Returns a list of children.
            const List& Children() const
            {
                return m_children;
            }

            List& Children()
            {
                return m_children;
            }

            //! Adding and setting children.
            //!     mychild = AddChild();                   // adds an unnamed child
            //!     mychild = AddChild( "Name" );           // adds a named child
            //!     mychild = SetChild( "Name", "Value" );  // adds a named child with value
            //!     mychild = GetChild( "Name" );           // returns child, or creates child if not exists
            //!     bool b  = HasChild( "Name" );           // returns true if child exists
            //
            Tree& AddChild();
            Tree& AddChild(BString name);
            Tree& SetChild(BString strKey, BString strValue);
            Tree& GetChild(const BString& name);
            bool HasChild(const BString& name) const;

            //! Getting Child Value.
            //!     value = Value( "Name", "Default" );     // returns "Default" if "Name" isn't found.
            BString ChildValue(const BString& name, const BString& Default = "") const;

            //
            // Setting non-string values
            //
            template <typename TValue>
            Tree& SetChildVar(BString strKey, TValue strValue);
            
            template <typename TValue>
            TValue ChildVar(BString strKey, TValue Default) const;
            
            template <typename TValue>
            void Var(TValue strValue);
            
            template <typename TValue>
            TValue Var() const;
            
            template <typename TValue>
            bool IsVar() const;
            
            bool IsBranch() const
            {
                return m_info == 0;
            }

        protected:

            BString m_name;
            BString m_value;
            unsigned char m_info;

            List m_children;
        };


        inline const BString& Tree::Name() const
        {
            return m_name;
        }

        inline void Tree::Name(const BString& name)
        {
            m_name = name;
        }

        inline const BString& Tree::Value() const
        {
            return m_value;
        }

        inline void Tree::Value(const BString& value)
        {
            m_info = Value::ValueId<BString>();
            m_value = value;
        }

        inline Tree& Tree::AddChild()
        {
            Tree t;
            m_children.push_back(t);

            Tree& back = m_children.back();
            return back;
        }

        inline Tree& Tree::AddChild(BString name)
        {
            Tree& tree = AddChild();
            tree.Name(name);
            return tree;
        }

        inline Tree& Tree::SetChild(BString strKey, BString strValue)
        {
            Tree& tchild = AddChild(strKey);
            tchild.Value(strValue);
            return tchild;
        }

        inline BString Tree::ChildValue(const BString& name, const BString& Default) const
        {
            GWKUTIL_FOREACH_CONST(a, Children(), typename List)
            {
                if (a->Name() == name)
                    return a->Value();
            }

            return Default;
        }

        inline bool Tree::HasChild(const BString& name) const
        {
            GWKUTIL_FOREACH_CONST(a, Children(), typename List)
            {
                if (a->Name() == name)
                    return true;
            }

            return false;
        }

        inline Tree& Tree::GetChild(const BString& name)
        {
            GWKUTIL_FOREACH(a, Children(), typename List)
            {
                if (a->Name() == name)
                    return *a;
            }

            return AddChild(name);
        }

        inline bool Tree::HasChildren() const
        {
            return !m_children.empty();
        }

        template <typename TValue>
        inline Tree& Tree::SetChildVar(BString strKey, TValue var)
        {
            Tree& tchild = AddChild(strKey);
            tchild.Var<TValue>(var);
            return tchild;
        }

        template <typename TValue>
        inline TValue Tree::ChildVar(BString strKey, TValue varDefault) const
        {
            GWKUTIL_FOREACH_CONST(a, Children(), typename List)
            {
                if (a->Name() == strKey)
                    return a->template Var<TValue>();
            }

            return varDefault;
        }

        template <typename TValue>
        inline void Tree::Var(TValue var)
        {
            m_info = Value::ValueId<TValue>();
            m_value = Value::ToString<TValue>(var);
        }

        template <typename TValue>
        inline TValue Tree::Var() const
        {
            return Value::ToValue<TValue>(Value());
        }

        template <typename TValue>
        inline bool Tree::IsVar() const
        {
            return m_info == Value::ValueId<TValue>();
        }

        namespace Json
        {
            bool Export(const GwkUtil::Data::Tree& tree, GwkUtil::BString& output,
                        bool bPretty = false);
            bool Import(GwkUtil::Data::Tree& tree, const GwkUtil::BString& input);
        }

    } // namespace Data

}

#endif // ifndef IMPORTEXPORT_UIDATA_H
