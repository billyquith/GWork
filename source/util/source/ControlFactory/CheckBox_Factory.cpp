
#include <Gwork/Util/ControlFactory.h>
#include <Gwork/Controls.h>

namespace Gwk
{
//    namespace ControlFactory
//    {
//        class CheckBox_Factory : public Gwk::ControlFactory::Base
//        {
//        public:
//
//            GWK_CONTROL_FACTORY_FOR(CheckBox, Base)
//            {
//            }
//
//            virtual Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent)
//            {
//                Gwk::Controls::CheckBox* pControl = new Gwk::Controls::CheckBox(parent);
//                return pControl;
//            }
//
//        };
//
//        GWK_CONTROL_FACTORY(CheckBox_Factory);
//
//
//        namespace Properties
//        {
//            class CheckboxText : public ControlFactory::Property
//            {
//                GWK_CONTROL_FACTORY_PROPERTY(CheckboxText, "The text, or label of the control");
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    return gwk_cast<Controls::CheckBoxWithLabel>(ctrl)->Label()->GetText();
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    gwk_cast<Controls::CheckBoxWithLabel>(ctrl)->Label()->SetText(str);
//                }
//
//            };
//        }
//
//        class CheckBoxWithLabel_Factory : public Gwk::ControlFactory::Base
//        {
//        public:
//
//            GWK_CONTROL_FACTORY_FOR(CheckBoxWithLabel, Base)
//            {
//                AddProperty(new Properties::CheckboxText());
//            }
//
//            virtual Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent)
//            {
//                Gwk::Controls::CheckBoxWithLabel* pControl =
//                    new Gwk::Controls::CheckBoxWithLabel(parent);
//                return pControl;
//            }
//
//        };
//
//
//        GWK_CONTROL_FACTORY(CheckBoxWithLabel_Factory);
//
//    }
}
