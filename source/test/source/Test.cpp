/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/Test.h>
#include <Gwork/Controls/DockedTabControl.h>
#include <Gwork/Controls/WindowControl.h>
#include <Gwork/Controls/CollapsibleList.h>
#include <Gwork/Controls/Layout/Position.h>
#include <Gwork/Platform.h>
#include <Gwork/Hook.h>
//#include <Gwork/Util/ControlFactory.h>

//#include <ponder/class.hpp>
////#include <ponder-xml/rapidxml.hpp>
//#include <ponder/classvisitor.hpp>
//#include <ponder/simpleproperty.hpp>
//#include <ponder/enumproperty.hpp>
//#include <ponder/userproperty.hpp>
//#include <ponder/enum.hpp>

using namespace Gwk;

Gwk::Controls::TabButton* button = NULL;


//class MyVistor : public ponder::ClassVisitor
//{
//    Controls::Properties *m_props;
//public:
//    MyVistor(Controls::Properties *pt) : m_props(pt) {}
//    
//    void visit(const ponder::SimpleProperty& property) override
//    {
//        m_props->Add("property", property.name());
//    }
//    
//    void visit(const ponder::ArrayProperty& property) override
//    {
//        m_props->Add("array", property.name());
//    }
//    
//    void visit(const ponder::EnumProperty& property) override
//    {
//        m_props->Add("enum", property.name());
//    }
//    
//    void visit(const ponder::UserProperty& property) override
//    {
//        m_props->Add("user", property.name());
//    }
//    
//    void visit(const ponder::Function& function) override
//    {
//        m_props->Add("function", function.name());
//    }
//};

