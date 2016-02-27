
#include "GwenUtil/ControlFactory.h"
#include "Gwen/Controls.h"

namespace Gwen
{
    namespace ControlFactory
    {
        namespace Properties
        {
            class ImageName : public ControlFactory::Property
            {
                GWEN_CONTROL_FACTORY_PROPERTY(ImageName,
                                              "The path to the image (relative to .exe)");

                String GetValueAsString(Controls::Base* ctrl) override
                {
                    return gwen_cast<Controls::ImagePanel>(ctrl)->GetImage();
                }

                void SetValueFromString(Controls::Base* ctrl, const String& str) override
                {
                    gwen_cast<Controls::ImagePanel>(ctrl)->SetImage(str);
                }

            };


            class Stretch : public ControlFactory::PropertyBool
            {
                GWEN_CONTROL_FACTORY_PROPERTY(Stretch, "The path to the image (relative to .exe)");

                String GetValueAsString(Controls::Base* ctrl) override
                {
                    if (gwen_cast<Controls::ImagePanel>(ctrl)->GetStretch())
                        return True;

                    return False;
                }

                void SetValueFromString(Controls::Base* ctrl, const String& str) override
                {
                    bool bTrue = (str == True);
                    gwen_cast<Controls::ImagePanel>(ctrl)->SetStretch(bTrue);
                }

            };


        }

        class ImagePanel_Factory : public Gwen::ControlFactory::Base
        {
        public:

            GWEN_CONTROL_FACTORY_FOR(ImagePanel, Base)
            {
                AddProperty(new Properties::ImageName());
                AddProperty(new Properties::Stretch());
            }

            virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent) override
            {
                Gwen::Controls::ImagePanel* pControl = new Gwen::Controls::ImagePanel(parent);
                pControl->SetSize(100, 20);
                return pControl;
            }

        };


        GWEN_CONTROL_FACTORY(ImagePanel_Factory);

    }
}