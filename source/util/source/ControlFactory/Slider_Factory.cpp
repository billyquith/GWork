
#include <Gwork/Util/ControlFactory.h>
#include <Gwork/Controls.h>


namespace Gwk
{
    namespace ControlFactory
    {
        using namespace Gwk;

        namespace Properties
        {
            class Min : public ControlFactory::Property
            {
                GWK_CONTROL_FACTORY_PROPERTY(Min, "The minimum value");

                String GetValueAsString(Controls::Base* ctrl) override
                {
                    return Gwk::Utility::Format("%f", (int)gwk_cast<Controls::Slider>(
                                                     ctrl)->GetMin());
                }

                void SetValueFromString(Controls::Base* ctrl, const String& str) override
                {
                    float val;

                    if (sscanf(str.c_str(), "%f", &val) != 1)
                        return;

                    if (val == gwk_cast<Controls::Slider>(ctrl)->GetMin())
                        return;

                    gwk_cast<Controls::Slider>(ctrl)->SetRange(val, gwk_cast<Controls::Slider>(
                                                                    ctrl)->GetMax());
                }

            };


            class Max : public ControlFactory::Property
            {
                GWK_CONTROL_FACTORY_PROPERTY(Max, "The max value");

                String GetValueAsString(Controls::Base* ctrl) override
                {
                    return Gwk::Utility::Format("%f", gwk_cast<Controls::Slider>(ctrl)->GetMax());
                }

                void SetValueFromString(Controls::Base* ctrl, const String& str) override
                {
                    float val;

                    if (sscanf(str.c_str(), "%f", &val) != 1)
                        return;

                    if (val == gwk_cast<Controls::Slider>(ctrl)->GetMax())
                        return;

                    gwk_cast<Controls::Slider>(ctrl)->SetRange(gwk_cast<Controls::Slider>(
                                                                    ctrl)->GetMin(), val);
                }

            };


        }

        class HorizontalSlider_Factory : public Gwk::ControlFactory::Base
        {
        public:

            GWK_CONTROL_FACTORY_FOR(HorizontalSlider, Base)
            {
                AddProperty(new Properties::Min());
                AddProperty(new Properties::Max());
            }

            Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent) override
            {
                Gwk::Controls::HorizontalSlider* control =
                    new Gwk::Controls::HorizontalSlider(parent);
                control->SetSize(100, 20);
                control->SetRange(0, 1);
                return control;
            }

        };


        GWK_CONTROL_FACTORY(HorizontalSlider_Factory);

    }
}
