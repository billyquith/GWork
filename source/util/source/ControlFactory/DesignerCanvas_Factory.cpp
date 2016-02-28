
#include <Gwork/Util/ControlFactory.h>
#include <Gwork/Controls.h>

class DesignerCanvas;

namespace Gwk
{
    namespace ControlFactory
    {
        
        // This is a container for saving designer content.
        // TODO - Move this to the designer code, or remove.
        class DesignerCanvas_Factory : public Gwk::ControlFactory::Base
        {
        public:

            GWK_CONTROL_FACTORY_DETAILS(DesignerCanvas, DesignerCanvas, Base)

            GWK_CONTROL_FACTORY_CONSTRUCTOR(DesignerCanvas_Factory, Base)
            {
            }

            virtual Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent)
            {
                return NULL;
            }

        };


        GWK_CONTROL_FACTORY(DesignerCanvas_Factory);

    }
}
