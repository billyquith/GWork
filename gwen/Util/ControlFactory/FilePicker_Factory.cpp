
#include "GwenUtil/ControlFactory.h"
#include "Gwen/Controls.h"

namespace Gwen
{
    namespace ControlFactory
    {
        using namespace Gwen;

        namespace Properties
        {
            using namespace Gwen;

            class FileType : public ControlFactory::Property
            {
                GWEN_CONTROL_FACTORY_PROPERTY(FileType, "In the format \"PNG file | *.png\"");

                String GetValueAsString(Controls::Base* ctrl) override
                {
                    return gwen_cast<Controls::FilePicker>(ctrl)->GetFileType();
                }

                void SetValueFromString(Controls::Base* ctrl, const String& str) override
                {
                    gwen_cast<Controls::FilePicker>(ctrl)->SetFileType(str);
                }

            };


        }

        class FilePicker_Factory : public Gwen::ControlFactory::Base
        {
        public:

            GWEN_CONTROL_FACTORY_FOR(FilePicker, Base)
            {
                AddProperty(new Properties::FileType());
            }

            virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent)
            {
                Gwen::Controls::FilePicker* pControl = new Gwen::Controls::FilePicker(parent);
                pControl->SetSize(100, 20);
                pControl->SetFileType("EXE file | *.exe");
                return pControl;
            }

        };


        GWEN_CONTROL_FACTORY(FilePicker_Factory);

    }
}
