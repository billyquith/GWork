/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2015-2016 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/ControlList.h>

namespace Gwk
{

void ControlList::Enable()
{
    for (List::const_iterator it = list.cbegin(); it != list.cend(); ++it)
    {
        (*it)->SetDisabled(false);
    }
}

void ControlList::Disable()
{
    for (List::const_iterator it = list.cbegin(); it != list.cend(); ++it)
    {
        (*it)->SetDisabled(true);
    }
}

void ControlList::Show()
{
    for (List::const_iterator it = list.cbegin(); it != list.cend(); ++it)
    {
        (*it)->Show();
    }
}

void ControlList::Hide()
{
    for (List::const_iterator it = list.cbegin(); it != list.cend(); ++it)
    {
        (*it)->Hide();
    }
}

Gwk::String ControlList::GetValue()
{
    for (List::const_iterator it = list.cbegin(); it != list.cend(); ++it)
    {
        return (*it)->GetValue();
    }

    return "";
}

void ControlList::SetValue(const Gwk::String& value)
{
    for (List::const_iterator it = list.cbegin(); it != list.cend(); ++it)
    {
        (*it)->SetValue(value);
    }
}

void ControlList::MoveBy(const Gwk::Point& point)
{
    for (List::const_iterator it = list.cbegin(); it != list.cend(); ++it)
    {
        (*it)->MoveBy(point.x, point.y);
    }
}

void ControlList::DoAction()
{
    for (List::const_iterator it = list.cbegin(); it != list.cend(); ++it)
    {
        (*it)->DoAction();
    }
}

void ControlList::SetActionInternal(Gwk::Event::Handler* object,
                                    void ( Gwk::Event::Handler::*f )(
                                        Gwk::Event::Info), const Gwk::Event::Packet& packet)
{
    for (List::const_iterator it = list.cbegin(); it != list.cend(); ++it)
    {
        (*it)->SetAction(object, f, packet);
    }
}

} // namespace Gwk
