
#include <Gwork/Util/ControlFactory.h>
#include <Gwork/Controls.h>

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
//                    Controls::Rectangle* rect = gwk_cast<Controls::Rectangle>(ctrl);
//                    return Utility::Format("%i %i %i %i", rect->GetColor().r,
//                                           rect->GetColor().g,
//                                           rect->GetColor().b, rect->GetColor().a);
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    Controls::Rectangle* rect = gwk_cast<Controls::Rectangle>(ctrl);
//                    int r, g, b, a;
//
//                    if (sscanf(str.c_str(), "%i %i %i %i", &r, &g, &b, &a) != 4)
//                        return;
//
//                    rect->SetColor(Gwk::Color(r, g, b, a));
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
//                    Controls::Rectangle* rect = gwk_cast<Controls::Rectangle>(ctrl);
//
//                    if (i == 0)
//                        return rect->GetColor().r;
//
//                    if (i == 1)
//                        return rect->GetColor().g;
//
//                    if (i == 2)
//                        return rect->GetColor().b;
//
//                    return rect->GetColor().a;
//                }
//
//                void NumSet(Controls::Base* ctrl, int i, float f) override
//                {
//                    Controls::Rectangle* rect = gwk_cast<Controls::Rectangle>(ctrl);
//                    Gwk::Color c = rect->GetColor();
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
//                    rect->SetColor(c);
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
//            Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent) override
//            {
//                Gwk::Controls::Rectangle* control = new Gwk::Controls::Rectangle(parent);
//                control->SetSize(100, 100);
//                return control;
//            }
//
//        };
//
//
//        GWK_CONTROL_FACTORY(Rectangle_Factory);
//
//    }
}
