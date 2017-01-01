/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/StatusBar.h>
#include <Gwork/Controls/Label.h>

using namespace Gwk;

class StatusBar : public TestUnit
{
public:

    GWK_CONTROL_INLINE(StatusBar, TestUnit)
    {
        Dock(Position::Fill);
        Gwk::Controls::StatusBar* status = new Gwk::Controls::StatusBar(this);
        Gwk::Controls::Label* right = new Gwk::Controls::Label(status);
        right->SetText("Label Added to Right");
        status->AddControl(right, true);
    }

    void Layout(Gwk::Skin::Base* skin) override {}
};


DECLARE_TEST(StatusBar);
