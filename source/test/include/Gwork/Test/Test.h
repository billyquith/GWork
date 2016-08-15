/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#pragma once
#ifndef GWK_TEST_TEST_H
#define GWK_TEST_TEST_H

#include <Gwork/Align.h>
#include <Gwork/Utility.h>
#include <Gwork/Controls/ListBox.h>
#include <Gwork/Controls/DockBase.h>
#include <Gwork/Controls/StatusBar.h>
#include <Gwork/Controls/TabControl.h>
#include <Gwork/Controls/Canvas.h>


// TODO - Put tests in Gwk namespace as can be included in project.

//
/// Frame into which all tests go.
//
class TestFrame : public Gwk::Controls::DockBase
{
public:
    GWK_CONTROL(TestFrame, Gwk::Controls::DockBase);
    
    //void PrintText(const Gwk::String& str);
    
    void Render(Gwk::Skin::Base* skin) override;
    
private:
    
    void OnCategorySelect(Gwk::Event::Info info);
    
    Gwk::Controls::TabControl*  m_testTabs;
    Gwk::Controls::StatusBar*   m_statusBar;
    unsigned int                m_frames;
    float                       m_fLastSecond;
};


//
/// Tab page into which test category goes.
//
class TestCategory : public Gwk::Controls::DockBase
{
public:
    GWK_CONTROL(TestCategory, Gwk::Controls::DockBase);
    
    void OutputToLog(const Gwk::String& str);
    
protected:

    Gwk::Controls::ListBox* m_textOutput;  // (optional) log
};


//
/// Individual test. All tests derive from this.
//
class TestUnit : public Gwk::Controls::Base
{
public:
    
    GWK_CONTROL_INLINE(TestUnit, Gwk::Controls::Base)
    ,   m_testCategory(nullptr)
    {}
    
    void SetTestCategory(TestCategory* t)
    {
        m_testCategory = t;
    }
    
    void OutputToLog(Gwk::String str);
    
    void Layout(Gwk::Skin::Base* skin) override
    {
        if (GetDock() != Gwk::Position::None)
            return;
        
        SizeToChildren(true, true);
    }
    
    TestCategory* m_testCategory;
};


#define DECLARE_TEST(NAME) \
    TestUnit* RegisterTest_##NAME(Gwk::Controls::Base *parent) { return new NAME(parent); }


#endif // ifndef GWK_TEST_TEST_H
