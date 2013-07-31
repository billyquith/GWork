
#include "Gwen/Util/ControlFactory.h"
#include "Gwen/Controls.h"

namespace Gwen
{
    namespace ControlFactory
    {
        using namespace Gwen;

        namespace Properties
        {
            class ControlName : public Gwen::ControlFactory::Property
            {
                GWEN_CONTROL_FACTORY_PROPERTY(ControlName, "The control's name");

                String GetValue(Controls::Base* ctrl)
                {
                    return Utility::Format("%S", ctrl->GetName().c_str());
                }

                void SetValue(Controls::Base* ctrl, const String& str)
                {
                    ctrl->SetName(str);
                }

            };


            class Position : public Gwen::ControlFactory::Property
            {
                GWEN_CONTROL_FACTORY_PROPERTY(Position, "Sets the position of the control");

                String GetValue(Controls::Base* ctrl)
                {
                    return Utility::Format("%i %i", ctrl->X(), ctrl->Y());
                }

                void SetValue(Controls::Base* ctrl, const String& str)
                {
                    int x, y;

                    if (sscanf(str.c_str(), "%i %i", &x, &y) != 2)
                        return;

                    ctrl->SetPos(x, y);
                }

                int NumCount()
                {
                    return 2;
                }

                Gwen::String NumName(int i)
                {
                    if (i == 0)
                        return "x";

                    return "y";
                }

                float NumGet(Controls::Base* ctrl, int i)
                {
                    if (i == 0)
                        return ctrl->X();

                    return ctrl->Y();
                }

                void NumSet(Controls::Base* ctrl, int i, float f)
                {
                    if (i == 0)
                        ctrl->SetPos(f, ctrl->Y());
                    else
                        ctrl->SetPos(ctrl->X(), f);
                }

            };


            class Margin : public Gwen::ControlFactory::Property
            {
                GWEN_CONTROL_FACTORY_PROPERTY(Margin, "Sets the margin of a docked control");

                String GetValue(Controls::Base* ctrl)
                {
                    Gwen::Margin m = ctrl->GetMargin();
                    return Utility::Format("%i %i %i %i", m.left, m.top, m.right, m.bottom);
                }

                void SetValue(Controls::Base* ctrl, const String& str)
                {
                    Gwen::Margin m;

                    if (sscanf(str.c_str(), "%i %i %i %i", &m.left, &m.top, &m.right,
                                &m.bottom) != 4)
                        return;

                    ctrl->SetMargin(m);
                }

                int NumCount()
                {
                    return 4;
                }

                Gwen::String NumName(int i)
                {
                    if (i == 0)
                        return "left";

                    if (i == 1)
                        return "top";

                    if (i == 2)
                        return "right";

                    return "bottom";
                }

                float NumGet(Controls::Base* ctrl, int i)
                {
                    Gwen::Margin m = ctrl->GetMargin();

                    if (i == 0)
                        return m.left;

                    if (i == 1)
                        return m.top;

                    if (i == 2)
                        return m.right;

                    return m.bottom;
                }

                void NumSet(Controls::Base* ctrl, int i, float f)
                {
                    Gwen::Margin m = ctrl->GetMargin();

                    if (i == 0)
                        m.left = f;

                    if (i == 1)
                        m.top = f;

                    if (i == 2)
                        m.right = f;

                    if (i == 3)
                        m.bottom = f;

                    ctrl->SetMargin(m);
                }

            };


            class Size : public Gwen::ControlFactory::Property
            {
                GWEN_CONTROL_FACTORY_PROPERTY(Size, "The with and height of the control");

                String GetValue(Controls::Base* ctrl)
                {
                    return Utility::Format("%i %i", ctrl->Width(), ctrl->Height());
                }

                void SetValue(Controls::Base* ctrl, const String& str)
                {
                    int w, h;

                    if (sscanf(str.c_str(), "%i %i", &w, &h) != 2)
                        return;

                    ctrl->SetSize(w, h);
                }

                int NumCount()
                {
                    return 2;
                }

                Gwen::String NumName(int i)
                {
                    if (i == 0)
                        return "w";

                    return "h";
                }

                float NumGet(Controls::Base* ctrl, int i)
                {
                    if (i == 0)
                        return ctrl->Width();

                    return ctrl->Height();
                }

                void NumSet(Controls::Base* ctrl, int i, float f)
                {
                    if (i == 0)
                        ctrl->SetSize(f, ctrl->Height());
                    else
                        ctrl->SetSize(ctrl->Width(), f);
                }

            };


            class Dock : public Gwen::ControlFactory::Property
            {
                GWEN_CONTROL_FACTORY_PROPERTY(Dock, "How the control is to be docked");

                String GetValue(Controls::Base* ctrl)
                {
                    switch (ctrl->GetDock())
                    {
                    case Pos::Left:
                        return "Left";

                    case Pos::Fill:
                        return "Fill";

                    case Pos::Right:
                        return "Right";

                    case Pos::Top:
                        return "Top";

                    case Pos::Bottom:
                        return "Bottom";
                    }

                    return "None";
                }

                void SetValue(Controls::Base* ctrl, const String& str)
                {
                    if (str == "Fill")
                        ctrl->Dock(Pos::Fill);

                    if (str == "Left")
                        ctrl->Dock(Pos::Left);

                    if (str == "Right")
                        ctrl->Dock(Pos::Right);

                    if (str == "Top")
                        ctrl->Dock(Pos::Top);

                    if (str == "Bottom")
                        ctrl->Dock(Pos::Bottom);

                    if (str == "None")
                        ctrl->Dock(Pos::None);
                }

                int OptionNum()
                {
                    return 6;
                }

                Gwen::String OptionGet(int i)
                {
                    if (i == 0)
                        return "None";

                    if (i == 1)
                        return "Left";

                    if (i == 2)
                        return "Right";

                    if (i == 3)
                        return "Top";

                    if (i == 4)
                        return "Bottom";

                    return "Fill";
                }

            };


        } // namespace Properties


        class Base_Factory : public Gwen::ControlFactory::Base
        {
        public:

            GWEN_CONTROL_FACTORY_CONSTRUCTOR(Base_Factory, Gwen::ControlFactory::Base)
            {
                AddProperty(new Properties::ControlName());
                AddProperty(new Properties::Dock());
                AddProperty(new Properties::Position());
                AddProperty(new Properties::Size());
                AddProperty(new Properties::Margin());
            }

            virtual Gwen::String Name()
            {
                return "Base";
            }

            virtual Gwen::String BaseName()
            {
                return "";
            }

            virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent)
            {
                Gwen::Controls::Base* pControl = new Gwen::Controls::Base(parent);
                pControl->SetSize(100, 100);
                return pControl;
            }

        };


        GWEN_CONTROL_FACTORY(Base_Factory);

    }
}
