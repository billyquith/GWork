
#include <GwenUtil/ControlFactory.h>

namespace Gwen {
namespace ControlFactory {
    
const Gwen::String PropertyBool::True    = "true";
const Gwen::String PropertyBool::False   = "false";

void InitializeControls()
{
    camp::Class::declare<Gwen::Color>()
        .property("a", &Gwen::Color::a)
        .property("r", &Gwen::Color::r)
        .property("g", &Gwen::Color::g)
        .property("b", &Gwen::Color::b)
        ;

    camp::Class::declare<Gwen::Margin>()
        .property("top",    &Gwen::Margin::top)
        .property("bottom", &Gwen::Margin::bottom)
        .property("left",   &Gwen::Margin::left)
        .property("right",  &Gwen::Margin::right)
        ;

    camp::Class::declare<Gwen::Point>()
        .property("x",  &Gwen::Point::x)
        .property("y",  &Gwen::Point::y)
        ;
    
    camp::Enum::declare<Docking::Area>()
        .value("none",   Gwen::Docking::None)
        .value("fill",   Gwen::Docking::Fill)
        .value("left",   Gwen::Docking::Left)
        .value("right",  Gwen::Docking::Right)
        .value("top",    Gwen::Docking::Top)
        .value("bottom", Gwen::Docking::Bottom)
        ;

    camp::Class::declare<Controls::Base>()
        .function("typename",   &Controls::Base::GetTypeName)
        .property("name",       &Controls::Base::GetName, &Controls::Base::SetName)
        .property("pos",        &Controls::Base::GetPos,
                                static_cast<void(Controls::Base::*)(const Point&)>(&Controls::Base::SetPos))
        .property("size",       &Controls::Base::GetSize,
                                [] (Controls::Base& c,const Point& s) {c.SetSize(s);})
        .property("margin",     &Controls::Base::GetMargin, &Controls::Base::SetMargin)
        .property("dock",       &Controls::Base::GetDock, &Controls::Base::Dock)
        ;

    camp::Class::declare<Controls::Label>()
        .base<Controls::Label::ParentClass>()
        .property("text", &Controls::Label::GetText, [] (Controls::Label& c, const Gwen::String& s) { c.SetText(s); })
        .property("textColor", &Controls::Label::GetTextColor, &Controls::Label::SetTextColor)
        ;

    camp::Class::declare<Controls::Button>()
        .base<Controls::Button::ParentClass>()
        .property("depressed", &Controls::Button::IsDepressed, &Controls::Button::SetDepressed)
        .property("isToggle", &Controls::Button::IsToggle, &Controls::Button::SetIsToggle)
        .property("toggled", &Controls::Button::GetToggleState, &Controls::Button::SetToggleState)
        ;

    camp::Class::declare<Controls::LabelClickable>()
        .base<Controls::LabelClickable::ParentClass>()
        ;

    camp::Class::declare<Controls::TextBox>()
        .base<Controls::TextBox::ParentClass>()
        ;

    camp::Class::declare<Controls::TextBoxMultiline>()
        .base<Controls::TextBoxMultiline::ParentClass>()
        ;

    camp::Class::declare<Controls::CheckBox>()
        .base<Controls::CheckBox::ParentClass>()
        ;

    camp::Class::declare<Controls::CheckBoxWithLabel>()
        .base<Controls::CheckBoxWithLabel::ParentClass>()
        .property("label", &Controls::CheckBoxWithLabel::Label)
        ;

    camp::Class::declare<Controls::ScrollControl>()
        .base<Controls::ScrollControl::ParentClass>()
        .property("canHorizScroll", &Controls::ScrollControl::CanScrollH)
        .property("canVertScroll", &Controls::ScrollControl::CanScrollV)
        .function("setScroll", &Controls::ScrollControl::SetScroll)
        ;

    camp::Class::declare<Controls::Menu>()
        .base<Controls::Menu::ParentClass>()
        .function("addItem", &Controls::Menu::AddItem)
        .function("addDivider", &Controls::Menu::AddDivider)
        ;

    camp::Class::declare<Controls::MenuStrip>()
        .base<Controls::MenuStrip::ParentClass>()
        ;

    camp::Class::declare<Controls::ListBox>()
        .base<Controls::ListBox::ParentClass>()
        ;

    camp::Class::declare<Controls::ImagePanel>()
        .base<Controls::ImagePanel::ParentClass>()
        ;

    camp::Class::declare<Controls::Rectangle>()
        .base<Controls::Rectangle::ParentClass>()
        .property("backColor", &Controls::Rectangle::GetColor, &Controls::Rectangle::SetColor)
        ;

    camp::Class::declare<Controls::ProgressBar>()
        .base<Controls::Rectangle::ParentClass>()
        .property("cycleSpeed", &Controls::ProgressBar::GetCycleSpeed, &Controls::ProgressBar::SetCycleSpeed)
        ;

    camp::Class::declare<Controls::ComboBox>()
        .base<Controls::ComboBox::ParentClass>()
        ;

    //
    // This sucks, but I don't think it's possible to get the factory
    // method working inside a static lib without some shady unportable compiler capers
    //
//            DECLARE_GWEN_CONTROL_FACTORY(Base_Factory);
//            DECLARE_GWEN_CONTROL_FACTORY(Label_Factory);
//            DECLARE_GWEN_CONTROL_FACTORY(LabelClickable_Factory);
//    DECLARE_GWEN_CONTROL_FACTORY(Button_Factory);
//    DECLARE_GWEN_CONTROL_FACTORY(TextBox_Factory);
//    DECLARE_GWEN_CONTROL_FACTORY(TextBoxMultiline_Factory);
//    DECLARE_GWEN_CONTROL_FACTORY(CheckBox_Factory);
//    DECLARE_GWEN_CONTROL_FACTORY(CheckBoxWithLabel_Factory);
//    DECLARE_GWEN_CONTROL_FACTORY(MenuStrip_Factory);
//    DECLARE_GWEN_CONTROL_FACTORY(ListBox_Factory);
//    // DECLARE_GWEN_CONTROL_FACTORY( VerticalSplitter_Factory );
//    DECLARE_GWEN_CONTROL_FACTORY(ImagePanel_Factory);
//    DECLARE_GWEN_CONTROL_FACTORY(FilePicker_Factory);
//    DECLARE_GWEN_CONTROL_FACTORY(FolderPicker_Factory);
//    DECLARE_GWEN_CONTROL_FACTORY(PageControl_Factory);
//    DECLARE_GWEN_CONTROL_FACTORY(Rectangle_Factory);
//    DECLARE_GWEN_CONTROL_FACTORY(ProgressBar_Factory);
//    DECLARE_GWEN_CONTROL_FACTORY(ComboBox_Factory);
//    DECLARE_GWEN_CONTROL_FACTORY(HorizontalSlider_Factory);
//    DECLARE_GWEN_CONTROL_FACTORY(DesignerCanvas_Factory);
}

List& GetList()
{
    static List list;
    static bool Initialized = false;

    if (!Initialized)
    {
        Initialized = true;
        InitializeControls();
    }

    return list;
}

ControlFactory::Base* Find(const Gwen::String& name)
{
    for (ControlFactory::List::iterator it = ControlFactory::GetList().begin();
         it != ControlFactory::GetList().end();
         ++it)
    {
        if ((*it)->Name() == name)
            return *it;
    }

    return NULL;
}

Base::Base()
{
    GetList().push_back(this);
}

void Base::AddProperty(Property* pProp)
{
    m_Properties.push_back(pProp);
}

Base* Base::GetBaseFactory()
{
    for (ControlFactory::List::iterator it = ControlFactory::GetList().begin();
         it != ControlFactory::GetList().end();
         ++it)
    {
        if ((*it)->Name() == ParentFactory())
            return *it;
    }

    return NULL;
}

Property* Base::GetProperty(const Gwen::String& name)
{
    for (ControlFactory::Property::List::const_iterator it = Properties().begin(),
         itEnd = Properties().end();
         it != itEnd; ++it)
    {
        if ((*it)->Name() != name)
            continue;

        return *it;
    }

    Base* pBase = GetBaseFactory();

    if (!pBase)
        return NULL;

    return pBase->GetProperty(name);
}

void Base::SetControlValue(Gwen::Controls::Base* ctrl, const Gwen::String& name,
                           const Gwen::String& str)
{
    Property* pProp = GetProperty(name);

    if (!pProp)
    {
        Base* pBase = GetBaseFactory();

        if (!pBase)
            return;

        return pBase->SetControlValue(ctrl, name, str);
    }

    pProp->SetValueFromString(ctrl, str);
}

void Base::AddChild(Gwen::Controls::Base* ctrl, Gwen::Controls::Base* child,
                    const Gwen::Point& pos)
{
    child->SetParent(ctrl);
}

void Base::AddChild(Gwen::Controls::Base* ctrl, Gwen::Controls::Base* child, int iPage)
{
    child->SetParent(ctrl);
}

Controls::Base* Clone(Controls::Base* pSource, ControlFactory::Base* pFactory)
{
    Controls::Base* pControl = pFactory->CreateInstance(pSource->GetParent());

    while (pFactory)
    {
        for (ControlFactory::Property::List::const_iterator
             it = pFactory->Properties().begin(), itEnd = pFactory->Properties().end();
             it != itEnd; ++it)
        {
            (*it)->SetValueFromString(pControl, (*it)->GetValueAsString(pSource));
        }

        pFactory = pFactory->GetBaseFactory();
    }

    return pControl;
}

void Base::SetParentPage(Gwen::Controls::Base* ctrl, int i)
{
    ctrl->UserData.Set<int>("ParentPage", i);
}

int Base::GetParentPage(Gwen::Controls::Base* ctrl)
{
    if (!ctrl->UserData.Exists("ParentPage"))
        return 0;

    return ctrl->UserData.Get<int>("ParentPage");
}

}
}
