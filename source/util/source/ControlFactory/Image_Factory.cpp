
#include <Gwork/Util/ControlFactory.h>
#include <Gwork/Controls.h>

namespace Gwk
{
    namespace ControlFactory
    {
        namespace Properties
        {
            class ImageName : public ControlFactory::Property
            {
                GWK_CONTROL_FACTORY_PROPERTY(ImageName,
                                             "The path to the image (relative to .exe)");

                String GetValueAsString(Controls::Base* ctrl) override
                {
                    return gwk_cast<Controls::ImagePanel>(ctrl)->GetImage();
                }

                void SetValueFromString(Controls::Base* ctrl, const String& str) override
                {
                    gwk_cast<Controls::ImagePanel>(ctrl)->SetImage(str);
                }

            };


            class Stretch : public ControlFactory::PropertyBool
            {
                GWK_CONTROL_FACTORY_PROPERTY(Stretch, "The path to the image (relative to .exe)");

                String GetValueAsString(Controls::Base* ctrl) override
                {
                    if (gwk_cast<Controls::ImagePanel>(ctrl)->GetStretch())
                        return True;

                    return False;
                }

                void SetValueFromString(Controls::Base* ctrl, const String& str) override
                {
                    bool bTrue = (str == True);
                    gwk_cast<Controls::ImagePanel>(ctrl)->SetStretch(bTrue);
                }

            };


        }

        class ImagePanel_Factory : public Gwk::ControlFactory::Base
        {
        public:

            GWK_CONTROL_FACTORY_FOR(ImagePanel, Base)
            {
                AddProperty(new Properties::ImageName());
                AddProperty(new Properties::Stretch());
            }

            Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent) override
            {
                Gwk::Controls::ImagePanel* control = new Gwk::Controls::ImagePanel(parent);
                control->SetSize(100, 20);
                return control;
            }

        };


        GWK_CONTROL_FACTORY(ImagePanel_Factory);

    }
}
