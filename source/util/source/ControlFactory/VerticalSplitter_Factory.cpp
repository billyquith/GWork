
#include <Gwork/Util/ControlFactory.h>
#include <Gwork/Controls.h>

namespace Gwk
{
    namespace ControlFactory
    {
        using namespace Gwk;

        class SplitterVertical_Factory : public Gwk::ControlFactory::Base
        {
        public:

            GWK_CONTROL_FACTORY_FOR(SplitterVertical, Base)
            {
            }

            virtual Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent) override
            {
                Gwk::Controls::SplitterVertical* control =
                    new Gwk::Controls::SplitterVertical(parent);
                control->SetSize(200, 200);
                return control;
            }

            void AddChild(Gwk::Controls::Base* ctrl, Gwk::Controls::Base* child, Gwk::Point& pos)
            {
                Gwk::Controls::SplitterVertical* splitter =
                    gwk_cast<Gwk::Controls::SplitterVertical>(ctrl);
                child->SetParent(splitter);

                if (pos.y < splitter->SplitterPos())
                    splitter->SetPanels(child, NULL);
                else
                    splitter->SetPanels(NULL, child);
            }

            void AddChild(Gwk::Controls::Base* ctrl, Gwk::Controls::Base* child, int iPage)
            {
                child->SetParent(ctrl);
            }

        };


        GWK_CONTROL_FACTORY(SplitterVertical_Factory);

    }
}
