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
    typedef std::string BString;
    typedef std::wstring WString;

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
        //
        //! The tree is a simple recursive Name-Value data structure.
        //! Each node can have a key, a value and multiple children nodes
        //
        template <typename TString>
        class TreeT
        {
        public:

            typedef TreeT<TString>                    ThisClass;
            typedef typename std::list<ThisClass>     List;

        public:

            TreeT()
            {
                m_Info = 0;
            }

            const TString& Name() const;
            void           Name(const TString& name);

            const TString& Value() const;
            void           Value(const TString& value);

            //!
            //! Returns true if we have some children
            //!
            //!     GWENUTIL_FOREACH[_CONST]( child, mytree.Children(), Bootil::Data::Tree::List )
            //!     {
            //!         // Do stuff to 'child'
            //!     }
            //!
            bool HasChildren() const;

            //
            //! Returns a list of children.
            //
            const List& Children() const
            {
                return m_Children;
            }

            List& Children()
            {
                return m_Children;
            }

            //
            //! Adding and setting children.
            //!     mychild = AddChild();                   // adds an unnamed child
            //!     mychild = AddChild( "Name" );           // adds a named child
            //!     mychild = SetChild( "Name", "Value" );  // adds a named child with value
            //!     mychild = GetChild( "Name" );           // returns child, or creates child if not exists
            //!     bool b  = HasChild( "Name" );           // returns true if child exists
            //
            TreeT<TString>& AddChild();
            TreeT<TString>& AddChild(TString name);
            TreeT<TString>& SetChild(TString strKey, TString strValue);
            TreeT<TString>& GetChild(const TString& name);
            bool            HasChild(const TString& name) const;

            //
            //! Getting Child Value.
            //!     value = Value( "Name", "Default" );     // returns "Default" if "Name" isn't found.
            //
            TString ChildValue(const TString& name, const TString& Default = "") const;


            //
            // Setting non-string values
            //
            template <typename TValue>
            TreeT<TString>& SetChildVar(TString strKey, TValue strValue);
            template <typename TValue>
            TValue ChildVar(TString strKey,
                            TValue Default) const;
            template <typename TValue>
            void Var(TValue strValue);
            template <typename TValue>
            TValue Var() const;
            template <typename TValue>
            bool IsVar() const;

            // Utility methods
            //
            template <typename TValue>
            TString VarToString(TValue var) const;
            template <typename TValue>
            TValue StringToVar(const TString& str) const;
            template <typename TValue>
            unsigned char VarID() const;
            bool          IsBranch() const
            {
                return m_Info == 0;
            }

        protected:

            TString m_Name;
            TString m_Value;
            unsigned char m_Info;

            List m_Children;
        };


        //
        // We have a normal version and a wide version by default
        // Although the wide version is probably a waste of time since
        // nothing supports it right now.
        //
        typedef TreeT<BString> Tree;
        typedef TreeT<WString> TreeW;


        //
        // Template function definitions
        //
        template <typename TString>
        inline const TString& TreeT<TString>::Name() const
        {
            return m_Name;
        }

        template <typename TString>
        inline void TreeT<TString>::Name(const TString& name)
        {
            m_Name = name;
        }

        template <typename TString>
        inline const TString& TreeT<TString>::Value() const
        {
            return m_Value;
        }

        template <typename TString>
        inline void TreeT<TString>::Value(const TString& value)
        {
            m_Info = 1;
            m_Value = value;
        }

        template <typename TString>
        inline TreeT<TString>& TreeT<TString>::AddChild()
        {
            {
                TreeT<TString> t;
                m_Children.push_back(t);
            }

            TreeT<TString>& t = m_Children.back();
            return t;
        }

        template <typename TString>
        inline TreeT<TString>& TreeT<TString>::AddChild(TString name)
        {
            TreeT<TString>& tree = AddChild();
            tree.Name(name);
            return tree;
        }

        template <typename TString>
        inline TreeT<TString>& TreeT<TString>::SetChild(TString strKey, TString strValue)
        {
            TreeT<TString>& tchild = AddChild(strKey);
            tchild.Value(strValue);
            return tchild;
        }

        template <typename TString>
        inline TString TreeT<TString>::ChildValue(const TString& name, const TString& Default) const
        {
            GWENUTIL_FOREACH_CONST(a, Children(), typename List)
            {
                if (a->Name() == name)
                    return a->Value();
            }

            return Default;
        }

        template <typename TString>
        inline bool TreeT<TString>::HasChild(const TString& name) const
        {
            GWENUTIL_FOREACH_CONST(a, Children(), typename List)
            {
                if (a->Name() == name)
                    return true;
            }

            return false;
        }

        template <typename TString>
        inline TreeT<TString>& TreeT<TString>::GetChild(const TString& name)
        {
            GWENUTIL_FOREACH(a, Children(), typename List)
            {
                if (a->Name() == name)
                    return *a;
            }

            return AddChild(name);
        }

        template <typename TString>
        inline bool TreeT<TString>::HasChildren() const
        {
            return !m_Children.empty();
        }

        template <typename TString>
        template <typename TValue>
        inline TreeT<TString>& TreeT<TString>::SetChildVar(TString strKey, TValue var)
        {
            TreeT<TString>& tchild = AddChild(strKey);
            tchild.Var<TValue>(var);
            return tchild;
        }

        template <typename TString>
        template <typename TValue>
        inline TValue TreeT<TString>::ChildVar(TString strKey, TValue varDefault) const
        {
            GWENUTIL_FOREACH_CONST(a, Children(), typename List)
            {
                if (a->Name() == strKey)
                    return a->template Var<TValue>();
            }

            return varDefault;
        }

        template <typename TString>
        template <typename TValue>
        inline void TreeT<TString>::Var(TValue var)
        {
            m_Info = VarID<TValue>();
            m_Value = VarToString<TValue>(var);
        }

        template <typename TString>
        template <typename TValue>
        inline TValue TreeT<TString>::Var() const
        {
            return StringToVar<TValue>(Value());
        }

        template <typename TString>
        template <typename TValue>
        inline bool TreeT<TString>::IsVar() const
        {
            return m_Info == VarID<TValue>();
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
