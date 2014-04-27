
#include "GwenUtil/ControlFactory.h"
#include "Gwen/Controls.h"

namespace Gwen
{
    namespace ControlFactory
    {
        class ListBox_Factory : public Gwen::ControlFactory::Base
        {
        public:

            GWEN_CONTROL_FACTORY_FOR(ListBox, Base)
            {
            }

            virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent)
            {
                Gwen::Controls::ListBox* pControl = new Gwen::Controls::ListBox(parent);
                pControl->SetSize(100, 100);
                return pControl;
            }

        };


        GWEN_CONTROL_FACTORY(ListBox_Factory);

    }
}
