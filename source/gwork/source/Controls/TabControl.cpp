/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/TabControl.h>
#include <Gwork/Controls/Highlight.h>
#include <Gwork/DragAndDrop.h>
#include <Gwork/Controls/WindowControl.h>
#include <Gwork/Controls/ScrollBarButton.h>


using namespace Gwk;
using namespace Gwk::Controls;

template <typename T, typename T2>
inline T Approach(T fCurrent, T fTarget, T2 fDelta)
{
    if (fCurrent < fTarget)
    {
        fCurrent += fDelta;
        
        if (fCurrent > fTarget)
            return fTarget;
    }
    else if (fCurrent > fTarget)
    {
        fCurrent -= fDelta;
        
        if (fCurrent < fTarget)
            return fTarget;
    }
    
    return fCurrent;
}

class TabControlInner : public Base
{
public:

    GWK_CONTROL_INLINE(TabControlInner, Base)
    {
    }

    void Render(Skin::Base* skin) override
    {
        skin->DrawTabControl(this);
    }

};


GWK_CONTROL_CONSTRUCTOR(TabControl)
{
    m_scrollOffset = 0;
    m_currentButton = nullptr;
    m_tabStrip = new TabStrip(this);
    m_tabStrip->SetTabPosition(Position::Top);
    // Make this some special control?
    m_scroll[0] = new ControlsInternal::ScrollBarButton(this);
    m_scroll[0]->SetDirectionLeft();
    m_scroll[0]->onPress.Add(this, &TabControl::OnScrollPressLeft);
    m_scroll[0]->SetSize(14, 14);
    m_scroll[1] = new ControlsInternal::ScrollBarButton(this);
    m_scroll[1]->SetDirectionRight();
    m_scroll[1]->onPress.Add(this, &TabControl::OnScrollPressRight);
    m_scroll[1]->SetSize(14, 14);
    m_innerPanel = new TabControlInner(this);
    m_innerPanel->Dock(Position::Fill);
    m_innerPanel->SendToBack();
    SetTabable(false);
}

TabButton* TabControl::AddPage(String strText, Controls::Base* page)
{
    if (!page)
        page = new Base(this);
    else
        page->SetParent(this);

    TabButton* button = new TabButton(m_tabStrip);
    button->SetText(strText);
    button->SetPage(page);
    button->SetTabable(false);
    AddPage(button);
    return button;
}

void TabControl::RemovePage(TabButton* button)
{
    button->SetParent(GetCanvas());
    OnLoseTab(button);
}

void TabControl::AddPage(TabButton* button)
{
    Base* page = button->GetPage();
    page->SetParent(this);
    page->SetHidden(true);
    page->SetMargin(Margin(6, 6, 6, 6));
    page->Dock(Position::Fill);
    button->SetParent(m_tabStrip);
    button->Dock(Position::Left);
    button->SizeToContents();

    if (button->GetTabControl())
        button->onPress.RemoveHandler(button->GetTabControl());

    button->SetTabControl(this);
    button->onPress.Add(this, &TabControl::OnTabPressed);

    if (!m_currentButton)
        button->OnPress(Event::Information(this));

    onAddTab.Call(this);
    Invalidate();
}

void TabControl::OnTabPressed(Event::Info info)
{
    TabButton* button = gwk_cast<TabButton>(info.ControlCaller);

    if (!button)
        return;

    Base* page = button->GetPage();

    if (!page)
        return;

    if (m_currentButton == button)
        return;

    if (m_currentButton)
    {
        Base* page = m_currentButton->GetPage();

        if (page)
            page->SetHidden(true);

        m_currentButton->Redraw();
        m_currentButton = nullptr;
    }

    m_currentButton = button;
    page->SetHidden(false);
    m_tabStrip->Invalidate();
    Invalidate();
}

void TabControl::PostLayout(Skin::Base* skin)
{
    ParentClass::PostLayout(skin);
    HandleOverflow();
}

void TabControl::OnLoseTab(TabButton* button)
{
    if (m_currentButton == button)
        m_currentButton = nullptr;

    // TODO: Select a tab if any exist.
    onLoseTab.Call(this);
    Invalidate();
}

int TabControl::TabCount(void)
{
    return m_tabStrip->NumChildren();
}

TabButton* TabControl::GetTab(int iNum)
{
    return gwk_cast<TabButton>(m_tabStrip->GetChild(iNum));
}

void TabControl::SetTabStripPosition(Position dock)
{
    m_tabStrip->SetTabPosition(dock);
}

bool TabControl::DoesAllowDrag()
{
    return m_tabStrip->AllowsTabReorder();
}

void TabControl::HandleOverflow()
{
    Gwk::Point TabsSize = m_tabStrip->ChildrenSize();
    // Only enable the scrollers if the tabs are at the top.
    // This is a limitation we should explore.
    // Really TabControl should have derivitives for tabs placed elsewhere where
    // we could specialize
    // some functions like this for each direction.
    bool bNeeded = TabsSize.x > Width() && m_tabStrip->GetDock() == Position::Top;
    m_scroll[0]->SetHidden(!bNeeded);
    m_scroll[1]->SetHidden(!bNeeded);

    if (!bNeeded)
        return;

    m_scrollOffset = Gwk::Clamp(m_scrollOffset, 0, TabsSize.x-Width()+32);
#if 0
    //
    // This isn't frame rate independent.
    // Could be better. Get rid of m_scrollOffset and just use
    // m_tabStrip->GetMargin().left ?
    // Then get a margin animation type and do it properly!
    // TODO!
    //
    m_tabStrip->SetMargin(Margin(Approach(m_tabStrip->GetMargin().left, m_scrollOffset* -1,
                                                2), 0, 0, 0));
    InvalidateParent();
#else // if 0
    m_tabStrip->SetMargin(Margin(m_scrollOffset* -1, 0, 0, 0));
#endif // if 0
    m_scroll[0]->SetPos(Width()-30, 5);
    m_scroll[1]->SetPos(m_scroll[0]->Right(), 5);
}

void TabControl::OnScrollPressLeft(Event::Info)
{
    m_scrollOffset -= 120;
}

void TabControl::OnScrollPressRight(Event::Info)
{
    m_scrollOffset += 120;
}
