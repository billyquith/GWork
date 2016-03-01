/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/Test.h>
#include <Gwork/Controls/StatusBar.h>
#include <Gwork/Controls/Label.h>

using namespace Gwk;

class StatusBar : public GUnit
{
public:

    GWK_CONTROL_INLINE(StatusBar, GUnit)
    {
        Dock(Docking::Fill);
        Gwk::Controls::StatusBar* status = new Gwk::Controls::StatusBar(this);
        Gwk::Controls::Label* right = new Gwk::Controls::Label(status);
        right->SetText("Label Added to Right");
        status->AddControl(right, true);
    }

    void Layout(Gwk::Skin::Base* skin)
    {
    }

};


DEFINE_UNIT_TEST(StatusBar);