//static void PopulateClassInfo(Controls::PropertyTree *tree)
//{    
//    for (std::size_t clsi=0; clsi < ponder::classCount(); ++clsi)
//    {
//        const ponder::Class &c = ponder::classByIndex(clsi);
//        auto props = tree->Add(c.name());
//        MyVistor mv(props);
//        c.visit(mv);
//    }
//}
//
//class ControlListener : public Gwk::Hook::BaseHook
//{
//    Controls::Base *m_lastOver;
//public:
//    ControlListener() : m_lastOver(NULL), m_props(NULL), m_parent(NULL) {}
//
//    Controls::PropertyTree *m_props;
//    Controls::Base *m_parent;
//
//    void updateProperties();
//
//    bool OnControlClicked(Gwk::Controls::Base* c, int /*iMouseX*/, int /*iMouseY*/) override
//    {
//        if (c && c != m_lastOver)
//        {
//            // make sure we have the common parent
//            auto cp = c;
//            while (cp != NULL && cp != m_parent)
//            {
//                cp = cp->GetParent();
//            }
//            
//            if (cp == m_parent)
//            {
//                m_lastOver = c;
//                updateProperties();
//            }
//        }
//        return false;
//    }
//};
//
//static std::string userToString(const ponder::UserObject& obj, const ponder::Property& prop)
//{
//    std::string vstr;
//
//    if (obj.pointer() == NULL)
//        return vstr;
//    
//    printf("prop: %s %s", prop.name().c_str(), ponder::detail::typeAsString(prop.type()));
//    
//    auto val = prop.get(obj);
//    if (val.type() == ponder::userType)
//    {
//        const ponder::UserObject& child = prop.get(obj).to<ponder::UserObject>();
//        const ponder::Class& ccls = child.getClass();
//        for (std::size_t ci = 0, nb = ccls.propertyCount(); ci < nb; ++ci)
//        {
//            vstr += userToString(child, ccls.property(ci));
//            if (ci < nb-1)
//                vstr += ",";
//        }
//    }
//    else
//    {
//        vstr = val.to<std::string>();
//    }
//    
//    return vstr;
//}
//
//void ControlListener::updateProperties()
//{
//    m_props->Clear();
//    
//    try {
//        const ponder::Class& cls = ponder::classByName(std::string("Gwk::Controls::") + m_lastOver->GetTypeName());
//        auto props = m_props->Add(cls.name());
//        const ponder::UserObject uobj(*m_lastOver);
//        
//        for (auto i = 0u; i < cls.propertyCount(); ++i)
//        {
//            const ponder::Property& prop = cls.property(i);
//            std::string vstr = userToString(ponder::UserObject(*m_lastOver), prop);
//            props->Add(prop.name(), vstr);
//        }
//    }
//    catch (ponder::ClassNotFound) {
//        m_props->Add(std::string("Unknown class: ") + m_lastOver->GetTypeName());
//    }
//    
//    m_props->ExpandAll();
//}
//
GWK_CONTROL_CONSTRUCTOR(UnitTest)
{
    m_lastControl = NULL;
    Dock(Docking::Fill);
    SetSize(1024, 768);

//    Controls::CollapsibleList* scriptList = new Controls::CollapsibleList(this);
//    GetLeft()->GetTabControl()->AddPage("Script", scriptList);
    
    Controls::CollapsibleList* apiList = new Controls::CollapsibleList(this);
    GetLeft()->GetTabControl()->AddPage("API", apiList);

    GetLeft()->SetWidth(150);

    // Create control property viewer
    m_controlProperties = new Controls::PropertyTree(GetRight());
    auto treeCampInfo = new Controls::PropertyTree(GetRight());
    button = GetRight()->GetTabControl()->AddPage("Properties", m_controlProperties);
    GetRight()->GetTabControl()->AddPage("Classes", treeCampInfo);
//    PopulateClassInfo(treeCampInfo);

    // Create output log and status bar.
    m_textOutput = new Controls::ListBox(GetBottom());
    button = GetBottom()->GetTabControl()->AddPage("Output", m_textOutput);
    GetBottom()->SetHeight(200);
    m_statusBar = new Controls::StatusBar(this);
    m_statusBar->Dock(Docking::Bottom);

    // Where to put the demo controls.
//    auto center = new ControlContainer(this);
    auto center = new Controls::Layout::Center(this);
    center->Dock(Docking::Fill);
//    auto listen = new ControlListener();
//    listen->m_props = m_controlProperties;
//    listen->m_parent = center;
//    Hook::AddHook(listen);

#define ADD_UNIT_TEST(NAME) \
    GUnit* RegisterUnitTest_##NAME(Gwk::Controls::Base *tab); \
    { \
        Controls::Button* button = cat->Add(#NAME); \
        button->SetName(#NAME); \
        GUnit* test = RegisterUnitTest_##NAME(center); \
        test->Hide(); \
        test->SetUnitTest(this); \
        button->onPress.Add(this, &ThisClass::OnCategorySelect, Gwk::Event::Packet(test)); \
    }

    // Create Controls using Gwork API.
    {
        Controls::CollapsibleCategory* cat = apiList->Add("Basic");
        ADD_UNIT_TEST(Button);
        ADD_UNIT_TEST(Label);
        ADD_UNIT_TEST(LabelMultiline);
    }
    {
        Controls::CollapsibleCategory* cat = apiList->Add("Non-Interactive");
        ADD_UNIT_TEST(ProgressBar);
        ADD_UNIT_TEST(GroupBox);
        ADD_UNIT_TEST(ImagePanel);
        ADD_UNIT_TEST(StatusBar);
    }
    {
        Controls::CollapsibleCategory* cat = apiList->Add("Controls");
        ADD_UNIT_TEST(ComboBox);
        ADD_UNIT_TEST(TextBox);
        ADD_UNIT_TEST(ListBox);
        ADD_UNIT_TEST(CrossSplitter);
        ADD_UNIT_TEST(RadioButton);
        ADD_UNIT_TEST(Checkbox);
        ADD_UNIT_TEST(Numeric);
        ADD_UNIT_TEST(Slider);
        ADD_UNIT_TEST(MenuStrip);
    }
    {
        Controls::CollapsibleCategory* cat = apiList->Add("Containers");
        ADD_UNIT_TEST(Window);
        ADD_UNIT_TEST(TreeControl);
        ADD_UNIT_TEST(Properties);
        ADD_UNIT_TEST(TabControl);
        ADD_UNIT_TEST(ScrollControl);
        ADD_UNIT_TEST(PageControl);
    }
    {
        Controls::CollapsibleCategory* cat = apiList->Add("Non-Standard");
        ADD_UNIT_TEST(CollapsibleList);
        ADD_UNIT_TEST(ColorPicker);
    }

    // Create Controls using script.
//    {
//        Controls::CollapsibleCategory* cat = scriptList->Add("Basic");
//        ADD_UNIT_TEST(Button);
////        ADD_UNIT_TEST(Label);
////        ADD_UNIT_TEST(LabelMultiline);
//    }
    
    m_statusBar->SendToBack();
    PrintText("Unit Test Started.");
    m_fLastSecond = Gwk::Platform::GetTimeInSeconds();
    m_frames = 0;
    
    apiList->GetNamedChildren("MenuStrip").DoAction();

    PrintText(Utility::Format("Size of Button = %lu bytes.", sizeof(Gwk::Controls::Button)));
}

void UnitTest::OnCategorySelect(Gwk::Event::Info info)
{
    if (m_lastControl)
        m_lastControl->Hide();

    info.Packet->Control->Show();
    m_lastControl = info.Packet->Control;
    
    if (m_controlProperties)
    {
        m_controlProperties->Clear();
    }
}

void UnitTest::PrintText(const Gwk::String& str)
{
    m_textOutput->AddItem(str);
    m_textOutput->ScrollToBottom();
}

void UnitTest::Render(Gwk::Skin::Base* skin)
{
    m_frames++;

    if (m_fLastSecond < Gwk::Platform::GetTimeInSeconds())
    {
        m_statusBar->SetText(Gwk::Utility::Format("Gwork Unit Test - %i fps", m_frames*2));
        m_fLastSecond = Gwk::Platform::GetTimeInSeconds()+0.5f;
        m_frames = 0;
    }

    ParentClass::Render(skin);
}

void GUnit::UnitPrint(Gwk::String str)
{
    m_unitTest->PrintText(str);
}

