/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Controls/ComboBox.h>
#include <Gwork/Controls/Menu.h>
#include <Gwork/Controls/Canvas.h>

using namespace Gwk;
using namespace Gwk::Controls;
using namespace Gwk::ControlsInternal;

class GWK_EXPORT DownArrow : public Controls::Base
{
public:

    GWK_CONTROL_INLINE(DownArrow, Controls::Base)
    {
        SetMouseInputEnabled(false);
        SetSize(15, 15);
    }

    void Render(Skin::Base* skin) override
    {
        if (!m_comboBox->ShouldDrawBackground())
            return skin->DrawComboDownArrow(this, false, false, false, m_comboBox->IsDisabled());

        skin->DrawComboDownArrow(this, m_comboBox->IsHovered(),
                                 m_comboBox->IsDepressed(),
                                 m_comboBox->IsMenuOpen(), m_comboBox->IsDisabled());
    }

    void SetComboBox(ComboBox* p)
    {
        m_comboBox = p;
    }

protected:

    ComboBox*   m_comboBox;
};


GWK_CONTROL_CONSTRUCTOR(ComboBox)
{
    SetSize(100, 20);
    
    m_menu = new Menu(this);
    m_menu->SetHidden(true);
    m_menu->SetDisableIconMargin(true);
    m_menu->SetTabable(false);
    
    DownArrow* arrow = new DownArrow(this);
    arrow->SetComboBox(this);
    
    m_button = arrow;
    m_selectedItem = nullptr;
    
    SetAlignment(Gwk::Position::Left|Gwk::Position::CenterV);
    SetText("");
    SetMargin(Margin(3, 0, 0, 0));
    SetTabable(true);
    SetKeyboardInputEnabled(true);
}

MenuItem* ComboBox::AddItem(const String& strLabel, const String& strName)
{
    MenuItem* item = m_menu->AddItem(strLabel, "");
    item->SetName(strName);
    item->onMenuItemSelected.Add(this, &ComboBox::OnItemSelected);

    // Default
    if (m_selectedItem == nullptr)
        OnItemSelected(item);

    return item;
}

void ComboBox::Render(Skin::Base* skin)
{
    if (!ShouldDrawBackground())
        return;

    skin->DrawComboBox(this, IsDepressed(), IsMenuOpen());
}

void ComboBox::Layout(Skin::Base* skin)
{
    m_button->SetPosition(Position::Right | Position::CenterV, 4, 0);
    ParentClass::Layout(skin);
}

void ComboBox::UpdateColours()
{
    if (!ShouldDrawBackground())
        return SetTextColor(GetSkin()->Colors.Button.Normal);

    ParentClass::UpdateColours();
}

void ComboBox::OnPress()
{
    if (IsMenuOpen())
        return GetCanvas()->CloseMenus();

    bool bWasMenuHidden = m_menu->Hidden();
    GetCanvas()->CloseMenus();

    if (bWasMenuHidden)
        OpenList();
}

void ComboBox::ClearItems()
{
    if (m_menu)
        m_menu->ClearItems();
}

void ComboBox::SelectItem(MenuItem* item, bool bFireChangeEvents)
{
    if (m_selectedItem == item)
        return;

    m_selectedItem = item;
    SetText(m_selectedItem->GetText());
    m_menu->SetHidden(true);
    Invalidate();

    if (bFireChangeEvents)
    {
        onSelection.Call(this);
        Focus();
    }
}

void ComboBox::OnItemSelected(Event::Info info)
{
    // Convert selected to a menu item
    MenuItem* item = gwk_cast<MenuItem>(info.ControlCaller);

    if (item)
    {
        SelectItem(item);        
    }
}

void ComboBox::SelectItemByName(const Gwk::String& name, bool bFireChangeEvents)
{
    Base::List& children = m_menu->GetChildren();
    Base::List::iterator it = children.begin();

    while (it != children.end())
    {
        MenuItem* child = gwk_cast<MenuItem>(*it);

        if (child->GetName() == name)
            return SelectItem(child, bFireChangeEvents);

        ++it;
    }
}

void ComboBox::OnLostKeyboardFocus()
{
    SetTextColor(Color(0, 0, 0, 255));
}

void ComboBox::OnKeyboardFocus()
{
    // Until we add the blue highlighting again
    SetTextColor(Color(0, 0, 0, 255));
    // m_selectedText->SetTextColor( Color( 255, 255, 255, 255 ) );
}

Gwk::Controls::Label* ComboBox::GetSelectedItem()
{
    return m_selectedItem;
}

bool ComboBox::IsMenuOpen()
{
    return m_menu && !m_menu->Hidden();
}

void ComboBox::OpenList()
{
    if (!m_menu)
        return;

    m_menu->SetParent(GetCanvas());
    m_menu->SetHidden(false);
    m_menu->BringToFront();
    Gwk::Point p = LocalPosToCanvas(Gwk::Point(0, 0));
    m_menu->SetBounds(Gwk::Rect(p.x, p.y+Height(), Width(), m_menu->Height()));
}

void ComboBox::CloseList()
{
    if (!m_menu)
        return;

    m_menu->Hide();
}

bool ComboBox::OnKeyUp(bool bDown)
{
    if (bDown)
    {
        Base::List& children = m_menu->GetChildren();
        Base::List::reverse_iterator it = std::find(children.rbegin(),
                                                    children.rend(), m_selectedItem);

        if (it != children.rend() && (++it != children.rend()))
        {
            Base* upElement = *it;
            OnItemSelected(upElement);
        }
    }

    return true;
}

bool ComboBox::OnKeyDown(bool bDown)
{
    if (bDown)
    {
        Base::List& children = m_menu->GetChildren();
        Base::List::iterator it = std::find(children.begin(), children.end(), m_selectedItem);

        if (it != children.end() && (++it != children.end()))
        {
            Base* downElement = *it;
            OnItemSelected(downElement);
        }
    }

    return true;
}

void ComboBox::RenderFocus(Gwk::Skin::Base* /*skin*/)
{
}
