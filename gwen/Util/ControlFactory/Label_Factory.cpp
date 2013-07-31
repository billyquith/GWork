
#include "Gwen/Util/ControlFactory.h"
#include "Gwen/Controls.h"

namespace Gwen
{
    namespace ControlFactory
    {
        using namespace Gwen;

        namespace Properties
        {
            class Text : public ControlFactory::Property
            {
                GWEN_CONTROL_FACTORY_PROPERTY(Text, "The text, or label of the control");

                String GetValue(Controls::Base* ctrl)
                {
                    String str = gwen_cast<Controls::Label>(ctrl)->GetText();
                    Gwen::Utility::Replace(str, "\n", "\\n");
                    Gwen::Utility::Replace(str, "\t", "\\t");
                    return str;
                }

                void SetValue(Controls::Base* ctrl, const String& str)
                {
                    String strOut = str;
                    Gwen::Utility::Replace(strOut, "\\n", "\n");
                    Gwen::Utility::Replace(strOut, "\\t", "\t");
                    gwen_cast<Controls::Label>(ctrl)->SetText(strOut);
                }

            };


            class Font : public ControlFactory::Property
            {
                GWEN_CONTROL_FACTORY_PROPERTY(Font, "The font name");

                String GetValue(Controls::Base* ctrl)
                {
                    return gwen_cast<Controls::Label>(ctrl)->GetFont()->facename;
                }

                void SetValue(Controls::Base* ctrl, const String& str)
                {
                    if (str == "")
                        return;

                    Gwen::Font* pFont = gwen_cast<Controls::Label>(ctrl)->GetFont();
                    gwen_cast<Controls::Label>(ctrl)->SetFont(str, pFont->size, pFont->bold);
                }

            };


            class FontSize : public ControlFactory::Property
            {
                GWEN_CONTROL_FACTORY_PROPERTY(FontSize, "The font size");

                String GetValue(Controls::Base* ctrl)
                {
                    return Gwen::Utility::Format("%i", (int)gwen_cast<Controls::Label>(
                                                     ctrl)->GetFont()->size);
                }

                void SetValue(Controls::Base* ctrl, const String& str)
                {
                    int size;

                    if (sscanf(str.c_str(), "%i", &size) != 1)
                        return;

                    Gwen::Font* pFont = gwen_cast<Controls::Label>(ctrl)->GetFont();

                    if (size == pFont->size)
                        return;

                    gwen_cast<Controls::Label>(ctrl)->SetFont(pFont->facename, size, pFont->bold);
                }

            };


            class FontBold : public ControlFactory::PropertyBool
            {
                GWEN_CONTROL_FACTORY_PROPERTY(FontBold, "The font bold");

                String GetValue(Controls::Base* ctrl)
                {
                    if (gwen_cast<Controls::Label>(ctrl)->GetFont()->bold)
                        return True;

                    return False;
                }

                void SetValue(Controls::Base* ctrl, const String& str)
                {
                    bool bTrue = (str == True);
                    Gwen::Font* pFont = gwen_cast<Controls::Label>(ctrl)->GetFont();

                    if (bTrue == pFont->bold)
                        return;

                    gwen_cast<Controls::Label>(ctrl)->SetFont(pFont->facename, pFont->size,
                                                              bTrue ? true : false);
                }

            };


            class Wrap : public ControlFactory::PropertyBool
            {
                GWEN_CONTROL_FACTORY_PROPERTY(Wrap, "Wrap the text");

                String GetValue(Controls::Base* ctrl)
                {
                    if (gwen_cast<Controls::Label>(ctrl)->Wrap())
                        return True;

                    return False;
                }

                void SetValue(Controls::Base* ctrl, const String& str)
                {
                    bool bTrue = (str == True);

                    if (bTrue == gwen_cast<Controls::Label>(ctrl)->Wrap())
                        return;

                    gwen_cast<Controls::Label>(ctrl)->SetWrap(bTrue);
                }

            };


            class VerticalAlign : public ControlFactory::Property
            {
                GWEN_CONTROL_FACTORY_PROPERTY(VerticalAlign, "VerticalAlign");

                String GetValue(Controls::Base* ctrl)
                {
                    if (gwen_cast<Controls::Label>(ctrl)->GetAlignment()&Pos::Top)
                        return "Top";

                    if (gwen_cast<Controls::Label>(ctrl)->GetAlignment()&Pos::CenterV)
                        return "Center";

                    if (gwen_cast<Controls::Label>(ctrl)->GetAlignment()&Pos::Bottom)
                        return "Bottom";

                    return "Top";
                }

