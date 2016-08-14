/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Util/ControlFactory.h>
#include <ponder/classget.hpp>
#include <ponder/class.hpp>
#include <ponder/pondertype.hpp>

using namespace Gwk;

class ReflectButton : public TestUnit
{
public:

    GWK_CONTROL(ReflectButton, TestUnit);
    
    ~ReflectButton();

    void onButtonA(Event::Info info)
    {
        OutputToLog("Button Pressed (using 'OnPress' event)");
    }

    void OnToggle(Controls::Base* control)
    {
        OutputToLog("Button Toggled (using 'OnToggle' event)");
    }

    void OnToggleOn(Controls::Base* control)
    {
        OutputToLog("Button Toggled ON (using 'OnToggleOn' event)");
    }

    void OnToggleOff(Controls::Base* control)
    {
        OutputToLog("Button Toggled Off (using 'OnToggleOff' event)");
    }

    ponder::UserObject m_buttonA;
};

static void declare()
{
    ponder::Class::declare<ReflectButton>()
        .base<Controls::Base>();
}

PONDER_AUTO_TYPE(ReflectButton, &declare);

GWK_CONTROL_CONSTRUCTOR(ReflectButton)
{
    const ponder::Class& metaclass = ponder::classByType<Controls::Button>();

    // Normal button
    m_buttonA = metaclass.create(static_cast<Controls::Base*>(this));
    m_buttonA.set("text", "Hello world!");
    m_buttonA.get("onPress").to<Event::Listener*>()->Add(this, &ReflectButton::onButtonA);
    
    // Unicode test
    auto buttonB = metaclass.create(static_cast<Controls::Base*>(this));
    buttonB.set("text", Utility::Narrow(
                L"\u0417\u0430\u043C\u0435\u0436\u043D\u0430\u044F \u043C\u043E\u0432\u0430"));
    
    // Gwk::Align::PlaceBelow(buttonB, buttonA, 10);
    ponder::classByType<Align>().function("placeBelow").call(buttonB, ponder::Args(m_buttonA, 20));
    
    //    // Tooltip Button
    auto buttonC = metaclass.create(static_cast<Controls::Base*>(this));
    buttonC.set("text", "With tooltip");
    buttonC.call("setTooltip", ponder::Args("This is a tooltip!"));
    
    // Gwk::Align::PlaceBelow(buttonB, buttonA, 10);
    ponder::classByType<Align>().function("placeBelow").call(buttonC, ponder::Args(buttonB, 30));
}

ReflectButton::~ReflectButton()
{
    const ponder::Class& metaclass = ponder::classByType<Controls::Button>();
    metaclass.destroy(m_buttonA);
}

DECLARE_TEST(ReflectButton);
