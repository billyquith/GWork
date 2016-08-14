/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Controls/ListBox.h>
#include <Gwork/Controls/ScrollControl.h>
#include <Gwork/InputHandler.h>

using namespace Gwk;
using namespace Gwk::Controls;

class ListBoxRow : public Layout::TableRow
{
    GWK_CONTROL_INLINE(ListBoxRow, Layout::TableRow)
    {
        SetMouseInputEnabled(true);
        SetSelected(false);
    }

    void Render(Skin::Base* skin) override
    {
        skin->DrawListBoxLine(this, IsSelected(), GetEven());
    }

    bool IsSelected() const
    {
        return m_bSelected;
    }

    void DoSelect()
    {
        SetSelected(true);
        onRowSelected.Call(this);
        Redraw();
    }

    void OnMouseClickLeft(int /*x*/, int /*y*/, bool bDown) override
    {
        if (bDown)
            DoSelect();
    }

    void SetSelected(bool b) override
    {
        m_bSelected = b;

        // TODO: Get these values from the skin.
        if (b)
            SetTextColor(Gwk::Colors::White);
        else
            SetTextColor(Gwk::Colors::Black);
    }

private:

    bool m_bSelected;

};


GWK_CONTROL_CONSTRUCTOR(ListBox)
{
    SetScroll(false, true);
    SetAutoHideBars(true);
    SetMargin(Margin(1, 1, 1, 1));
    m_innerPanel->SetPadding(Padding(2, 2, 2, 2));
    m_table = new Controls::Layout::Table(this);
    m_table->SetColumnCount(1);
    m_bMultiSelect = false;
}

Layout::TableRow* ListBox::AddItem(const String& strLabel, const String& strName)
{
    ListBoxRow* row = new ListBoxRow(this);
    m_table->AddRow(row);
    row->SetCellText(0, strLabel);
    row->SetName(strName);
    row->onRowSelected.Add(this, &ListBox::OnRowSelected);
    return row;
}

void ListBox::RemoveItem(Layout::TableRow* row)
{
    m_selectedRows.erase(std::find(m_selectedRows.begin(), m_selectedRows.end(), row));
    m_table->Remove(row);
}

void ListBox::Render(Skin::Base* skin)
{
    skin->DrawListBox(this);
}

void ListBox::Layout(Skin::Base* skin)
{
    ParentClass::Layout(skin);
    const Gwk::Rect& inner = m_innerPanel->GetInnerBounds();
    m_table->SetPos(inner.x, inner.y);
    m_table->SetWidth(inner.w);
    m_table->SizeToChildren(false, true);
    ParentClass::Layout(skin);
}

void ListBox::UnselectAll()
{
    std::list<Layout::TableRow*>::iterator it = m_selectedRows.begin();

    while (it != m_selectedRows.end())
    {
        ListBoxRow* row = static_cast<ListBoxRow*>(*it);
        it = m_selectedRows.erase(it);
        row->SetSelected(false);
    }
}

void ListBox::OnRowSelected(Event::Info info)
{
    bool bClear = !Gwk::Input::IsShiftDown();

    if (!AllowMultiSelect())
        bClear = true;

    SetSelectedRow(info.ControlCaller, bClear);
}

Layout::TableRow* ListBox::GetSelectedRow()
{
    if (m_selectedRows.empty())
        return nullptr;

    return *m_selectedRows.begin();
}

Gwk::String ListBox::GetSelectedRowName()
{
    Layout::TableRow* row = GetSelectedRow();

    if (!row)
        return "";

    return row->GetName();
}

void ListBox::Clear()
{
    UnselectAll();
    m_table->Clear();
}

void ListBox::SetSelectedRow(Gwk::Controls::Base* control, bool bClearOthers)
{
    if (bClearOthers)
        UnselectAll();

    ListBoxRow* row = gwk_cast<ListBoxRow>(control);

    if (!row)
        return;

    // TODO: make sure this is one of our rows!
    row->SetSelected(true);
    m_selectedRows.push_back(row);
    onRowSelected.Call(this);
}

void ListBox::SelectByString(const String& strName, bool bClearOthers)
{
    if (bClearOthers)
        UnselectAll();

    Base::List& children = m_table->GetChildren();

    for (auto&& control : children)
    {
        ListBoxRow* child = gwk_cast<ListBoxRow>(control);

        if (!child)
            continue;

        if (Utility::Strings::Wildcard(strName, child->GetText(0)))
            SetSelectedRow(child, false);
    }
}

bool ListBox::OnKeyDown(bool bDown)
{
    if (bDown)
    {
        Base::List& children = m_table->Children;
        Base::List::const_iterator begin = children.cbegin();
        Base::List::const_iterator end = children.cend();
        Controls::Base* sel_row = GetSelectedRow();

        if (sel_row == nullptr && !children.empty())   // no user selection yet, so
        {                                           // select first element
            sel_row = children.front();
        }

        Base::List::const_iterator result = std::find(begin, end, sel_row);

        if (result != end)
        {
            Base::List::const_iterator next = result;
            ++next;

            if (next != end)
                result = next;

            ListBoxRow* row = gwk_cast<ListBoxRow>(*result);

            if (row)
            {
                row->DoSelect();
                Controls::VerticalScrollBar* scroll = gwk_cast<Controls::VerticalScrollBar>(
                    m_verticalScrollBar);

                if (scroll)
                    scroll->NudgeDown(this);

                Redraw();
            }
        }
    }

    return true;
}

bool ListBox::OnKeyUp(bool bDown)
{
    if (bDown)
    {
        Base::List& children = m_table->Children;
        Base::List::const_iterator begin = children.cbegin();
        Base::List::const_iterator end = children.cend();
        Controls::Base* sel_row = GetSelectedRow();

        // no user selection yet, so select first element
        if (sel_row == nullptr && !children.empty())
            sel_row = children.front();

        Base::List::const_iterator result = std::find(begin, end, sel_row);

        if (result != end)
        {
            if (result != begin)
                --result;

            ListBoxRow* row = gwk_cast<ListBoxRow>(*result);

            if (row)
            {
                row->DoSelect();
                Controls::VerticalScrollBar* scroll = gwk_cast<Controls::VerticalScrollBar>(
                    m_verticalScrollBar);

                if (scroll)
                    scroll->NudgeUp(this);

                Redraw();
            }
        }
    }

    return true;
}
