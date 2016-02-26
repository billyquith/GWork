
#include "GwenUtil/ControlFactory.h"
#include "Gwen/Controls.h"


namespace Gwen
{
    namespace ControlFactory
    {
        using namespace Gwen;

        namespace Properties
        {
            class NumPages : public ControlFactory::Property
            {
                GWEN_CONTROL_FACTORY_PROPERTY(NumPages, "The number of pages we have");

                String GetValueAsString(Controls::Base* ctrl) override
                {
                    return Utility::Format("%i", (int)gwen_cast<Controls::PageControl>(
                                               ctrl)->GetPageCount());
                }

                void SetValueFromString(Controls::Base* ctrl, const String& str) override
                {
                    int num;

                    if (sscanf(str.c_str(), "%i", &num) != 1)
                        return;

                    gwen_cast<Controls::PageControl>(ctrl)->SetPageCount(num);
                }

            };


            class FinishName : public ControlFactory::Property
            {
                GWEN_CONTROL_FACTORY_PROPERTY(FinishName, "The name of the finish button");

                String GetValueAsString(Controls::Base* ctrl) override
                {
                    Gwen::Controls::PageControl* pControl =
                        gwen_cast<Gwen::Controls::PageControl>(ctrl);
                    return pControl->FinishButton()->GetName();
                }

                void SetValueFromString(Controls::Base* ctrl, const String& str) override
                {
                    Gwen::Controls::PageControl* pControl =
                        gwen_cast<Gwen::Controls::PageControl>(ctrl);
                    pControl->FinishButton()->SetName(str);
                }

            };


        }

        class PageControl_Factory : public Gwen::ControlFactory::Base
        {
        public:

            GWEN_CONTROL_FACTORY_FOR(PageControl, Base)
            {
                AddProperty(new Properties::NumPages());
                AddProperty(new Properties::FinishName());
            }

            virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent) override
            {
                Gwen::Controls::PageControl* pControl = new Gwen::Controls::PageControl(parent);
                pControl->SetSize(300, 300);
                pControl->SetPageCount(1);
                return pControl;
            }

            //
            // Called when a child is clicked on in the editor
            //
            virtual bool ChildTouched(Gwen::Controls::Base* ctrl,
                                      Gwen::Controls::Base* pChildControl)
            {
                Gwen::Controls::PageControl* pControl = gwen_cast<Gwen::Controls::PageControl>(
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
            void AddChild(Gwen::Controls::Base* ctrl, Gwen::Controls::Base* child,
                          const Gwen::Point& pos)
            {
                Gwen::Controls::PageControl* pControl = gwen_cast<Gwen::Controls::PageControl>(
                    ctrl);
                AddChild(ctrl, child, pControl->GetPageNumber());
            }

            void AddChild(Gwen::Controls::Base* ctrl, Gwen::Controls::Base* child, int iPage)
            {
                Gwen::Controls::PageControl* pControl = gwen_cast<Gwen::Controls::PageControl>(
                    ctrl);

                if (!pControl->GetPage(iPage))
                    iPage = 0;

                SetParentPage(child, iPage);
                child->SetParent(pControl->GetPage(iPage));
            }

        };


        GWEN_CONTROL_FACTORY(PageControl_Factory);

    }
}
