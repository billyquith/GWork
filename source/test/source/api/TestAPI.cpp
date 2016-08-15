/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/DockedTabControl.h>
#include <Gwork/Controls/WindowControl.h>
#include <Gwork/Controls/CollapsibleList.h>
#include <Gwork/Controls/Layout/Position.h>
#include <Gwork/Platform.h>
#include <Gwork/PlatformCommon.h>

using namespace Gwk;


GWK_CONTROL_CONSTRUCTOR(TestAPI)
{
    GWK_IF_ALLOC_STATS( Platform::AllocStatsAddMark("API test"); )
    
    m_lastControl = nullptr;

    Dock(Position::Fill);
    
    Controls::CollapsibleList* apiList = new Controls::CollapsibleList( this );
    GetLeft()->GetTabControl()->AddPage( "Tests", apiList );
    GetLeft()->SetWidth( 150 );

    // Where to put the demo controls.
    auto center = new Controls::Layout::Center(this);
    center->Dock(Position::Fill);
    
    m_textOutput = new Controls::ListBox(GetBottom());
    GetBottom()->GetTabControl()->AddPage("Output", m_textOutput);
    GetBottom()->SetHeight(140);


#define ADD_TEST(NAME) \
    TestUnit* RegisterTest_##NAME(Gwk::Controls::Base *tab); \
    { \
        Controls::Button *button = cat->Add(#NAME); \
        button->SetName(#NAME); \
        GWK_IF_ALLOC_STATS( Platform::AllocStatsAddMark(#NAME); ) \
        TestUnit *test = RegisterTest_##NAME(center); \
        GWK_IF_ALLOC_STATS( Platform::AllocStatsAddMark(#NAME); ) \
        test->SetTestCategory(this); \
        test->Hide(); \
        button->onPress.Add( this, &TestAPI::OnCategorySelect, test );\
    }

    // Create Controls using Gwork API.
    {
        Controls::CollapsibleCategory* cat = apiList->Add("Basic");
        ADD_TEST(Button);
        ADD_TEST(Label);
        ADD_TEST(LabelMultiline);
    }
    {
        Controls::CollapsibleCategory* cat = apiList->Add("Non-Interactive");
        ADD_TEST(ProgressBar);
        ADD_TEST(GroupBox);
        ADD_TEST(ImagePanel);
        ADD_TEST(StatusBar);
    }
    {
        Controls::CollapsibleCategory* cat = apiList->Add("Controls");
        ADD_TEST(ComboBox);
        ADD_TEST(TextBox);
        ADD_TEST(ListBox);
        ADD_TEST(CrossSplitter);
        ADD_TEST(RadioButton);
        ADD_TEST(Checkbox);
        ADD_TEST(Numeric);
        ADD_TEST(Slider);
        ADD_TEST(MenuStrip);
    }
    {
        Controls::CollapsibleCategory* cat = apiList->Add("Containers");
        ADD_TEST(Window);
        ADD_TEST(TreeControl);
        ADD_TEST(Properties);
        ADD_TEST(TabControl);
        ADD_TEST(ScrollControl);
        ADD_TEST(PageControl);
    }
    {
        Controls::CollapsibleCategory* cat = apiList->Add("Non-Standard");
        ADD_TEST(CollapsibleList);
        ADD_TEST(ColorPicker);
    }
#undef ADD_TEST
    
    GWK_IF_ALLOC_STATS( Platform::AllocStatsAddMark("API test"); )
    
    GWK_IF_ALLOC_STATS(
        FILE *fh = fopen(GWK_STATS_DIR "curr_allocs.csv", "w");
        Platform::AllocStatsDump(fh);
        fclose(fh); )
}

void TestAPI::OnCategorySelect(Gwk::Event::Info info)
{
    if (m_lastControl)
        m_lastControl->Hide();  // hide last

    info.Packet->Control->Show();
    m_lastControl = info.Packet->Control;
}