                void SetValue(Controls::Base* ctrl, const String& str)
                {
                    int iAlign = gwen_cast<Controls::Label>(ctrl)->GetAlignment();
                    iAlign &= ~Pos::Top;
                    iAlign &= ~Pos::CenterV;
                    iAlign &= ~Pos::Bottom;

                    if (str == "Top")
                        gwen_cast<Controls::Label>(ctrl)->SetAlignment(iAlign|Pos::Top);

                    if (str == "Center")
                        gwen_cast<Controls::Label>(ctrl)->SetAlignment(iAlign|Pos::CenterV);

                    if (str == "Bottom")
                        gwen_cast<Controls::Label>(ctrl)->SetAlignment(iAlign|Pos::Bottom);
                }

                int OptionNum()
                {
                    return 3;
                }

                Gwen::String OptionGet(int i)
                {
                    if (i == 0)
                        return "Top";

                    if (i == 1)
                        return "Center";

                    return "Bottom";
                }

            };


            class HorizontalAlign : public ControlFactory::Property
            {
                GWEN_CONTROL_FACTORY_PROPERTY(HorizontalAlign, "HorizontalAlign");

                String GetValue(Controls::Base* ctrl)
                {
                    if (gwen_cast<Controls::Label>(ctrl)->GetAlignment()&Pos::Left)
                        return "Left";

                    if (gwen_cast<Controls::Label>(ctrl)->GetAlignment()&Pos::CenterH)
                        return "Center";

                    if (gwen_cast<Controls::Label>(ctrl)->GetAlignment()&Pos::Right)
                        return "Right";

                    return "Left";
                }

                void SetValue(Controls::Base* ctrl, const String& str)
                {
                    int iAlign = gwen_cast<Controls::Label>(ctrl)->GetAlignment();
                    iAlign &= ~Pos::Left;
                    iAlign &= ~Pos::CenterH;
                    iAlign &= ~Pos::Right;

                    if (str == "Left")
                        gwen_cast<Controls::Label>(ctrl)->SetAlignment(iAlign|Pos::Left);

                    if (str == "Center")
                        gwen_cast<Controls::Label>(ctrl)->SetAlignment(iAlign|Pos::CenterH);

                    if (str == "Right")
                        gwen_cast<Controls::Label>(ctrl)->SetAlignment(iAlign|Pos::Right);
                }

                int OptionNum()
                {
                    return 3;
                }

                Gwen::String OptionGet(int i)
                {
                    if (i == 0)
                        return "Left";

                    if (i == 1)
                        return "Center";

                    return "Right";
                }

            };


        } // namespace Properties

        class Label_Factory : public Gwen::ControlFactory::Base
        {
        public:

            GWEN_CONTROL_FACTORY_CONSTRUCTOR(Label_Factory, ControlFactory::Base)
            {
                AddProperty(new Properties::Text());
                AddProperty(new Properties::Wrap());
                AddProperty(new Properties::Font());
                AddProperty(new Properties::FontSize());
                AddProperty(new Properties::FontBold());
                AddProperty(new Properties::VerticalAlign());
                AddProperty(new Properties::HorizontalAlign());
            }

            virtual Gwen::String Name()
            {
                return "Label";
            }

            virtual Gwen::String BaseName()
            {
                return "Base";
            }

            virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent)
            {
                Gwen::Controls::Label* pControl = new Gwen::Controls::Label(parent);
                pControl->SetSize(100, 20);
                pControl->SetText("New Label");
                return pControl;
            }

        };


        GWEN_CONTROL_FACTORY(Label_Factory);

        class LabelClickable_Factory : public Gwen::ControlFactory::Base
        {
        public:

            GWEN_CONTROL_FACTORY_CONSTRUCTOR(LabelClickable_Factory, ControlFactory::Base)
            {
            }

            virtual Gwen::String Name()
            {
                return "LabelClickable";
            }

            virtual Gwen::String BaseName()
            {
                return "Label";
            }

            virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent)
            {
                Gwen::Controls::Label* pControl = new Gwen::Controls::LabelClickable(parent);
                pControl->SetSize(100, 20);
                pControl->SetText("LabelClickable");
                return pControl;
            }

        };


        GWEN_CONTROL_FACTORY(LabelClickable_Factory);

    }
}
