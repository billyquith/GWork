//
//  GwenUtil.h - Utilites for Gwen ImportExport.
//
//  Note: This is done to remove the dependency on Bootil and keep the Gwen and the editor self
//        contained. Bootil contains a lot of other stuff that we don't want. Import needs to be
//        able to be used by the client app, and Gwen "dropped in".
//

#include <string>
#include <list>

#pragma once
#ifndef IMPORTEXPORT_UIDATA_H
#define IMPORTEXPORT_UIDATA_H

namespace GwenUtil
{
    typedef std::string     BString;
    typedef std::wstring    WString;

#define GWENUTIL_FOREACH(varname, arrayname, arraytype) \
    for (arraytype::iterator varname = arrayname.begin(); varname != arrayname.end(); ++varname)

#define GWENUTIL_FOREACH_CONST(varname, arrayname, arraytype) \
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
            BString BinaryToHex(const void* pData, unsigned int iLength);
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
            inline unsigned char ValueId<GwenUtil::BString>() { return 1; }
            
            // Float
            template <>
            inline unsigned char ValueId<float>() { return 2; }
            
            template <>
            inline GwenUtil::BString ToString(float var)
            {
                return GwenUtil::String::Format::NiceFloat(var);
            }
            
            template <>
            inline float ToValue(const GwenUtil::BString& var)
            {
                return GwenUtil::String::To::Float(var);
            }
            
            // int
            template <>
            inline unsigned char ValueId<int>() { return 3; }
            
            template <>
            inline GwenUtil::BString ToString(int var)
            {
                return GwenUtil::String::Format::Int(var);
            }
            
            template <>
            inline int ToValue(const GwenUtil::BString& var)
            {
                return GwenUtil::String::To::Int(var);
            }
            
            // Bool
            template <>
            inline unsigned char ValueId<bool>() { return 4; }
            
            template <>
            inline GwenUtil::BString ToString(bool var)
            {
                return var ? "true" : "false";
            }
            
            template <>
            inline bool ToValue(const GwenUtil::BString& var)
            {
                return GwenUtil::String::To::Bool(var);
            }
            
            // Double
            template <>
            inline unsigned char ValueId<double>() { return 5; }
            
            template <>
            inline GwenUtil::BString ToString(double var)
            {
                return GwenUtil::String::Format::NiceDouble(var);
            }
            
            template <>
            inline double ToValue(const GwenUtil::BString& var)
            {
                return GwenUtil::String::To::Double(var);
            }
        }
        
        //! The tree is a simple recursive Name-Value data structure.
        //! Each node can have a key, a value and multiple child nodes.
        class Tree
        {
        public:

            typedef typename std::list<Tree>   List;

            Tree()
            :   m_Info(NULL)
            {
            }

            const BString& Name() const;
            void           Name(const BString& name);

            const BString& Value() const;
            void           Value(const BString& value);

            //!
            //! Returns true if we have some children
            //!
            //!     GWENUTIL_FOREACH[_CONST]( child, mytree.Children(), Bootil::Data::Tree::List )
            //!     {
            //!         // Do stuff to 'child'
            //!     }
            //!
            bool HasChildren() const;

            //! Returns a list of children.
            const List& Children() const
            {
                return m_Children;
            }

            List& Children()
            {
                return m_Children;
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
                return m_Info == 0;
            }

        protected:

            BString m_Name;
            BString m_Value;
            unsigned char m_Info;

            List m_Children;
        };


        inline const BString& Tree::Name() const
        {
            return m_Name;
        }

        inline void Tree::Name(const BString& name)
        {
            m_Name = name;
        }

        inline const BString& Tree::Value() const
        {
            return m_Value;
        }

        inline void Tree::Value(const BString& value)
        {
            m_Info = Value::ValueId<BString>();
            m_Value = value;
        }

        inline Tree& Tree::AddChild()
        {
            Tree t;
            m_Children.push_back(t);

            Tree& back = m_Children.back();
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
            GWENUTIL_FOREACH_CONST(a, Children(), typename List)
            {
                if (a->Name() == name)
                    return a->Value();
            }

            return Default;
        }

        inline bool Tree::HasChild(const BString& name) const
        {
            GWENUTIL_FOREACH_CONST(a, Children(), typename List)
            {
                if (a->Name() == name)
                    return true;
            }

            return false;
        }

        inline Tree& Tree::GetChild(const BString& name)
        {
            GWENUTIL_FOREACH(a, Children(), typename List)
            {
                if (a->Name() == name)
                    return *a;
            }

            return AddChild(name);
        }

        inline bool Tree::HasChildren() const
        {
            return !m_Children.empty();
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
            GWENUTIL_FOREACH_CONST(a, Children(), typename List)
            {
                if (a->Name() == strKey)
                    return a->template Var<TValue>();
            }

            return varDefault;
        }

        template <typename TValue>
        inline void Tree::Var(TValue var)
        {
            m_Info = Value::ValueId<TValue>();
            m_Value = Value::ToString<TValue>(var);
        }

        template <typename TValue>
        inline TValue Tree::Var() const
        {
            return Value::ToValue<TValue>(Value());
        }

        template <typename TValue>
        inline bool Tree::IsVar() const
        {
            return m_Info == Value::ValueId<TValue>();
        }

        namespace Json
        {
            bool Export(const GwenUtil::Data::Tree& tree, GwenUtil::BString& output,
                        bool bPretty = false);
            bool Import(GwenUtil::Data::Tree& tree, const GwenUtil::BString& input);
        }

    } // namespace Data

}

#endif // ifndef IMPORTEXPORT_UIDATA_H
