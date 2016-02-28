/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2015-2016 Billy Quith
 *  See license in Gwork.h
 */


#include "Gwork/Gwork.h"
#include "Gwork/ControlList.h"

using namespace Gwk;
using namespace Gwk::Controls;

void ControlList::Enable()
{
    for (List::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        (*it)->SetDisabled(false);
    }
}

void ControlList::Disable()
{
    for (List::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        (*it)->SetDisabled(true);
    }
}

void ControlList::Show()
{
    for (List::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        (*it)->Show();
    }
}

void ControlList::Hide()
{
    for (List::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        (*it)->Hide();
    }
}

Gwk::String ControlList::GetValue()
{
    for (List::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        return (*it)->GetValue();
    }

    return "";
}

void ControlList::SetValue(const Gwk::String& value)
{
    for (List::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        (*it)->SetValue(value);
    }
}

void ControlList::MoveBy(const Gwk::Point& point)
{
    for (List::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        (*it)->MoveBy(point.x, point.y);
    }
}

void ControlList::DoAction()
{
    for (List::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        (*it)->DoAction();
    }
}

void ControlList::SetActionInternal(Gwk::Event::Handler* pObject,
                                    void ( Gwk::Event::Handler::*f )(
                                        Gwk::Event::Info), const Gwk::Event::Packet& packet)
{
    for (List::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        (*it)->SetAction(pObject, f, packet);
    }
}
