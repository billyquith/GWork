/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once

#include <Gwork/Gwork.h>
#include <Gwork/Controls.h>
#include <ponder/pondertype.hpp>
#include <ponder/classbuilder.hpp>

PONDER_TYPE(Gwk::Color)
PONDER_TYPE(Gwk::Margin)
PONDER_TYPE(Gwk::Point)
PONDER_TYPE(Gwk::Position)

PONDER_TYPE(Gwk::Controls::Base)
PONDER_TYPE(Gwk::Controls::Label)
PONDER_TYPE(Gwk::Controls::LabelClickable)
PONDER_TYPE(Gwk::Controls::Button)
PONDER_TYPE(Gwk::Controls::TextBox)
PONDER_TYPE(Gwk::Controls::TextBoxMultiline)
PONDER_TYPE(Gwk::Controls::CheckBox)
PONDER_TYPE(Gwk::Controls::CheckBoxWithLabel)
PONDER_TYPE(Gwk::Controls::ScrollControl)
PONDER_TYPE(Gwk::Controls::Menu)
PONDER_TYPE(Gwk::Controls::MenuItem)
PONDER_TYPE(Gwk::Controls::MenuStrip)
PONDER_TYPE(Gwk::Controls::ListBox)
PONDER_TYPE(Gwk::Controls::ImagePanel)
//PONDER_TYPE(Gwk::Controls::FilePicker)
//PONDER_TYPE(Gwk::Controls::FolderPicker)
//PONDER_TYPE(Gwk::Controls::PageControl)
PONDER_TYPE(Gwk::Controls::Rectangle)
PONDER_TYPE(Gwk::Controls::ProgressBar)
PONDER_TYPE(Gwk::Controls::ComboBox)
//PONDER_TYPE(Gwk::Controls::HorizontalSlider)
//PONDER_TYPE(Gwk::Controls::DesignerCanvas)


namespace Gwk
{
    namespace ControlFactory
    {
        class Base;
        
        typedef std::list<ControlFactory::Base*> List;
        
        void InitialiseControls();

        List&                 GetList();
        ControlFactory::Base* Find(const Gwk::String& name);
        Controls::Base*       Clone(Controls::Base* ent, ControlFactory::Base* factory);
        
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
            :   m_enums(pairs)
            ,   m_enumCount(count)
            {}
            
            size_t GetEnumCount() const { return m_enumCount; }
            
            const char* GetNameByIndex(Count_t index) const
            {
                return m_enums[index].name;
            }

            size_t GetIndexByName(const char *name) const
            {
                for (Count_t i=0; i < m_enumCount; ++i)
                {
                    if (strcmp(m_enums[i].name, name)==0)
                        return i;
                }
                return Undefined;
            }

            ENUM GetValueByName(const char *name, EnumType defaultValue = Undefined) const
            {
                const size_t i = GetIndexByName(name);
                return i!=Undefined ? m_enums[i].value : defaultValue;
            }

            const char* GetNameByValue(EnumType value, Count_t defaultIndex=0) const
            {
                for (size_t i=0; i < m_enumCount; ++i)
                {
                    const Enum &item = m_enums[i];
                    if (item.value == value)
                        return item.name;   // We found the item.
                }
                return m_enums[defaultIndex].name;   // Not found. Return default indexed.
            }

        private:
            const Enum     *m_enums;
            const Count_t   m_enumCount;
        };
        

        class Property
        {
        public:

            typedef std::list<Property*> List;

            // For serialisation.
            virtual Gwk::String Name() const = 0;
            
            // Help.
            virtual Gwk::String Description() const = 0;

            // For the property editor.
            virtual Gwk::String GetValueAsString(Gwk::Controls::Base* ctrl) = 0;
            virtual void SetValueFromString(Gwk::Controls::Base* ctrl, const Gwk::String& str) = 0;

            virtual size_t OptionCount() const
            {
                return 0;
            }

            virtual Gwk::String OptionGet(int i)
            {
                return "";
            }

            virtual size_t NumCount() const
            {
                return 0;
            }

            virtual Gwk::String NumName(size_t i) const
            {
                return "unknown";
            }

            virtual float NumGet(Gwk::Controls::Base* ctrl, int i)
            {
                return 0.0f;
            }

            virtual void NumSet(Gwk::Controls::Base* ctrl, int i, float f)
            {
            }

