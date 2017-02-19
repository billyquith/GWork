/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Util/ControlReflect.h>
#include <ponder/classget.hpp>
#include <ponder/class.hpp>
#include <ponder/pondertype.hpp>
#include <ponder/uses/runtime.hpp>

#include <lua.h>
#include <lauxlib.h>
#include <ponder/uses/lua.hpp>

using namespace Gwk;

static lua_State *g_L = NULL;

class ReflectButton : public TestUnit
{
public:

    GWK_CONTROL_INLINE(ReflectButton, TestUnit) {}
    
    virtual ~ReflectButton();

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

class ReflectAPIButton : public ReflectButton
{
public:
    GWK_CONTROL(ReflectAPIButton, ReflectButton);
    virtual ~ReflectAPIButton() {}
};

class ReflectLuaButton : public ReflectButton
{
public:
    GWK_CONTROL(ReflectLuaButton, ReflectButton);
    virtual ~ReflectLuaButton() {}
};

static void declare()
{
    ponder::Class::declare<ReflectButton>()
        .base<Controls::Base>();

    g_L = luaL_newstate();
    ponder::lua::expose<Gwk::Controls::Button>(g_L, "Button");
    ponder::lua::expose<ReflectButton>(g_L, "ReflectButton");
}

PONDER_AUTO_TYPE(ReflectButton, &declare);

ReflectButton::~ReflectButton()
{
    ponder::runtime::destroy(m_buttonA);
}

GWK_CONTROL_CONSTRUCTOR(ReflectAPIButton)
{
    using namespace ponder;
    
    const ponder::Class& metaclass = ponder::classByType<Controls::Button>();
    runtime::ObjectFactory fact(metaclass);

    // Normal button
    m_buttonA = fact.create(static_cast<Controls::Base*>(this));
    m_buttonA.set("text", "Hello world!");
    m_buttonA.get("onPress").to<Event::Listener*>()->Add(this, &ReflectButton::onButtonA);
    
    // Unicode test
    auto buttonB = fact.create(static_cast<Controls::Base*>(this));
    buttonB.set("text", Utility::Narrow(
                L"\u0417\u0430\u043C\u0435\u0436\u043D\u0430\u044F \u043C\u043E\u0432\u0430"));
    
    // Gwk::Align::PlaceBelow(buttonB, buttonA, 10);
    runtime::callStatic(classByType<Align>().function("placeBelow"), buttonB, m_buttonA, 20);
    
    // Tooltip Button
    auto buttonC = fact.create(static_cast<Controls::Base*>(this));
    buttonC.set("text", "With tooltip");
    runtime::call(metaclass.function("setTooltip"), buttonC, "This is a tooltip!");
    
    // Gwk::Align::PlaceBelow(buttonB, buttonA, 10);
    runtime::callStatic(classByType<Align>().function("placeBelow"), buttonC, buttonB, 30);
}

GWK_CONTROL_CONSTRUCTOR(ReflectLuaButton)
{
    // Ensure declare() is called.
    (void) ponder::classByType<ReflectButton>();
    
    // Tell Lua about the parent control, into which we'll create our controls.
    ponder::lua::pushUserObject(g_L,
        ponder::UserObject::makeRef(*static_cast<Gwk::Controls::Base*>(this)));
    lua_setglobal(g_L, "parent");

    const char* str = R"lua(
        buttA = Button(parent)
        buttA.text = 'Hello from Lua!'
    )lua";
    ponder::lua::runString(g_L, str);
}

DECLARE_TEST(ReflectAPIButton);
DECLARE_TEST(ReflectLuaButton);

