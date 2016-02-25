/*
 *  GWEN
 *  Copyright (c) 2010 Facepunch Studios
 *  See license in Gwen.h
 */

#include <Gwen/UnitTest/UnitTest.h>
#include <Gwen/Controls/DockedTabControl.h>
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/CollapsibleList.h>
#include <Gwen/Controls/Layout/Position.h>
#include <Gwen/Platform.h>
#include <Gwen/Hook.h>
#include <GwenUtil/ControlFactory.h>

#include <ponder/class.hpp>
//#include <ponder-xml/rapidxml.hpp>
#include <ponder/classvisitor.hpp>
#include <ponder/simpleproperty.hpp>
#include <ponder/enumproperty.hpp>
#include <ponder/userproperty.hpp>
#include <ponder/enum.hpp>

using namespace Gwen;

Gwen::Controls::TabButton* pButton = NULL;


class MyVistor : public ponder::ClassVisitor
{
    Controls::Properties *m_props;
public:
    MyVistor(Controls::Properties *pt) : m_props(pt) {}
    
    void visit(const ponder::SimpleProperty& property) override
    {
        m_props->Add("property", property.name());
    }
    
    void visit(const ponder::ArrayProperty& property) override
    {
        m_props->Add("array", property.name());
    }
    
    void visit(const ponder::EnumProperty& property) override
    {
        m_props->Add("enum", property.name());
    }
    
    void visit(const ponder::UserProperty& property) override
    {
        m_props->Add("user", property.name());
    }
    
    void visit(const ponder::Function& function) override
    {
        m_props->Add("function", function.name());
    }
};

static void PopulateClassInfo(Controls::PropertyTree *tree)
{    
    for (std::size_t clsi=0; clsi < ponder::classCount(); ++clsi)
    {
        const ponder::Class &c = ponder::classByIndex(clsi);
        auto props = tree->Add(c.name());
        MyVistor mv(props);
        c.visit(mv);
    }
}

class ControlListener : public Gwen::Hook::BaseHook
{
    Controls::Base *m_lastOver;
public:
    ControlListener() : m_lastOver(NULL), m_props(NULL), m_parent(NULL) {}

    Controls::PropertyTree *m_props;
    Controls::Base *m_parent;

    void updateProperties();

    bool OnControlClicked(Gwen::Controls::Base* c, int /*iMouseX*/, int /*iMouseY*/) override
    {
        if (c && c != m_lastOver)
        {
            // make sure we have the common parent
            auto cp = c;
            while (cp != NULL && cp != m_parent)
            {
                cp = cp->GetParent();
            }
            
            if (cp == m_parent)
            {
                m_lastOver = c;
                updateProperties();
            }
        }
        return false;
    }
};

static std::string userToString(const ponder::UserObject& obj, const ponder::Property& prop)
{
    std::string vstr;

    if (obj.pointer() == NULL)
        return vstr;
    
    printf("prop: %s %s", prop.name().c_str(), ponder::detail::typeAsString(prop.type()));
    
    auto val = prop.get(obj);
    if (val.type() == ponder::userType)
    {
        const ponder::UserObject& child = prop.get(obj).to<ponder::UserObject>();
        const ponder::Class& ccls = child.getClass();
        for (std::size_t ci = 0, nb = ccls.propertyCount(); ci < nb; ++ci)
        {
            vstr += userToString(child, ccls.property(ci));
            if (ci < nb-1)
                vstr += ",";
        }
    }
    else
    {
        vstr = val.to<std::string>();
    }
    
    return vstr;
}

void ControlListener::updateProperties()
{
    m_props->Clear();
    
    try {
        const ponder::Class& cls = ponder::classByName(std::string("Gwen::Controls::") + m_lastOver->GetTypeName());
        auto props = m_props->Add(cls.name());
        const ponder::UserObject uobj(*m_lastOver);
        
        for (auto i = 0u; i < cls.propertyCount(); ++i)
        {
            const ponder::Property& prop = cls.property(i);
            std::string vstr = userToString(ponder::UserObject(*m_lastOver), prop);
            props->Add(prop.name(), vstr);
        }
    }
    catch (ponder::ClassNotFound) {
        m_props->Add(std::string("Unknown class: ") + m_lastOver->GetTypeName());
    }
    
    m_props->ExpandAll();
}

