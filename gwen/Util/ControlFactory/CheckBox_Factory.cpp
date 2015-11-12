
#include "GwenUtil/ControlFactory.h"
#include "Gwen/Controls.h"

namespace Gwen
{
//    namespace ControlFactory
//    {
//        class CheckBox_Factory : public Gwen::ControlFactory::Base
//        {
//        public:
//
//            GWEN_CONTROL_FACTORY_FOR(CheckBox, Base)
//            {
//            }
//
//            virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent)
//            {
//                Gwen::Controls::CheckBox* pControl = new Gwen::Controls::CheckBox(parent);
//                return pControl;
//            }
//
//        };
//
//        GWEN_CONTROL_FACTORY(CheckBox_Factory);
//
//
//        namespace Properties
//        {
//            class CheckboxText : public ControlFactory::Property
//            {
//                GWEN_CONTROL_FACTORY_PROPERTY(CheckboxText, "The text, or label of the control");
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    return gwen_cast<Controls::CheckBoxWithLabel>(ctrl)->Label()->GetText();
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    gwen_cast<Controls::CheckBoxWithLabel>(ctrl)->Label()->SetText(str);
//                }
//
//            };
//        }
//
//        class CheckBoxWithLabel_Factory : public Gwen::ControlFactory::Base
//        {
//        public:
//
//            GWEN_CONTROL_FACTORY_FOR(CheckBoxWithLabel, Base)
//            {
//                AddProperty(new Properties::CheckboxText());
//            }
//
//            virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent)
//            {
//                Gwen::Controls::CheckBoxWithLabel* pControl =
//                    new Gwen::Controls::CheckBoxWithLabel(parent);
//                return pControl;
//            }
//
//        };
//
//
//        GWEN_CONTROL_FACTORY(CheckBoxWithLabel_Factory);
//
//    }
}
