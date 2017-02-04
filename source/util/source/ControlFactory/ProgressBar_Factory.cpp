
#include <Gwork/Util/ControlFactory.h>
#include <Gwork/Controls.h>

namespace Gwk
{
//    namespace ControlFactory
//    {
//        using namespace Gwk;
//
//        namespace Properties
//        {
//            class CycleSpeed : public ControlFactory::Property
//            {
//                GWK_CONTROL_FACTORY_PROPERTY(CycleSpeed, "");
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    return Utility::Format("%f",static_cast<float>(gwk_cast<Controls::ProgressBar>(
//                                               ctrl)->GetCycleSpeed()));
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    float num;
//
//                    if (sscanf(str.c_str(), "%f", &num) != 1)
//                        return;
//
//                    gwk_cast<Controls::ProgressBar>(ctrl)->SetCycleSpeed(num);
//                }
//
//            };
//
//
//        }
//
//        class ProgressBar_Factory : public Gwk::ControlFactory::Base
//        {
//        public:
//
//            GWK_CONTROL_FACTORY_FOR(ProgressBar, Base)
//            {
//                AddProperty(new Properties::CycleSpeed());
//            }
//
//            Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent) override
//            {
//                Gwk::Controls::ProgressBar* control = new Gwk::Controls::ProgressBar(parent);
//                control->SetSize(200, 20);
//                return control;
//            }
//
//        };
//
//
//        GWK_CONTROL_FACTORY(ProgressBar_Factory);
//
//    }
}