GWEN_CONTROL_CONSTRUCTOR(UnitTest)
{
    m_pLastControl = NULL;
    Dock(Docking::Fill);
    SetSize(1024, 768);

//    Controls::CollapsibleList* scriptList = new Controls::CollapsibleList(this);
//    GetLeft()->GetTabControl()->AddPage("Script", scriptList);
    
    Controls::CollapsibleList* apiList = new Controls::CollapsibleList(this);
    GetLeft()->GetTabControl()->AddPage("API", apiList);

    GetLeft()->SetWidth(150);

    // Create control property viewer
    m_ControlProperties = new Controls::PropertyTree(GetRight());
    auto treeCampInfo = new Controls::PropertyTree(GetRight());
    pButton = GetRight()->GetTabControl()->AddPage("Properties", m_ControlProperties);
    GetRight()->GetTabControl()->AddPage("Classes", treeCampInfo);
    PopulateClassInfo(treeCampInfo);

    // Create output log and status bar.
    m_TextOutput = new Controls::ListBox(GetBottom());
    pButton = GetBottom()->GetTabControl()->AddPage("Output", m_TextOutput);
    GetBottom()->SetHeight(200);
    m_StatusBar = new Controls::StatusBar(this);
    m_StatusBar->Dock(Docking::Bottom);

    // Where to put the demo controls.
//    auto pCenter = new ControlContainer(this);
    auto pCenter = new Controls::Layout::Center(this);
    pCenter->Dock(Docking::Fill);
    auto listen = new ControlListener();
    listen->m_props = m_ControlProperties;
    listen->m_parent = pCenter;
    Hook::AddHook(listen);

#define ADD_UNIT_TEST(NAME) \
    GUnit* RegisterUnitTest_##NAME(Gwen::Controls::Base *tab); \
    { \
        Controls::Button* pButton = cat->Add(#NAME); \
        pButton->SetName(#NAME); \
        GUnit* test = RegisterUnitTest_##NAME(pCenter); \
        test->Hide(); \
        test->SetUnitTest(this); \
        pButton->onPress.Add(this, &ThisClass::OnCategorySelect, Gwen::Event::Packet(test)); \
    }

    // Create Controls using GWEN API.
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
    
    m_StatusBar->SendToBack();
    PrintText("Unit Test Started.");
    m_fLastSecond = Gwen::Platform::GetTimeInSeconds();
    m_iFrames = 0;
    
    apiList->GetNamedChildren("MenuStrip").DoAction();

    PrintText(Utility::Format("Size of Button = %lu bytes.", sizeof(Gwen::Controls::Button)));
}

void UnitTest::OnCategorySelect(Gwen::Event::Info info)
{
    if (m_pLastControl)
        m_pLastControl->Hide();

    info.Packet->Control->Show();
    m_pLastControl = info.Packet->Control;
    
    if (m_ControlProperties)
    {
        m_ControlProperties->Clear();
    }
}

void UnitTest::PrintText(const Gwen::String& str)
{
    m_TextOutput->AddItem(str);
    m_TextOutput->ScrollToBottom();
}

void UnitTest::Render(Gwen::Skin::Base* skin)
{
    m_iFrames++;

    if (m_fLastSecond < Gwen::Platform::GetTimeInSeconds())
    {
        m_StatusBar->SetText(Gwen::Utility::Format("GWEN Unit Test - %i fps", m_iFrames*2));
        m_fLastSecond = Gwen::Platform::GetTimeInSeconds()+0.5f;
        m_iFrames = 0;
    }

    ParentClass::Render(skin);
}

void GUnit::UnitPrint(Gwen::String str)
{
    m_pUnitTest->PrintText(str);
}

