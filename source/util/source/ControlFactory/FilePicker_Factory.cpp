
#include <Gwork/Util/ControlFactory.h>
#include <Gwork/Controls.h>

namespace Gwk
{
    namespace ControlFactory
    {
        using namespace Gwk;

//        namespace Properties
//        {
//            using namespace Gwk;
//
//            class FileType : public ControlFactory::Property
//            {
//                GWK_CONTROL_FACTORY_PROPERTY(FileType, "In the format \"PNG file | *.png\"");
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    return gwk_cast<Controls::FilePicker>(ctrl)->GetFileType();
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    gwk_cast<Controls::FilePicker>(ctrl)->SetFileType(str);
//                }
//
//            };
//        }

        class FilePicker_Factory : public Gwk::ControlFactory::Base
        {
        public:

            GWK_CONTROL_FACTORY_FOR(FilePicker, Base)
            {
//                ponder::Class::declare<Controls::FilePicker>()
//                    .base<Controls::Base>()
//                    ;

//                AddProperty(new Properties::FileType());
            }

            Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent) override
            {
                Gwk::Controls::FilePicker* control = new Gwk::Controls::FilePicker(parent);
                control->SetSize(100, 20);
                control->SetFileType("EXE file | *.exe");
                return control;
            }

        };


        GWK_CONTROL_FACTORY(FilePicker_Factory);

    }
}
