/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#pragma once
#ifndef GWK_UNITTEST_UNITTEST_H
#define GWK_UNITTEST_UNITTEST_H

#include <Gwork/Gwork.h>
#include <Gwork/Align.h>
#include <Gwork/Utility.h>
#include <Gwork/Controls/WindowControl.h>
#include <Gwork/Controls/TabControl.h>
#include <Gwork/Controls/ListBox.h>
#include <Gwork/Controls/DockBase.h>
#include <Gwork/Controls/StatusBar.h>
#include <Gwork/Controls/PropertyTree.h>

class UnitTest;


class GUnit : public Gwk::Controls::Base
{
public:

    GWK_CONTROL_INLINE(GUnit, Gwk::Controls::Base)
    {
        m_pUnitTest = NULL;
    }

    void SetUnitTest(UnitTest* u)
    {
        m_pUnitTest = u;
    }

    void UnitPrint(Gwk::String str);

    void Layout(Gwk::Skin::Base* skin)
    {
        if (GetDock() != Gwk::Docking::None)
            return;

        SizeToChildren(true, true);
    }

    UnitTest* m_pUnitTest;
};


class UnitTest : public Gwk::Controls::DockBase
{
public:

    GWK_CONTROL(UnitTest, Gwk::Controls::DockBase);

    void PrintText(const Gwk::String& str);

    void Render(Gwk::Skin::Base* skin);

private:

    void OnCategorySelect(Gwk::Event::Info info);

    Gwk::Controls::TabControl* m_TabControl;
    Gwk::Controls::ListBox*    m_TextOutput;
    Gwk::Controls::PropertyTree* m_ControlProperties;
    Gwk::Controls::StatusBar*  m_StatusBar;
    unsigned int m_iFrames;
    float m_fLastSecond;

    Gwk::Controls::Base*       m_pLastControl;

};


#define DEFINE_UNIT_TEST(NAME) \
    GUnit* RegisterUnitTest_##NAME(Gwk::Controls::Base *tab) \
    { return new NAME(tab); }


#endif // ifndef GWK_UNITTEST_UNITTEST_H
