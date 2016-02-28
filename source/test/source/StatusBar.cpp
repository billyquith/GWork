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
        Gwk::Controls::StatusBar* pStatus = new Gwk::Controls::StatusBar(this);
        Gwk::Controls::Label* pRight = new Gwk::Controls::Label(pStatus);
        pRight->SetText("Label Added to Right");
        pStatus->AddControl(pRight, true);
    }

    void Layout(Gwk::Skin::Base* skin)
    {
    }

};


DEFINE_UNIT_TEST(StatusBar);
