
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
                Gwk::Controls::SplitterVertical* pControl =
                    new Gwk::Controls::SplitterVertical(parent);
                pControl->SetSize(200, 200);
                return pControl;
            }

            void AddChild(Gwk::Controls::Base* ctrl, Gwk::Controls::Base* child, Gwk::Point& pos)
            {
                Gwk::Controls::SplitterVertical* pSplitter =
                    gwk_cast<Gwk::Controls::SplitterVertical>(ctrl);
                child->SetParent(pSplitter);

                if (pos.y < pSplitter->SplitterPos())
                    pSplitter->SetPanels(child, NULL);
                else
                    pSplitter->SetPanels(NULL, child);
            }

            void AddChild(Gwk::Controls::Base* ctrl, Gwk::Controls::Base* child, int iPage)
            {
                child->SetParent(ctrl);
            }

        };


        GWK_CONTROL_FACTORY(SplitterVertical_Factory);

    }
}
