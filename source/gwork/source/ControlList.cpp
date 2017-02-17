/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/ControlList.h>

namespace Gwk
{

void ControlList::Enable()
{
    for (auto&& item : list)
    {
        item->SetDisabled(false);
    }
}

void ControlList::Disable()
{
    for (auto&& item : list)
    {
        item->SetDisabled(true);
    }
}

void ControlList::Show()
{
    for (auto&& item : list)
    {
        item->Show();
    }
}

void ControlList::Hide()
{
    for (auto&& item : list)
    {
        item->Hide();
    }
}

Gwk::String ControlList::GetValue()
{
    for (auto&& item : list)
    {
        return item->GetValue();
    }

    return "";
}

void ControlList::SetValue(const Gwk::String& value)
{
    for (auto&& item : list)
    {
        item->SetValue(value);
    }
}

void ControlList::MoveBy(const Gwk::Point& point)
{
    for (auto&& item : list)
    {
        item->MoveBy(point.x, point.y);
    }
}

void ControlList::DoAction()
{
    for (auto&& item : list)
    {
        item->DoAction();
    }
}

void ControlList::SetActionInternal(Gwk::Event::Handler* object,
                                    void ( Gwk::Event::Handler::*f )(
                                        Gwk::Event::Info), const Gwk::Event::Packet& packet)
{
    for (auto&& item : list)
    {
        item->SetAction(object, f, packet);
    }
}

} // namespace Gwk
