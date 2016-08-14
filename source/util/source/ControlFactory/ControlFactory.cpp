
#include <Gwork/Util/ControlFactory.h>

namespace Gwk {
namespace ControlFactory {
    
const Gwk::String PropertyBool::True    = "true";
const Gwk::String PropertyBool::False   = "false";

void InitialiseControls()
{
    ponder::Class::declare<Gwk::Color>()
        .property("r", &Gwk::Color::r)
        .property("g", &Gwk::Color::g)
        .property("b", &Gwk::Color::b)
        .property("a", &Gwk::Color::a)
        ;

    ponder::Class::declare<Gwk::Margin>()
        .property("top",    &Gwk::Margin::top)
        .property("bottom", &Gwk::Margin::bottom)
        .property("left",   &Gwk::Margin::left)
        .property("right",  &Gwk::Margin::right)
        ;

    ponder::Class::declare<Gwk::Point>()
        .property("x",  &Gwk::Point::x)
        .property("y",  &Gwk::Point::y)
        ;
    
    ponder::Enum::declare<Position>()
        .value("none",    Gwk::Position::None)
        .value("left",    Gwk::Position::Left)
        .value("right",   Gwk::Position::Right)
        .value("top",     Gwk::Position::Top)
        .value("bottom",  Gwk::Position::Bottom)
        .value("midy",    Gwk::Position::CenterV)
        .value("midx",    Gwk::Position::CenterH)
        .value("center",  Gwk::Position::Center)
        .value("fill",    Gwk::Position::Fill)
        ;
    
    ponder::Class::declare<Gwk::Align>()
        .function("placeBelow", &Gwk::Align::PlaceBelow)
        ;

    ponder::Class::declare<Gwk::Event::Listener>()
        .function("add", &Gwk::Event::Listener::AddCb)
        ;

    //
    // Controls::Base
    //
    ponder::Class::declare<Controls::Base>()
        .function("typename",   &Controls::Base::GetTypeName)
        .property("name",       &Controls::Base::GetName, &Controls::Base::SetName)
        .property("pos",        &Controls::Base::GetPos,
                  static_cast<void(Controls::Base::*)(const Point&)>(&Controls::Base::SetPos))
        .property("size",       &Controls::Base::GetSize,
                                [] (Controls::Base& c,const Point& s) {c.SetSize(s);})
    
        .property("dock",       &Controls::Base::GetDock, &Controls::Base::Dock)
        .property("margin",     &Controls::Base::GetMargin, &Controls::Base::SetMargin)
        .property("tooltip",    &Controls::Base::GetToolTip, &Controls::Base::SetToolTip)
        .function("setTooltip", &Controls::Base::SetToolTipText)
        ;

    ponder::Class::declare<Controls::Label>()
        .base<Controls::Label::ParentClass>()
        .constructor<Controls::Base*>()
        .property("text", &Controls::Label::GetText,
                          // setter has >1 params
                          [] (Controls::Label &c, const Gwk::String &s) { c.SetText(s); })
        .property("textColor",  &Controls::Label::GetTextColor, &Controls::Label::SetTextColor)
        ;

    ponder::Class::declare<Controls::Button>()
        .base<Controls::Button::ParentClass>()
        .constructor<Controls::Base*>()
        .property("depressed",  &Controls::Button::IsDepressed, &Controls::Button::SetDepressed)
        .property("isToggle",   &Controls::Button::IsToggle, &Controls::Button::SetIsToggle)
        .property("toggled",    &Controls::Button::GetToggleState,
                                &Controls::Button::SetToggleState)
        .property("onPress",        &Controls::Button::onPress)
        .property("onRightPress",   &Controls::Button::onRightPress)
        .property("onDown",         &Controls::Button::onDown)
        .property("onUp",           &Controls::Button::onUp)
        .property("onDoubleClick",  &Controls::Button::onDoubleClick)
        .property("onToggle",       &Controls::Button::onToggle)
        .property("onToggleOn",     &Controls::Button::onToggleOn)
        .property("onToggleOff",    &Controls::Button::onToggleOff)
        ;

    ponder::Class::declare<Controls::LabelClickable>()
        .base<Controls::LabelClickable::ParentClass>()
        ;

    ponder::Class::declare<Controls::TextBox>()
        .base<Controls::TextBox::ParentClass>()
        ;

    ponder::Class::declare<Controls::TextBoxMultiline>()
        .base<Controls::TextBoxMultiline::ParentClass>()
        ;

    ponder::Class::declare<Controls::CheckBox>()
        .base<Controls::CheckBox::ParentClass>()
        ;

    ponder::Class::declare<Controls::CheckBoxWithLabel>()
        .base<Controls::CheckBoxWithLabel::ParentClass>()
        .property("label", &Controls::CheckBoxWithLabel::Label)
        ;

    ponder::Class::declare<Controls::ScrollControl>()
        .base<Controls::ScrollControl::ParentClass>()
        .property("canHorizScroll", &Controls::ScrollControl::CanScrollH)
        .property("canVertScroll", &Controls::ScrollControl::CanScrollV)
        .function("setScroll", &Controls::ScrollControl::SetScroll)
        ;

    ponder::Class::declare<Controls::Menu>()
        .base<Controls::Menu::ParentClass>()
        .function("addItem", &Controls::Menu::AddItem)
        .function("addDivider", &Controls::Menu::AddDivider)
        ;

    ponder::Class::declare<Controls::MenuStrip>()
        .base<Controls::MenuStrip::ParentClass>()
        ;

    ponder::Class::declare<Controls::ListBox>()
        .base<Controls::ListBox::ParentClass>()
        ;

    ponder::Class::declare<Controls::ImagePanel>()
        .base<Controls::ImagePanel::ParentClass>()
        ;

    ponder::Class::declare<Controls::Rectangle>()
        .base<Controls::Rectangle::ParentClass>()
        .property("backColor", &Controls::Rectangle::GetColor, &Controls::Rectangle::SetColor)
        ;

    ponder::Class::declare<Controls::ProgressBar>()
        .base<Controls::Rectangle::ParentClass>()
        .property("cycleSpeed", &Controls::ProgressBar::GetCycleSpeed,
                                &Controls::ProgressBar::SetCycleSpeed)
        ;

    ponder::Class::declare<Controls::ComboBox>()
        .base<Controls::ComboBox::ParentClass>()
        ;

    //
    // This sucks, but I don't think it's possible to get the factory
    // method working inside a static lib without some shady unportable compiler capers
    //
//            DECLARE_GWK_CONTROL_FACTORY(Base_Factory);
//            DECLARE_GWK_CONTROL_FACTORY(Label_Factory);
//            DECLARE_GWK_CONTROL_FACTORY(LabelClickable_Factory);
//    DECLARE_GWK_CONTROL_FACTORY(Button_Factory);
//    DECLARE_GWK_CONTROL_FACTORY(TextBox_Factory);
//    DECLARE_GWK_CONTROL_FACTORY(TextBoxMultiline_Factory);
//    DECLARE_GWK_CONTROL_FACTORY(CheckBox_Factory);
//    DECLARE_GWK_CONTROL_FACTORY(CheckBoxWithLabel_Factory);
//    DECLARE_GWK_CONTROL_FACTORY(MenuStrip_Factory);
//    DECLARE_GWK_CONTROL_FACTORY(ListBox_Factory);
//    // DECLARE_GWK_CONTROL_FACTORY( VerticalSplitter_Factory );
//    DECLARE_GWK_CONTROL_FACTORY(ImagePanel_Factory);
//    DECLARE_GWK_CONTROL_FACTORY(FilePicker_Factory);
//    DECLARE_GWK_CONTROL_FACTORY(FolderPicker_Factory);
//    DECLARE_GWK_CONTROL_FACTORY(PageControl_Factory);
//    DECLARE_GWK_CONTROL_FACTORY(Rectangle_Factory);
//    DECLARE_GWK_CONTROL_FACTORY(ProgressBar_Factory);
//    DECLARE_GWK_CONTROL_FACTORY(ComboBox_Factory);
//    DECLARE_GWK_CONTROL_FACTORY(HorizontalSlider_Factory);
//    DECLARE_GWK_CONTROL_FACTORY(DesignerCanvas_Factory);
}

List& GetList()
{
    static List list;
    static bool Initialized = false;

    if (!Initialized)
    {
        Initialized = true;
//        InitialiseControls();
    }

    return list;
}

ControlFactory::Base* Find(const Gwk::String& name)
{
    for (auto const& control : ControlFactory::GetList())
    {
        if (control->Name() == name)
            return control;
    }

    return nullptr;
}

Base::Base()
{
    GetList().push_back(this);
}

void Base::AddProperty(Property* prop)
{
    m_properties.push_back(prop);
}

Base* Base::GetBaseFactory()
{
    for (auto const& control : ControlFactory::GetList())
    {
        if (control->Name() == ParentFactory())
            return control;
    }

    return nullptr;
}

Property* Base::GetProperty(const Gwk::String& name)
{
    for (auto const& property : Properties())
    {
        if (property->Name() != name)
            continue;

        return property;
    }

    Base* base = GetBaseFactory();

    if (!base)
        return nullptr;

    return base->GetProperty(name);
}

void Base::SetControlValue(Gwk::Controls::Base* ctrl, const Gwk::String& name,
                           const Gwk::String& str)
{
    Property* prop = GetProperty(name);

    if (!prop)
    {
        Base* base = GetBaseFactory();

        if (!base)
            return;

        return base->SetControlValue(ctrl, name, str);
    }

    prop->SetValueFromString(ctrl, str);
}

void Base::AddChild(Gwk::Controls::Base* ctrl, Gwk::Controls::Base* child,
                    const Gwk::Point& pos)
{
    child->SetParent(ctrl);
}

void Base::AddChild(Gwk::Controls::Base* ctrl, Gwk::Controls::Base* child, int iPage)
{
    child->SetParent(ctrl);
}

Controls::Base* Clone(Controls::Base* source, ControlFactory::Base* factory)
{
    Controls::Base* control = factory->CreateInstance(source->GetParent());

    while (factory)
    {
	for (auto&& property : factory->Properties())
        {
            property->SetValueFromString(control, property->GetValueAsString(source));
        }

        factory = factory->GetBaseFactory();
    }

    return control;
}

void Base::SetParentPage(Gwk::Controls::Base* ctrl, int i)
{
    ctrl->UserData.Set<int>("ParentPage", i);
}

int Base::GetParentPage(Gwk::Controls::Base* ctrl)
{
    if (!ctrl->UserData.Exists("ParentPage"))
        return 0;

    return ctrl->UserData.Get<int>("ParentPage");
}

} // namespace ControlFactory
} // namespace Gwk
