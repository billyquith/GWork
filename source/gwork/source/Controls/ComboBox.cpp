/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Controls/ComboBox.h>
#include <Gwork/Controls/Menu.h>


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

    void Render(Skin::Base* skin)
    {
        if (!m_ComboBox->ShouldDrawBackground())
            return skin->DrawComboDownArrow(this, false, false, false, m_ComboBox->IsDisabled());

        skin->DrawComboDownArrow(this, m_ComboBox->IsHovered(),
                                 m_ComboBox->IsDepressed(),
                                 m_ComboBox->IsMenuOpen(), m_ComboBox->IsDisabled());
    }

    void SetComboBox(ComboBox* p)
    {
        m_ComboBox = p;
    }

protected:

    ComboBox*   m_ComboBox;
};


GWK_CONTROL_CONSTRUCTOR(ComboBox)
{
    SetSize(100, 20);
    
    m_Menu = new Menu(this);
    m_Menu->SetHidden(true);
    m_Menu->SetDisableIconMargin(true);
    m_Menu->SetTabable(false);
    
    DownArrow* pArrow = new DownArrow(this);
    pArrow->SetComboBox(this);
    
    m_Button = pArrow;
    m_SelectedItem = NULL;
    
    SetAlignment(Gwk::Docking::Left|Gwk::Docking::CenterV);
    SetText("");
    SetMargin(Margin(3, 0, 0, 0));
    SetTabable(true);
    SetKeyboardInputEnabled(true);
}

MenuItem* ComboBox::AddItem(const String& strLabel, const String& strName)
{
    MenuItem* pItem = m_Menu->AddItem(strLabel, "");
    pItem->SetName(strName);
    pItem->onMenuItemSelected.Add(this, &ComboBox::OnItemSelected);

    // Default
    if (m_SelectedItem == NULL)
        OnItemSelected(pItem);

    return pItem;
}

void ComboBox::Render(Skin::Base* skin)
{
    if (!ShouldDrawBackground())
        return;

    skin->DrawComboBox(this, IsDepressed(), IsMenuOpen());
}

void ComboBox::Layout(Skin::Base* skin)
{
    m_Button->Position(Docking::Right|Docking::CenterV, 4, 0);
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

    bool bWasMenuHidden = m_Menu->Hidden();
    GetCanvas()->CloseMenus();

    if (bWasMenuHidden)
        OpenList();
}

void ComboBox::ClearItems()
{
    if (m_Menu)
        m_Menu->ClearItems();
}

void ComboBox::SelectItem(MenuItem* pItem, bool bFireChangeEvents)
{
    if (m_SelectedItem == pItem)
        return;

    m_SelectedItem = pItem;
    SetText(m_SelectedItem->GetText());
    m_Menu->SetHidden(true);
    Invalidate();

    if (bFireChangeEvents)
    {
        onSelection.Call(this);
        Focus();
    }
}

void ComboBox::OnItemSelected(Controls::Base* pControl)
{
    // Convert selected to a menu item
    MenuItem* pItem = gwk_cast<MenuItem>(pControl);

    if (!pItem)
        return;

    SelectItem(pItem);
}

void ComboBox::SelectItemByName(const Gwk::String& name, bool bFireChangeEvents)
{
    Base::List& children = m_Menu->GetChildren();
    Base::List::iterator it = children.begin();

    while (it != children.end())
    {
        MenuItem* pChild = gwk_cast<MenuItem>(*it);

        if (pChild->GetName() == name)
            return SelectItem(pChild, bFireChangeEvents);

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
    // m_SelectedText->SetTextColor( Color( 255, 255, 255, 255 ) );
}

Gwk::Controls::Label* ComboBox::GetSelectedItem()
{
    return m_SelectedItem;
}

bool ComboBox::IsMenuOpen()
{
    return m_Menu && !m_Menu->Hidden();
}

void ComboBox::OpenList()
{
    if (!m_Menu)
        return;

    m_Menu->SetParent(GetCanvas());
    m_Menu->SetHidden(false);
    m_Menu->BringToFront();
    Gwk::Point p = LocalPosToCanvas(Gwk::Point(0, 0));
    m_Menu->SetBounds(Gwk::Rect(p.x, p.y+Height(), Width(), m_Menu->Height()));
}

void ComboBox::CloseList()
{
    if (!m_Menu)
        return;

    m_Menu->Hide();
}

bool ComboBox::OnKeyUp(bool bDown)
{
    if (bDown)
    {
        Base::List& children = m_Menu->GetChildren();
        Base::List::reverse_iterator it = std::find(children.rbegin(),
                                                    children.rend(), m_SelectedItem);

        if (it != children.rend() && (++it != children.rend()))
        {
            Base* pUpElement = *it;
            OnItemSelected(pUpElement);
        }
    }

    return true;
}

bool ComboBox::OnKeyDown(bool bDown)
{
    if (bDown)
    {
        Base::List& children = m_Menu->GetChildren();
        Base::List::iterator it = std::find(children.begin(), children.end(), m_SelectedItem);

        if (it != children.end() && (++it != children.end()))
        {
            Base* pDownElement = *it;
            OnItemSelected(pDownElement);
        }
    }

    return true;
}

void ComboBox::RenderFocus(Gwk::Skin::Base* /*skin*/)
{
}
