
#include "GwenUtil/ControlFactory.h"
#include "Gwen/Controls.h"

namespace Gwen
{
    namespace ControlFactory
    {
        class MenuStrip_Factory : public Gwen::ControlFactory::Base
        {
        public:

            GWEN_CONTROL_FACTORY_FOR(MenuStrip, Base)
            {
            }

            virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent) override
            {
                Gwen::Controls::MenuStrip* pControl = new Gwen::Controls::MenuStrip(parent);
                return pControl;
            }

        };


        GWEN_CONTROL_FACTORY(MenuStrip_Factory);

    }
}
