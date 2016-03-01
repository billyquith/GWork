
#include <Gwork/Util/ControlFactory.h>
#include <Gwork/Controls.h>

namespace Gwk
{
    namespace ControlFactory
    {
        using namespace Gwk;

        class FolderPicker_Factory : public Gwk::ControlFactory::Base
        {
        public:

            GWK_CONTROL_FACTORY_FOR(FolderPicker, Base)
            {
            }

            virtual Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent)
            {
                Gwk::Controls::FolderPicker* control = new Gwk::Controls::FolderPicker(parent);
                control->SetSize(100, 20);
                return control;
            }

        };


        GWK_CONTROL_FACTORY(FolderPicker_Factory);

    }
}