            inline void NumSet(Gwk::Controls::Base* ctrl, const Gwk::String& str, float f)
            {
                for (size_t i = 0; i < NumCount(); i++)
                {
                    if (NumName(i) == str)
                    {
                        NumSet(ctrl, i, f);
                        break;
                    }
                }
            }

        };


        class PropertyBool : public Property
        {
        public:

            static const Gwk::String True;
            static const Gwk::String False;

            virtual size_t OptionCount() const override
            {
                return 2;
            }

            virtual Gwk::String OptionGet(int i) override
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

            virtual Gwk::String Name() const = 0;
            virtual Gwk::String ParentFactory() const = 0;

            virtual Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent) = 0;

            Base* GetBaseFactory();
            void  AddProperty(Property* prop);

            Property* GetProperty(const Gwk::String& name);
            void      SetControlValue(Gwk::Controls::Base* ctrl, const Gwk::String& name,
                                      const Gwk::String& str);

            const Property::List& Properties()
            {
                return m_properties;
            }

            /// Called when the control is drag and dropped onto the parent, even
            /// when just moving in the designer
            virtual void AddChild(Gwk::Controls::Base* ctrl,
                                  Gwk::Controls::Base* child,
                                  const Gwk::Point& pos);

            /// Called when creating the control - param might be empty
            virtual void AddChild(Gwk::Controls::Base* ctrl,
                                  Gwk::Controls::Base* child,
                                  int iPage = 0);

            /// Called when a child is clicked on in an editor
            virtual bool ChildTouched(Gwk::Controls::Base* ctrl,
                                      Gwk::Controls::Base* childControl)
            {
                return false;
            }

            //
            /// If a control is parented to a control with multiple sections, or
            /// pages these functions are used to store which page they're on - on the
            /// child control
            //
            virtual void SetParentPage(Gwk::Controls::Base* ctrl, int i);
            virtual int  GetParentPage(Gwk::Controls::Base* ctrl);

        protected:

            Property::List m_properties;
        };


    } // ControlFactory

} // Gwork


//! Information about the ControlFactory.
//! @param CONTROL : Name of the control factory class.
//! @param TYPE : The control type (including namespace) that this is factory for.
//! @param INHERITS : Name of the factory class we inherit properties from.
#define GWK_CONTROL_FACTORY_DETAILS(CONTROL, TYPE, INHERITS) \
    typedef TYPE FactoryFor; \
    virtual Gwk::String Name() const override { return #CONTROL; } \
    virtual Gwk::String ParentFactory() const override { return #INHERITS; }

//! Information about the ControlFactory.
//! @param FACTORY : Name of the control factory class.
//! @param FACTORY_PARENT :  The factory class (not Control) that this factory derives from.
#define GWK_CONTROL_FACTORY_CONSTRUCTOR(FACTORY, FACTORY_PARENT) \
    typedef Gwk::ControlFactory::FACTORY ThisClass; \
    typedef FACTORY_PARENT ParentClass; \
    FACTORY() : ParentClass()

//! Standard declaration for a control factory. This can be overridden for unusal cases
//! and the other macros used.
//! @param CONTROL : Name of the control we are creating a factory for.
//! @param INHERITS : Name of the factory class we inherit properties from.
#define GWK_CONTROL_FACTORY_FOR(CONTROL, INHERITS) \
    GWK_CONTROL_FACTORY_DETAILS(CONTROL, Gwk::Controls::CONTROL, INHERITS) \
    GWK_CONTROL_FACTORY_CONSTRUCTOR(CONTROL##_Factory, Gwk::ControlFactory::Base)

//! Instance a ControlFactory.
//! @param FACTORY_CLASS - The name of the factory.
#define GWK_CONTROL_FACTORY(FACTORY_CLASS) \
    void GworkControlFactory##FACTORY_CLASS() \
    { \
        static FACTORY_CLASS instance; \
    }

//! Declare a ControlFactory factory so that it can be called.
//! @param FACTORY : Name of the control factory class.
#define DECLARE_GWK_CONTROL_FACTORY(FACTORY) \
    extern void GworkControlFactory##FACTORY(); \
    GworkControlFactory##FACTORY();

#define GWK_CONTROL_FACTORY_PROPERTY(PROP_NAME, DESCRIPTION) \
public: \
    Gwk::String Name() const  override { return #PROP_NAME; } \
    Gwk::String Description() const override { return DESCRIPTION; }


