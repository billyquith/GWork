
#include <Gwork/Util/ControlFactory.h>
#include <Gwork/Controls.h>


namespace Gwk
{
    namespace ControlFactory
    {
        using namespace Gwk;

        namespace Properties
        {
            class NumPages : public ControlFactory::Property
            {
                GWK_CONTROL_FACTORY_PROPERTY(NumPages, "The number of pages we have");

                String GetValueAsString(Controls::Base* ctrl) override
                {
                    return Utility::Format("%i", (int)gwk_cast<Controls::PageControl>(
                                               ctrl)->GetPageCount());
                }

                void SetValueFromString(Controls::Base* ctrl, const String& str) override
                {
                    int num;

                    if (sscanf(str.c_str(), "%i", &num) != 1)
                        return;

                    gwk_cast<Controls::PageControl>(ctrl)->SetPageCount(num);
                }

            };


            class FinishName : public ControlFactory::Property
            {
                GWK_CONTROL_FACTORY_PROPERTY(FinishName, "The name of the finish button");

                String GetValueAsString(Controls::Base* ctrl) override
                {
                    Gwk::Controls::PageControl* pControl =
                        gwk_cast<Gwk::Controls::PageControl>(ctrl);
                    return pControl->FinishButton()->GetName();
                }

                void SetValueFromString(Controls::Base* ctrl, const String& str) override
                {
                    Gwk::Controls::PageControl* pControl =
                        gwk_cast<Gwk::Controls::PageControl>(ctrl);
                    pControl->FinishButton()->SetName(str);
                }

            };


        }

        class PageControl_Factory : public Gwk::ControlFactory::Base
        {
        public:

            GWK_CONTROL_FACTORY_FOR(PageControl, Base)
            {
                AddProperty(new Properties::NumPages());
                AddProperty(new Properties::FinishName());
            }

            virtual Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent) override
            {
                Gwk::Controls::PageControl* pControl = new Gwk::Controls::PageControl(parent);
                pControl->SetSize(300, 300);
                pControl->SetPageCount(1);
                return pControl;
            }

            //
            // Called when a child is clicked on in the editor
            //
            virtual bool ChildTouched(Gwk::Controls::Base* ctrl,
                                      Gwk::Controls::Base* pChildControl)
            {
                Gwk::Controls::PageControl* pControl = gwk_cast<Gwk::Controls::PageControl>(
                    ctrl);

                if (pChildControl == pControl->NextButton())
                {
                    pControl->NextButton()->DoAction(); return true;
                }

                if (pChildControl == pControl->BackButton())
                {
                    pControl->BackButton()->DoAction(); return true;
                }

                return false;
            }

            //
            // A child is being dropped on this position.. set the parent
            // properly
            //
            void AddChild(Gwk::Controls::Base* ctrl, Gwk::Controls::Base* child,
                          const Gwk::Point& pos)
            {
                Gwk::Controls::PageControl* pControl = gwk_cast<Gwk::Controls::PageControl>(
                    ctrl);
                AddChild(ctrl, child, pControl->GetPageNumber());
            }

            void AddChild(Gwk::Controls::Base* ctrl, Gwk::Controls::Base* child, int iPage)
            {
                Gwk::Controls::PageControl* pControl = gwk_cast<Gwk::Controls::PageControl>(
                    ctrl);

                if (!pControl->GetPage(iPage))
                    iPage = 0;

                SetParentPage(child, iPage);
                child->SetParent(pControl->GetPage(iPage));
            }

        };


        GWK_CONTROL_FACTORY(PageControl_Factory);

    }
}
