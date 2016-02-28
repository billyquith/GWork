
#include "Gwork/Util/ControlFactory.h"
#include "Gwork/Controls.h"

namespace Gwk
{
//    namespace ControlFactory
//    {
//        using namespace Gwk;
//
//        namespace Properties
//        {
//            class Color : public ControlFactory::Property
//            {
//                GWK_CONTROL_FACTORY_PROPERTY(Color, "Rectangle's Background Color");
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    Controls::Rectangle* pRect = gwk_cast<Controls::Rectangle>(ctrl);
//                    return Utility::Format("%i %i %i %i", pRect->GetColor().r,
//                                           pRect->GetColor().g,
//                                           pRect->GetColor().b, pRect->GetColor().a);
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    Controls::Rectangle* pRect = gwk_cast<Controls::Rectangle>(ctrl);
//                    int r, g, b, a;
//
//                    if (sscanf(str.c_str(), "%i %i %i %i", &r, &g, &b, &a) != 4)
//                        return;
//
//                    pRect->SetColor(Gwk::Color(r, g, b, a));
//                }
//
//                size_t NumCount() const override
//                {
//                    return 4;
//                }
//
//                Gwk::String NumName(size_t i) const override
//                {
//                    if (i == 0)
//                        return "r";
//
//                    if (i == 1)
//                        return "g";
//
//                    if (i == 2)
//                        return "b";
//
//                    return "a";
//                }
//
//                float NumGet(Controls::Base* ctrl, int i) override
//                {
//                    Controls::Rectangle* pRect = gwk_cast<Controls::Rectangle>(ctrl);
//
//                    if (i == 0)
//                        return pRect->GetColor().r;
//
//                    if (i == 1)
//                        return pRect->GetColor().g;
//
//                    if (i == 2)
//                        return pRect->GetColor().b;
//
//                    return pRect->GetColor().a;
//                }
//
//                void NumSet(Controls::Base* ctrl, int i, float f) override
//                {
//                    Controls::Rectangle* pRect = gwk_cast<Controls::Rectangle>(ctrl);
//                    Gwk::Color c = pRect->GetColor();
//
//                    if (i == 0)
//                        c.r = f;
//
//                    if (i == 1)
//                        c.g = f;
//
//                    if (i == 2)
//                        c.b = f;
//
//                    if (i == 3)
//                        c.a = f;
//
//                    pRect->SetColor(c);
//                }
//
//            };
//
//
//        }
//
//
//        class Rectangle_Factory : public Gwk::ControlFactory::Base
//        {
//        public:
//
//            GWK_CONTROL_FACTORY_FOR(Rectangle, Base)
//            {
//                AddProperty(new Properties::Color());
//            }
//
//            virtual Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent) override
//            {
//                Gwk::Controls::Rectangle* pControl = new Gwk::Controls::Rectangle(parent);
//                pControl->SetSize(100, 100);
//                return pControl;
//            }
//
//        };
//
//
//        GWK_CONTROL_FACTORY(Rectangle_Factory);
//
//    }
}
