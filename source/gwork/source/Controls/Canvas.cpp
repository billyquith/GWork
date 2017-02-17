/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Gwork.h>
#include <Gwork/Controls/Canvas.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/Menu.h>
#include <Gwork/DragAndDrop.h>
#include <Gwork/Tooltip.h>

#if GWK_ANIMATE
#include <Gwork/Anim.h>
#endif

#include <cctype>

using namespace Gwk::Controls;


Canvas::Canvas(Gwk::Skin::Base* skin) : ParentClass(nullptr), m_bAnyDelete(false)
{
    SetBounds(0, 0, 10000, 10000);
    SetScale(1.0f);
    SetBackgroundColor(Color(255, 255, 255, 255));
    SetDrawBackground(false);

    if (skin)
        SetSkin(skin);
}

Canvas::~Canvas()
{
    ReleaseChildren();
}

void Canvas::RenderCanvas()
{
    DoThink();
    Gwk::Renderer::Base* render = m_skin->GetRender();
    render->Begin();
    RecurseLayout(m_skin);
    render->SetClipRegion(GetBounds());
    render->SetRenderOffset(Gwk::Point(0, 0));
    render->SetScale(Scale());

    if (m_bDrawBackground)
    {
        render->SetDrawColor(m_backgroundColor);
        render->DrawFilledRect(GetRenderBounds());
    }

    DoRender(m_skin);
    DragAndDrop::RenderOverlay(this, m_skin);
    Tooltip::RenderTooltip(m_skin);
    render->End();
}

void Canvas::Render(Gwk::Skin::Base* /*render*/)
{
    m_bNeedsRedraw = false;
}

void Canvas::OnBoundsChanged(Gwk::Rect oldBounds)
{
    ParentClass::OnBoundsChanged(oldBounds);
    InvalidateChildren(true);
}

void Canvas::DoThink()
{
    ProcessDelayedDeletes();

    if (Hidden())
        return;

#if GWK_ANIMATE
    Gwk::Anim::Think();
#endif
    // Reset tabbing
    {
        NextTab = nullptr;
        FirstTab = nullptr;
    }
    ProcessDelayedDeletes();
    // Check has focus etc..
    RecurseLayout(m_skin);

    // If we didn't have a next tab, cycle to the start.
    if (NextTab == nullptr)
        NextTab = FirstTab;

    Gwk::Input::OnCanvasThink(this);
}

void Canvas::SetScale(float f)
{
    if (m_fScale == f)
        return;

    m_fScale = f;

    if (m_skin && m_skin->GetRender())
        m_skin->GetRender()->SetScale(m_fScale);

    OnScaleChanged();
    Redraw();
}

void Canvas::AddDelayedDelete(Gwk::Controls::Base* control)
{
    if (!m_bAnyDelete || m_deleteSet.find(control) == m_deleteSet.end())
    {
        m_bAnyDelete = true;
        m_deleteSet.insert(control);
        m_deleteList.push_back(control);
    }
}

void Canvas::PreDeleteCanvas(Gwk::Controls::Base* control)
{
    if (m_bAnyDelete)
    {
        std::set<Controls::Base*>::iterator itFind;

        if ((itFind = m_deleteSet.find(control)) != m_deleteSet.end())
        {
            m_deleteList.remove(control);
            m_deleteSet.erase(control);
            m_bAnyDelete = !m_deleteSet.empty();
        }
    }
}

void Canvas::ProcessDelayedDeletes()
{
    while (m_bAnyDelete)
    {
        m_bAnyDelete = false;
        Controls::Base::List deleteList = m_deleteList;
        m_deleteList.clear();
        m_deleteSet.clear();

        for (auto&& control : deleteList)
        {
            control->PreDelete(GetSkin());
            delete control;
            Redraw();
        }
    }
}

void Canvas::ReleaseChildren()
{
    Base::List::iterator iter = Children.begin();

    while (iter != Children.end())
    {
        Base* child = *iter;
        iter = Children.erase(iter);
        delete child;
    }
}

bool Canvas::InputMouseMoved(int x, int y, int deltaX, int deltaY)
{
    if (Hidden())
        return false;

    if (Tooltip::TooltipActive())
        Redraw();

    // Todo: Handle scaling here..
    // float fScale = 1.0f / Scale();
    Gwk::Input::OnMouseMoved(this, x, y, deltaX, deltaY);

    if (!Gwk::HoveredControl)
        return false;

    if (Gwk::HoveredControl == this)
        return false;

    if (Gwk::HoveredControl->GetCanvas() != this)
        return false;

    Gwk::HoveredControl->OnMouseMoved(x, y, deltaX, deltaY);
    Gwk::HoveredControl->UpdateCursor();
    DragAndDrop::OnMouseMoved(Gwk::HoveredControl, x, y);
    return true;
}

bool Canvas::InputMouseButton(int iButton, bool bDown)
{
    if (Hidden())
        return false;

    return Gwk::Input::OnMouseClicked(this, iButton, bDown);
}

bool Canvas::InputModifierKey(int iKey, bool bDown)
{
    if (Hidden())
        return false;

    if (iKey <= Gwk::Key::Invalid)
        return false;

    if (iKey >= Gwk::Key::KeysCount)
        return false;

    return Gwk::Input::OnKeyEvent(this, iKey, bDown);
}

bool Canvas::InputCharacter(Gwk::UnicodeChar chr)
{
    if (Hidden())
        return false;

    // Check if character is printable, i.e. don't want hidden codes, like backspace.
    if (!std::isprint(chr))
        return false;

    // Handle Accelerators
    if (Gwk::Input::HandleAccelerator(this, chr))
        return true;

    // Handle characters
    if (!Gwk::KeyboardFocus)
        return false;

    if (Gwk::KeyboardFocus->GetCanvas() != this)
        return false;

    if (!Gwk::KeyboardFocus->Visible())
        return false;

    if (Gwk::Input::IsControlDown())
        return false;

    return KeyboardFocus->OnChar(chr);
}

bool Canvas::InputMouseWheel(int val)
{
    if (Hidden())
        return false;

    if (!Gwk::HoveredControl)
        return false;

    if (Gwk::HoveredControl == this)
        return false;

    if (Gwk::HoveredControl->GetCanvas() != this)
        return false;

    return Gwk::HoveredControl->OnMouseWheeled(val);
}
