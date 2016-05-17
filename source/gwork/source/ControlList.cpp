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
    for(auto&& iter : list)
    {
        iter->SetDisabled(false);
    }
}

void ControlList::Disable()
{
    for(auto&& iter : list)
    {
        iter->SetDisabled(true);
    }
}

void ControlList::Show()
{
    for(auto&& iter : list)
    {
        iter->Show();
    }
}

void ControlList::Hide()
{
    for(auto&& iter : list)
    {
        iter->Hide();
    }
}

Gwk::String ControlList::GetValue()
{
    for(auto&& iter : list)
    {
        return iter->GetValue();
    }

    return "";
}

void ControlList::SetValue(const Gwk::String& value)
{
    for(auto&& iter : list)
    {
        iter->SetValue(value);
    }
}

void ControlList::MoveBy(const Gwk::Point& point)
{
    for(auto&& iter : list)
    {
        iter->MoveBy(point.x, point.y);
    }
}

void ControlList::DoAction()
{
    for(auto&& iter : list)
    {
        iter->DoAction();
    }
}

void ControlList::SetActionInternal(Gwk::Event::Handler* object,
                                    void ( Gwk::Event::Handler::*f )(
                                        Gwk::Event::Info), const Gwk::Event::Packet& packet)
{
    for(auto&& iter : list)
    {
        iter->SetAction(object, f, packet);
    }
}

} // namespace Gwk
