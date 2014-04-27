
#pragma once

#include <Gwen/Gwen.h>

namespace Gwen
{
    namespace ControlFactory
    {
        class Base;
        
        typedef std::list<ControlFactory::Base*> List;

        List&                 GetList();
        ControlFactory::Base* Find(const Gwen::String& name);
        Controls::Base*       Clone(Controls::Base* pEnt, ControlFactory::Base* pFactory);
        
        template <typename ENUM>
        struct ValueEnumMapItem
        {
            typedef ENUM EnumType;
            const char *name;
            EnumType value;
        };
        
        //! Class to allow mapping from enums to strings and back.
        //! @param TYPE : The enum type we are mapping.
        template <typename ENUM>
        class ValueEnumMap
        {
        public:
            typedef ENUM EnumType;
            typedef unsigned int Count_t;
            typedef ValueEnumMapItem<EnumType> Enum;
            static const int Undefined = -1;
            
            ValueEnumMap(const Enum *pairs, Count_t count)
            :   m_Enums(pairs)
            ,   m_EnumCount(count)
            {}
            
            size_t GetNumEnums() const { return m_EnumCount; }
            
            const char* GetNameByIndex(Count_t index) const
            {
                return m_Enums[index].name;
            }

            size_t GetIndexByName(const char *name) const
            {
                for (Count_t i=0; i < m_EnumCount; ++i)
                {
                    const Enum &item = m_Enums[i];
                    if (strcmp(item.name, name)==0)
                        return i;
                }
                return Undefined;
            }

            ENUM GetValueByName(const char *name, EnumType defaultValue = Undefined) const
            {
                const size_t i = GetIndexByName(name);
                return i!=Undefined ? m_Enums[i].value : defaultValue;
            }

            const char* GetNameByValue(EnumType value, Count_t defaultIndex=0) const
            {
                for (size_t i=0; i < m_EnumCount; ++i)
                {
                    const Enum &item = m_Enums[i];
                    if (item.value == value)
                        return item.name;       // We found the item.
                }
                return m_Enums[defaultIndex].name;   // Not found. Return default indexed.
            }

        private:
            const Enum     *m_Enums;
            const Count_t   m_EnumCount;
        };
        

        class Property
        {
        public:

            typedef std::list<Property*> List;

            // For serialisation.
            virtual Gwen::String Name() const = 0;
            
            // Help.
            virtual Gwen::String Description() const = 0;

            // For the property editor.
            virtual Gwen::String GetValueAsString(Gwen::Controls::Base* ctrl) = 0;
            virtual void SetValueFromString(Gwen::Controls::Base* ctrl, const Gwen::String& str) = 0;

            virtual int OptionNum()
            {
                return 0;
            }

            virtual Gwen::String OptionGet(int i)
            {
                return "";
            }

            virtual int NumCount()
            {
                return 0;
            }

            virtual Gwen::String NumName(int i)
            {
                return "unknown";
            }

            virtual float NumGet(Gwen::Controls::Base* ctrl, int i)
            {
                return 0.0f;
            }

            virtual void NumSet(Gwen::Controls::Base* ctrl, int i, float f)
            {
            }

            inline void NumSet(Gwen::Controls::Base* ctrl, const Gwen::String& str, float f)
            {
                for (int i = 0; i < NumCount(); i++)
                {
                    if (NumName(i) == str)
                        NumSet(ctrl, i, f);
                }
            }

        };


        class PropertyBool : public Property
        {
        public:

            static const Gwen::String True;
            static const Gwen::String False;

            virtual int OptionNum()
            {
                return 2;
            }

            virtual Gwen::String OptionGet(int i)
            {
                if (i == 0)
                    return False;

                return True;
            }

        };


        /// Base class for ControlFactorys.
        //
        class Base
        {
        public:

            Base();

            virtual Gwen::String Name() const = 0;
            virtual Gwen::String ParentFactory() const = 0;

            virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent) = 0;

            Base* GetBaseFactory();
            void  AddProperty(Property* pProp);

            Property* GetProperty(const Gwen::String& name);
            void      SetControlValue(Gwen::Controls::Base* ctrl, const Gwen::String& name,
                                      const Gwen::String& str);

            const Property::List& Properties()
            {
                return m_Properties;
            }

            /// Called when the control is drag and dropped onto the parent, even
            /// when just moving in the designer
            virtual void AddChild(Gwen::Controls::Base* ctrl,
                                  Gwen::Controls::Base* child,
                                  const Gwen::Point& pos);

            /// Called when creating the control - param might be empty
            virtual void AddChild(Gwen::Controls::Base* ctrl,
                                  Gwen::Controls::Base* child,
                                  int iPage = 0);

            /// Called when a child is clicked on in an editor
            virtual bool ChildTouched(Gwen::Controls::Base* ctrl,
                                      Gwen::Controls::Base* pChildControl)
            {
                return false;
            }

            //
            /// If a control is parented to a control with multiple sections, or
            /// pages these functions are used to store which page they're on - on the
            /// child control
            //
            virtual void SetParentPage(Gwen::Controls::Base* ctrl, int i);
            virtual int  GetParentPage(Gwen::Controls::Base* ctrl);

        protected:

            Property::List m_Properties;
        };


    } // ControlFactory

} // Gwen


//! Information about the ControlFactory.
//! @param CONTROL : Name of the control factory class.
//! @param TYPE : The control type (including namespace) that this is factory for.
//! @param INHERITS : Name of the factory class we inherit properties from.
#define GWEN_CONTROL_FACTORY_DETAILS(CONTROL, TYPE, INHERITS) \
    typedef TYPE FactoryFor; \
    virtual Gwen::String Name() const { return #CONTROL; } \
    virtual Gwen::String ParentFactory() const { return #INHERITS; }

//! Information about the ControlFactory.
//! @param FACTORY : Name of the control factory class.
//! @param FACTORY_PARENT :  The factory class (not Control) that this factory derives from.
#define GWEN_CONTROL_FACTORY_CONSTRUCTOR(FACTORY, FACTORY_PARENT) \
    typedef Gwen::ControlFactory::FACTORY ThisClass; \
    typedef FACTORY_PARENT ParentClass; \
    FACTORY() : ParentClass()

//! Standard declaration for a control factory. This can be overridden for unusal cases
//! and the other macros used.
//! @param CONTROL : Name of the control we are creating a factory for.
//! @param INHERITS : Name of the factory class we inherit properties from.
#define GWEN_CONTROL_FACTORY_FOR(CONTROL, INHERITS) \
    GWEN_CONTROL_FACTORY_DETAILS(CONTROL, Gwen::Controls::CONTROL, INHERITS) \
    GWEN_CONTROL_FACTORY_CONSTRUCTOR(CONTROL##_Factory, Gwen::ControlFactory::Base)

//! Instance a ControlFactory.
//! @param FACTORY_CLASS - The name of the factory.
#define GWEN_CONTROL_FACTORY(FACTORY_CLASS) \
    void GWENCONTROLFACTORY##FACTORY_CLASS() \
    { \
        static FACTORY_CLASS instance; \
    }

//! Declare a ControlFactory factory so that it can be called.
//! @param FACTORY : Name of the control factory class.
#define DECLARE_GWEN_CONTROL_FACTORY(FACTORY) \
    extern void GWENCONTROLFACTORY##FACTORY(); \
    GWENCONTROLFACTORY##FACTORY();

#define GWEN_CONTROL_FACTORY_PROPERTY(PROP_NAME, DESCRIPTION) \
public: \
    Gwen::String Name() const { return #PROP_NAME; } \
    Gwen::String Description() const { return DESCRIPTION; }


