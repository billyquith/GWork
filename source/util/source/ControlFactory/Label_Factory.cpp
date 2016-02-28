
#include <Gwork/Util/ControlFactory.h>
#include <Gwork/Controls.h>

namespace Gwk
{
//    namespace ControlFactory
//    {
//        namespace Properties
//        {
//            class Text : public ControlFactory::Property
//            {
//                GWK_CONTROL_FACTORY_PROPERTY(Text, "The text, or label of the control");
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    String str = gwk_cast<Controls::Label>(ctrl)->GetText();
//                    Gwk::Utility::Replace(str, "\n", "\\n");
//                    Gwk::Utility::Replace(str, "\t", "\\t");
//                    return str;
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    String strOut = str;
//                    Gwk::Utility::Replace(strOut, "\\n", "\n");
//                    Gwk::Utility::Replace(strOut, "\\t", "\t");
//                    gwk_cast<Controls::Label>(ctrl)->SetText(strOut);
//                }
//
//            };
//
//
//            class Font : public ControlFactory::Property
//            {
//                GWK_CONTROL_FACTORY_PROPERTY(Font, "The font name");
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    return gwk_cast<Controls::Label>(ctrl)->GetFont()->facename;
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    if (str == "")
//                        return;
//
//                    Gwk::Font* pFont = gwk_cast<Controls::Label>(ctrl)->GetFont();
//                    gwk_cast<Controls::Label>(ctrl)->SetFont(str, pFont->size, pFont->bold);
//                }
//
//            };
//
//
//            class FontSize : public ControlFactory::Property
//            {
//                GWK_CONTROL_FACTORY_PROPERTY(FontSize, "The font size");
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    return Gwk::Utility::Format("%i", (int)gwk_cast<Controls::Label>(
//                                                     ctrl)->GetFont()->size);
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    int size;
//
//                    if (sscanf(str.c_str(), "%i", &size) != 1)
//                        return;
//
//                    Gwk::Font* pFont = gwk_cast<Controls::Label>(ctrl)->GetFont();
//
//                    if (size == pFont->size)
//                        return;
//
//                    gwk_cast<Controls::Label>(ctrl)->SetFont(pFont->facename, size, pFont->bold);
//                }
//
//            };
//
//
//            class FontBold : public ControlFactory::PropertyBool
//            {
//                GWK_CONTROL_FACTORY_PROPERTY(FontBold, "The font bold");
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    if (gwk_cast<Controls::Label>(ctrl)->GetFont()->bold)
//                        return True;
//
//                    return False;
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    bool bTrue = (str == True);
//                    Gwk::Font* pFont = gwk_cast<Controls::Label>(ctrl)->GetFont();
//
//                    if (bTrue == pFont->bold)
//                        return;
//
//                    gwk_cast<Controls::Label>(ctrl)->SetFont(pFont->facename, pFont->size,
//                                                              bTrue ? true : false);
//                }
//
//            };
//
//
//            class Wrap : public ControlFactory::PropertyBool
//            {
//                GWK_CONTROL_FACTORY_PROPERTY(Wrap, "Wrap the text");
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    if (gwk_cast<Controls::Label>(ctrl)->Wrap())
//                        return True;
//
//                    return False;
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    bool bTrue = (str == True);
//
//                    if (bTrue == gwk_cast<Controls::Label>(ctrl)->Wrap())
//                        return;
//
//                    gwk_cast<Controls::Label>(ctrl)->SetWrap(bTrue);
//                }
//
//            };
//
//
//            class VerticalAlign : public ControlFactory::Property
//            {
//                GWK_CONTROL_FACTORY_PROPERTY(VerticalAlign, "VerticalAlign");
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    const int align = gwk_cast<Controls::Label>(ctrl)->GetAlignment();
//                    
//                    if (align & Docking::Top)
//                        return "Top";
//
//                    if (align & Docking::CenterV)
//                        return "Center";
//
//                    if (align & Docking::Bottom)
//                        return "Bottom";
//
//                    return "Top";
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    int align = gwk_cast<Controls::Label>(ctrl)->GetAlignment();
//                    align &= ~Docking::Top;
//                    align &= ~Docking::CenterV;
//                    align &= ~Docking::Bottom;
//
//                    if (str == "Top")
//                        gwk_cast<Controls::Label>(ctrl)->SetAlignment(align|Docking::Top);
//                    else if (str == "Center")
//                        gwk_cast<Controls::Label>(ctrl)->SetAlignment(align|Docking::CenterV);
//                    else if (str == "Bottom")
//                        gwk_cast<Controls::Label>(ctrl)->SetAlignment(align|Docking::Bottom);
//                }
//
//                size_t OptionCount() const override
//                {
//                    return 3;
//                }
//
//                Gwk::String OptionGet(int i) override
//                {
//                    if (i == 0)
//                        return "Top";
//
//                    if (i == 1)
//                        return "Center";
//
//                    return "Bottom";
//                }
//
//            };
//
//
//            class HorizontalAlign : public ControlFactory::Property
//            {
//                GWK_CONTROL_FACTORY_PROPERTY(HorizontalAlign, "HorizontalAlign");
//
//                String GetValueAsString(Controls::Base* ctrl) override
//                {
//                    if (gwk_cast<Controls::Label>(ctrl)->GetAlignment() & Docking::Left)
//                        return "Left";
//
//                    if (gwk_cast<Controls::Label>(ctrl)->GetAlignment() & Docking::CenterH)
//                        return "Center";
//
//                    if (gwk_cast<Controls::Label>(ctrl)->GetAlignment() & Docking::Right)
//                        return "Right";
//
//                    return "Left";
//                }
//
//                void SetValueFromString(Controls::Base* ctrl, const String& str) override
//                {
//                    int iAlign = gwk_cast<Controls::Label>(ctrl)->GetAlignment();
//                    iAlign &= ~Docking::Left;
//                    iAlign &= ~Docking::CenterH;
//                    iAlign &= ~Docking::Right;
//
//                    if (str == "Left")
//                        gwk_cast<Controls::Label>(ctrl)->SetAlignment(iAlign|Docking::Left);
//
//                    if (str == "Center")
//                        gwk_cast<Controls::Label>(ctrl)->SetAlignment(iAlign|Docking::CenterH);
//
//                    if (str == "Right")
//                        gwk_cast<Controls::Label>(ctrl)->SetAlignment(iAlign|Docking::Right);
//                }
//
//                size_t OptionCount() const override
//                {
//                    return 3;
//                }
//
//                Gwk::String OptionGet(int i) override
//                {
//                    if (i == 0)
//                        return "Left";
//
//                    if (i == 1)
//                        return "Center";
//
//                    return "Right";
//                }
//
//            };
//
//
//        } // namespace Properties

        
//        class Label_Factory : public Gwk::ControlFactory::Base
//        {
//        public:
//
//            GWK_CONTROL_FACTORY_FOR(Label, Base)
//            {
////                AddProperty(new Properties::Text());
////                AddProperty(new Properties::Wrap());
////                AddProperty(new Properties::Font());
////                AddProperty(new Properties::FontSize());
////                AddProperty(new Properties::FontBold());
////                AddProperty(new Properties::VerticalAlign());
////                AddProperty(new Properties::HorizontalAlign());
//            }
//
//            virtual Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent) override
//            {
//                Gwk::Controls::Label* pControl = new Gwk::Controls::Label(parent);
//                pControl->SetSize(100, 20);
//                pControl->SetText("New Label");
//                return pControl;
//            }
//
//        };
//
//        GWK_CONTROL_FACTORY(Label_Factory);
//        
//        
//        class LabelClickable_Factory : public Gwk::ControlFactory::Base
//        {
//        public:
//
//            GWK_CONTROL_FACTORY_FOR(LabelClickable, Label)
//            {
//            }
//
//            virtual Gwk::Controls::Base* CreateInstance(Gwk::Controls::Base* parent) override
//            {
//                Gwk::Controls::Label* pControl = new Gwk::Controls::LabelClickable(parent);
//                pControl->SetSize(100, 20);
//                pControl->SetText("LabelClickable");
//                return pControl;
//            }
//
//        };
//
//        GWK_CONTROL_FACTORY(LabelClickable_Factory);
//
//    }
}
