#include <Gwork/Util/ControlFactory.h>
#include <Gwork/Controls.h>

//namespace Gwk
//{
//    namespace ControlFactory
//    {
//        namespace Properties
//        {
//            
//            class ControlName : public Gwk::ControlFactory::Property
//            {
//                GWK_CONTROL_FACTORY_PROPERTY(ControlName, "The control's name");
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    return Utility::Format("%S", ctrl->GetName().c_str());
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    ctrl->SetName(str);
//                }
//
//            };
//            
//            enum PositionEnum { X, Y };
//
//            static const ValueEnumMapItem<PositionEnum> g_posEnums[] = {
//                { "x", X },
//                { "y", Y }
//            };

//            class Position : public Gwk::ControlFactory::Property
//            {
//                typedef ValueEnumMap<PositionEnum> EnumMap;
//                EnumMap m_EnumMap;
//                
//            public:
//                GWK_CONTROL_FACTORY_PROPERTY(Position, "Sets the position of the control");
//                
//                Position()
//                :   m_EnumMap(g_posEnums, GWK_ARRAY_COUNT(g_posEnums))
//                {}
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    return Utility::Format("%i %i", ctrl->X(), ctrl->Y());
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    int x, y;
//
//                    if (sscanf(str.c_str(), "%i %i", &x, &y) == 2)
//                    {
//                        ctrl->SetPos(x, y);
//                    }
//                }
//
//                size_t NumCount() const override
//                {
//                    return m_EnumMap.GetEnumCount();
//                }
//
//                Gwk::String NumName(size_t i) const override
//                {
//                    return m_EnumMap.GetNameByIndex(i);
//                }
//
//                float NumGet(Controls::Base* ctrl, int i) override
//                {
//                    return i == 0 ? ctrl->X() : ctrl->Y();
//                }
//
//                void NumSet(Controls::Base* ctrl, int i, float f) override
//                {
//                    return i == 0
//                        ? ctrl->SetPos(f, ctrl->Y())
//                        : ctrl->SetPos(ctrl->X(), f);
//                }
//            };


//            class Margin : public Gwk::ControlFactory::Property
//            {
//                GWK_CONTROL_FACTORY_PROPERTY(Margin, "Sets the margin of a docked control");
//
//                String GetValueAsString(Controls::Base* ctrl)
//                {
//                    Gwk::Margin m = ctrl->GetMargin();
//                    return Utility::Format("%i %i %i %i", m.left, m.top, m.right, m.bottom);
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    Gwk::Margin m;
//
//                    if (sscanf(str.c_str(), "%i %i %i %i", &m.left, &m.top, &m.right,
//                                &m.bottom) != 4)
//                        return;
//
//                    ctrl->SetMargin(m);
//                }
//
//                size_t NumCount() const override
//                {
//                    return 4;
//                }
//
//                Gwk::String NumName(size_t i) const override
//                {
//                    if (i == 0)
//                        return "left";
//
//                    if (i == 1)
//                        return "top";
//
//                    if (i == 2)
//                        return "right";
//
//                    return "bottom";
//                }
//
//                float NumGet(Controls::Base* ctrl, int i) override
//                {
//                    Gwk::Margin m = ctrl->GetMargin();
//
//                    if (i == 0)
//                        return m.left;
//
//                    if (i == 1)
//                        return m.top;
//
//                    if (i == 2)
//                        return m.right;
//
//                    return m.bottom;
//                }
//
//                void NumSet(Controls::Base* ctrl, int i, float f) override
//                {
//                    Gwk::Margin m = ctrl->GetMargin();
//
//                    if (i == 0)
//                        m.left = f;
//
//                    if (i == 1)
//                        m.top = f;
//
//                    if (i == 2)
//                        m.right = f;
//
//                    if (i == 3)
//                        m.bottom = f;
//
//                    ctrl->SetMargin(m);
//                }
//
//            };


//            class Size : public Gwk::ControlFactory::Property
//            {
//                GWK_CONTROL_FACTORY_PROPERTY(Size, "The with and height of the control");
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    return Utility::Format("%i %i", ctrl->Width(), ctrl->Height());
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    int w, h;
//
//                    if (sscanf(str.c_str(), "%i %i", &w, &h) != 2)
//                        return;
//
//                    ctrl->SetSize(w, h);
//                }
//
//                size_t NumCount() const override
//                {
//                    return 2;
//                }
//
//                Gwk::String NumName(size_t i) const override
//                {
//                    if (i == 0)
//                        return "w";
//
//                    return "h";
//                }
//
//                float NumGet(Controls::Base* ctrl, int i) override
//                {
//                    if (i == 0)
//                        return ctrl->Width();
//
//                    return ctrl->Height();
//                }
//
//                void NumSet(Controls::Base* ctrl, int i, float f) override
//                {
//                    if (i == 0)
//                        ctrl->SetSize(f, ctrl->Height());
//                    else
//                        ctrl->SetSize(ctrl->Width(), f);
//                }
//
//            };
            
            
//            static const ValueEnumMapItem<Docking::Area> g_DockEnums[] = {
//                { "None",   Gwk::Docking::None },
//                { "Fill",   Gwk::Docking::Fill },
//                { "Left",   Gwk::Docking::Left },
//                { "Right",  Gwk::Docking::Right },
//                { "Top",    Gwk::Docking::Top },
//                { "Bottom", Gwk::Docking::Bottom },
//            };
//            
//            class Dock : public Gwk::ControlFactory::Property
//            {
//                typedef ValueEnumMap<Gwk::Docking::Area> EnumMap;
//                EnumMap m_EnumMap;
//                
//            public:
//                GWK_CONTROL_FACTORY_PROPERTY(Dock, "How the control is to be docked");
//                
//                Dock()
//                :   m_EnumMap(g_DockEnums, GWK_ARRAY_COUNT(g_DockEnums))
//                {}
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    return m_EnumMap.GetNameByValue(ctrl->GetDock());
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    Docking::Area dock = m_EnumMap.GetValueByName(str.c_str(), Docking::None);
//                    ctrl->Dock(dock);
//                }
//
//                size_t OptionCount() const override
//                {
//                    return m_EnumMap.GetEnumCount();
//                }
//
//                Gwk::String OptionGet(int i) override
//                {
//                    return m_EnumMap.GetNameByIndex(i);
//                }
//                
//            };
//            
//        } // namespace Properties


//        class Base_Factory : public Gwk::ControlFactory::Base
//        {
//        public:
//
//            GWK_CONTROL_FACTORY_FOR(Base,)
//            {
////                AddProperty(new Properties::ControlName());
////                AddProperty(new Properties::Dock());
////                AddProperty(new Properties::Position());
////                AddProperty(new Properties::Size());
////                AddProperty(new Properties::Margin());                
//            }
//
//            virtual Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent)
//            {
//                Gwk::Controls::Base* pControl = new Gwk::Controls::Base(parent);
//                pControl->SetSize(100, 100);
//                return pControl;
//            }
//        };
//
//
//        GWK_CONTROL_FACTORY(Base_Factory);
//
//    }
//}
