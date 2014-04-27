
#include "GwenUtil/ControlFactory.h"
#include "Gwen/Controls.h"

class DesignerCanvas;

namespace Gwen
{
    namespace ControlFactory
    {
        
        // This is a container for saving designer content.
        // TODO - Move this to the designer code, or remove.
        class DesignerCanvas_Factory : public Gwen::ControlFactory::Base
        {
        public:

            GWEN_CONTROL_FACTORY_DETAILS(DesignerCanvas, DesignerCanvas, Base)

            GWEN_CONTROL_FACTORY_CONSTRUCTOR(DesignerCanvas_Factory, Base)
            {
            }

            virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent)
            {
                return NULL;
            }

        };


        GWEN_CONTROL_FACTORY(DesignerCanvas_Factory);

    }
}
