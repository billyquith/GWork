/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Controls/Canvas.h>
#include <Gwork/Gwork.h>
#include <Gwork/BaseRender.h>
#include <Gwork/Skin.h>
#include <Gwork/Platform.h>
#include <Gwork/DragAndDrop.h>
#include <Gwork/Tooltip.h>
#include <Gwork/Utility.h>
#include <list>

#if GWK_ANIMATE
#   include <Gwork/Anim.h>
#endif



namespace Gwk {
namespace Controls {

Base::Base(Base* parent, const Gwk::String& Name)
{
    m_parent = nullptr;
    m_actualParent = nullptr;
    m_innerPanel = nullptr;
    m_skin = nullptr;
    SetName(Name);
    SetParent(parent);
    m_bHidden = false;
    m_bounds = Gwk::Rect(0, 0, 10, 10);
    m_padding = Padding(0, 0, 0, 0);
    m_margin = Margin(0, 0, 0, 0);
    m_dock = Position::None;
    m_dragAndDrop_Package = nullptr;
    RestrictToParent(false);
    SetMouseInputEnabled(true);
    SetKeyboardInputEnabled(false);
    Invalidate();
    SetCursor(Gwk::CursorType::Normal);
    SetTooltip(nullptr);
    SetTabable(false);
    SetShouldDrawBackground(true);
    m_bDisabled = false;
    m_bCacheTextureDirty = true;
    m_bCacheToTexture = false;
    m_bIncludeInSize = true;
}

Base::~Base()
{
    {
        Canvas* canvas = GetCanvas();

        if (canvas)
            canvas->PreDeleteCanvas(this);
    }

    Base::List::iterator iter = Children.begin();

    while (iter != Children.end())
    {
        Base* child = *iter;
        iter = Children.erase(iter);
        delete child;
    }

    for (AccelMap::iterator accelIt = m_accelerators.begin();
         accelIt != m_accelerators.end();
         ++accelIt)
    {
        delete accelIt->second;
    }

    m_accelerators.clear();
    SetParent(nullptr);

    if (Gwk::HoveredControl == this)
        Gwk::HoveredControl = nullptr;

    if (Gwk::KeyboardFocus == this)
        Gwk::KeyboardFocus = nullptr;

    if (Gwk::MouseFocus == this)
        Gwk::MouseFocus = nullptr;

    DragAndDrop::ControlDeleted(this);
    Tooltip::ControlDeleted(this);
#if GWK_ANIMATE
    Anim::Cancel(this);
#endif

    if (m_dragAndDrop_Package)
    {
        delete m_dragAndDrop_Package;
        m_dragAndDrop_Package = nullptr;
    }
}

void Base::Invalidate()
{
    m_bNeedsLayout = true;
    m_bCacheTextureDirty = true;
}

void Base::DelayedDelete()
{
    Canvas* canvas = GetCanvas();
    canvas->AddDelayedDelete(this);
}

Canvas* Base::GetCanvas()
{
    Base* canvas = m_parent;

    if (!canvas)
        return nullptr;

    return canvas->GetCanvas();
}

void Base::SetParent(Base* parent)
{
    if (m_parent == parent)
        return;

    if (m_parent)
        m_parent->RemoveChild(this);

    m_parent = parent;
    m_actualParent = nullptr;

    if (m_parent)
        m_parent->AddChild(this);
}

void Base::Dock(Position dock)
{
    if (m_dock == dock)
        return;

    m_dock = dock;
    Invalidate();
    InvalidateParent();
}

Position Base::GetDock() const
{
    return m_dock;
}

bool Base::Hidden() const
{
    return m_bHidden;
}

bool Base::Visible() const
{
    if (Hidden())
        return false;

    if (GetParent())
        return GetParent()->Visible();

    return true;
}

void Base::InvalidateChildren(bool bRecursive)
{
    for (auto&& child : Children)
    {
        child->Invalidate();

        if (bRecursive)
            child->InvalidateChildren(bRecursive);
    }

    if (m_innerPanel)
    {
	for (auto&& innerchild : m_innerPanel->Children)
	{
            innerchild->Invalidate();

            if (bRecursive)
                innerchild->InvalidateChildren(bRecursive);
        }
    }
}

void Base::SetPosition(Position pos, int xpadding, int ypadding)
{
    const Rect& bounds = GetParent()->GetInnerBounds();
    const Margin& margin = GetMargin();
    int x = X();
    int y = Y();

    if (pos & Position::Left)
        x = bounds.x + xpadding + margin.left;

    if (pos & Position::Right)
        x = bounds.x + (bounds.w - Width() - xpadding - margin.right);

    if (pos & Position::CenterH)
        x = bounds.x + (bounds.w - Width())/2;

    if (pos & Position::Top)
        y = bounds.y + ypadding;

    if (pos & Position::Bottom)
        y = bounds.y + (bounds.h - Height() - ypadding);

    if (pos & Position::CenterV)
        y = bounds.y + (bounds.h - Height())/2 + ypadding;

    SetPos(x, y);
}

void Base::SendToBack()
{
    if (!m_actualParent)
        return;

    if (m_actualParent->Children.front() == this)
        return;

    m_actualParent->Children.remove(this);
    m_actualParent->Children.push_front(this);
    InvalidateParent();
}

void Base::BringToFront()
{
    if (!m_actualParent)
        return;

    if (m_actualParent->Children.back() == this)
        return;

    m_actualParent->Children.remove(this);
    m_actualParent->Children.push_back(this);
    InvalidateParent();
    Redraw();
}

Controls::Base* Base::FindChildByName(const Gwk::String& name, bool bRecursive)
{
    for (auto&& child : Children)
    {
        if (!child->GetName().empty() && child->GetName() == name)
            return child;

        if (bRecursive)
        {
            Controls::Base* subChild = child->FindChildByName(name, true);

            if (subChild)
                return subChild;
        }
    }

    return nullptr;
}

void Base::BringNextToControl(Controls::Base* child, bool bBehind)
{
    if (!m_actualParent)
        return;

    m_actualParent->Children.remove(this);
    Base::List::iterator it = std::find(
        m_actualParent->Children.begin(), m_actualParent->Children.end(), child);

    if (it == m_actualParent->Children.end())
        return BringToFront();

    if (bBehind)
    {
        ++it;

        if (it == m_actualParent->Children.end())
            return BringToFront();
    }

    m_actualParent->Children.insert(it, this);
    InvalidateParent();
}

void Base::AddChild(Base* child)
{
    if (m_innerPanel)
    {
        m_innerPanel->AddChild(child);
        return;
    }

    Children.push_back(child);
    OnChildAdded(child);
    child->m_actualParent = this;
}

void Base::RemoveChild(Base* child)
{
    // If we removed our innerpanel, remove our pointer to it
    if (m_innerPanel == child)
        m_innerPanel = nullptr;

    if (m_innerPanel)
        m_innerPanel->RemoveChild(child);

    Children.remove(child);
    OnChildRemoved(child);
}

void Base::RemoveAllChildren()
{
    while (Children.size() > 0)
    {
        RemoveChild(*Children.begin());
    }
}

unsigned int Base::NumChildren()
{
    // Include m_innerPanel's children here?
    return static_cast<unsigned int>(Children.size());
}

Controls::Base* Base::GetChild(unsigned int i)
{
    if (i >= NumChildren())
        return nullptr;

    for (auto&& child : Children)
    {
        if (i == 0)
            return child;

        i--;
    }

    // Should never happen.
    return nullptr;
}

void Base::OnChildAdded(Base* /*child*/)
{
    Invalidate();
}

void Base::OnChildRemoved(Base* /*child*/)
{
    Invalidate();
}

Skin::Base* Base::GetSkin(void)
{
    if (m_skin)
        return m_skin;

    if (m_parent)
        return m_parent->GetSkin();

    Debug::AssertCheck(0, "Base::GetSkin Returning nullptr!\n");
    return nullptr;
}

void Base::MoveBy(int x, int y)
{
    MoveTo(X()+x, Y()+y);
}

void Base::MoveTo(int x, int y)
{
    if (m_bRestrictToParent && GetParent())
    {
        Base* parent = GetParent();

        if (x-GetPadding().left < parent->GetMargin().left)
            x = parent->GetMargin().left+GetPadding().left;

        if (y-GetPadding().top < parent->GetMargin().top)
            y = parent->GetMargin().top+GetPadding().top;

        if (x+Width()+GetPadding().right > parent->Width()-parent->GetMargin().right)
            x = parent->Width()-parent->GetMargin().right-Width()-GetPadding().right;

        if (y+Height()+GetPadding().bottom > parent->Height()-parent->GetMargin().bottom)
            y = parent->Height()-parent->GetMargin().bottom-Height()-GetPadding().bottom;
    }

    SetBounds(x, y, Width(), Height());
}

void Base::SetPos(int x, int y)
{
    SetBounds(x, y, Width(), Height());
}

bool Base::SetSize(int w, int h)
{
    return SetBounds(X(), Y(), w, h);
}

bool Base::SetSize(const Point& p)
{
    return SetSize(p.x, p.y);
}

bool Base::SetBounds(const Gwk::Rect& bounds)
{
    if (m_bounds == bounds)
        return false;

    const Gwk::Rect oldBounds = GetBounds();
    m_bounds = bounds;
    OnBoundsChanged(oldBounds);
    return true;
}

bool Base::SetBounds(int x, int y, int w, int h)
{
    return SetBounds(Rect(x,y,w,h));
}

void Base::OnBoundsChanged(Gwk::Rect oldBounds)
{
    // Anything that needs to update on size changes
    // Iterate my children and tell them I've changed
    //
    if (GetParent())
        GetParent()->OnChildBoundsChanged(oldBounds, this);

    if (m_bounds.w != oldBounds.w || m_bounds.h != oldBounds.h)
        Invalidate();

    Redraw();
    UpdateRenderBounds();
}

void Base::OnScaleChanged()
{
    for (auto&& child : Children)
    {
        child->OnScaleChanged();
    }
}

void Base::OnChildBoundsChanged(Gwk::Rect /*oldChildBounds*/, Base* /*child*/)
{
}

void Base::Render(Gwk::Skin::Base* /*skin*/)
{
}

void Base::DoCacheRender(Gwk::Skin::Base* skin, Gwk::Controls::Base* master)
{
    Gwk::Renderer::Base* render = skin->GetRender();
    Gwk::Renderer::ICacheToTexture* cache = render->GetCTT();

    if (!cache)
        return;

    Gwk::Point oldRenderOffset = render->GetRenderOffset();
    Gwk::Rect rOldRegion = render->ClipRegion();

    if (this != master)
    {
        render->AddRenderOffset(GetBounds());
        render->AddClipRegion(GetBounds());
    }
    else
    {
        render->SetRenderOffset(Gwk::Point(0, 0));
        render->SetClipRegion(GetBounds());
    }

    // See if we need to update the cached texture. Dirty?
    if (m_bCacheTextureDirty && render->ClipRegionVisible())
    {
        render->StartClip();
        {
            if (IsCachedToTexture())
                cache->SetupCacheTexture(this);

            // Render myself first
            Render(skin);

            if (!Children.empty())
            {
                // Now render my kids
                for (auto&& child : Children)
                {
                    if (!child->Hidden())
                    {
                        // Draw child control using normal render. If it is cached it will
                        // be handled in the same way as this one.
                        child->DoRender(skin);
                    }
                }
            }

            if (IsCachedToTexture())
            {
                cache->FinishCacheTexture(this);
                m_bCacheTextureDirty = false;
            }
        }
        render->EndClip();
    }

    // Draw the cached texture.
    render->SetClipRegion(rOldRegion);
    render->StartClip();
    {
        render->SetRenderOffset(oldRenderOffset);
        cache->DrawCachedControlTexture(this);
    }
    render->EndClip();
}

void Base::DoRender(Gwk::Skin::Base* skin)
{
    // If this control has a different skin,
    // then so does its children.
    if (m_skin)
        skin = m_skin;

    // Do think
    Think();
    Gwk::Renderer::Base* render = skin->GetRender();

    if (render->GetCTT() && IsCachedToTexture())
    {
        DoCacheRender(skin, this);
        return;
    }

    RenderRecursive(skin, GetBounds());
}

void Base::RenderRecursive(Gwk::Skin::Base* skin, const Gwk::Rect& cliprect)
{
    Gwk::Renderer::Base* render = skin->GetRender();
    Gwk::Point oldRenderOffset = render->GetRenderOffset();
    render->AddRenderOffset(cliprect);
    RenderUnder(skin);
    Gwk::Rect rOldRegion = render->ClipRegion();

    //
    // If this control is clipping, change the clip rect to ourselves
    // ( if not then we still clip using our parents clip rect )
    //
    if (ShouldClip())
    {
        render->AddClipRegion(cliprect);

        if (!render->ClipRegionVisible())
        {
            render->SetRenderOffset(oldRenderOffset);
            render->SetClipRegion(rOldRegion);
            return;
        }
    }

    //
    // Render this control and children controls
    //
    render->StartClip();
    {
        Render(skin);

        if (!Children.empty())
        {
            // Now render my kids
            for (auto&& child : Children)
            {
                if (child->Hidden())
                    continue;

                child->DoRender(skin);
            }
        }
    }
    render->EndClip();
    //
    // Render overlay/focus
    //
    {
        render->SetClipRegion(rOldRegion);
        render->StartClip();
        {
            RenderOver(skin);
            RenderFocus(skin);
        }
        render->EndClip();
        render->SetRenderOffset(oldRenderOffset);
    }
}

void Base::SetSkin(Skin::Base* skin, bool doChildren)
{
    if (m_skin == skin)
        return;

    m_skin = skin;
    Invalidate();
    Redraw();
    OnSkinChanged(skin);

    if (doChildren)
    {
        for (auto&& child : Children)
        {
            child->SetSkin(skin, true);
        }
    }
}

void Base::OnSkinChanged(Skin::Base* /*skin*/)
{
    // Do something
}

bool Base::OnMouseWheeled(int iDelta)
{
    if (m_actualParent)
        return m_actualParent->OnMouseWheeled(iDelta);

    return false;
}

void Base::OnMouseMoved(int /*x*/, int /*y*/, int /*deltaX*/, int /*deltaY*/)
{
}

void Base::OnMouseEnter()
{
    onHoverEnter.Call(this);

    if (GetTooltip())
        Tooltip::Enable(this);
    else if (GetParent() && GetParent()->GetTooltip())
        Tooltip::Enable(GetParent());

    Redraw();
}

void Base::OnMouseLeave()
{
    onHoverLeave.Call(this);

    if (GetTooltip())
        Tooltip::Disable(this);

    Redraw();
}

bool Base::IsHovered() const
{
    return Gwk::HoveredControl == this;
}

bool Base::ShouldDrawHover() const
{
    return Gwk::MouseFocus == this || Gwk::MouseFocus == nullptr;
}

bool Base::IsFocussed()
{
    return Gwk::KeyboardFocus == this;
}

void Base::Focus()
{
    if (Gwk::KeyboardFocus == this)
        return;

    if (Gwk::KeyboardFocus)
        Gwk::KeyboardFocus->OnLostKeyboardFocus();

    Gwk::KeyboardFocus = this;
    OnKeyboardFocus();
    Redraw();
}

void Base::Unfocus()
{
    if (Gwk::KeyboardFocus != this)
        return;

    Gwk::KeyboardFocus = nullptr;
    OnLostKeyboardFocus();
    Redraw();
}

bool Base::IsOnTop()
{
    if (!GetParent())
        return false;

    Base::List::iterator iter = GetParent()->Children.begin();
    Base* child = *iter;

    if (child == this)
        return true;

    return false;
}

void Base::Touch()
{
    if (GetParent())
        GetParent()->OnChildTouched(this);
}

void Base::OnChildTouched(Controls::Base* /*child*/)
{
    Touch();
}

Base* Base::GetControlAt(int x, int y, bool bOnlyIfMouseEnabled)
{
    if (Hidden())
        return nullptr;

    if (x < 0 || y < 0 || x >= Width() || y >= Height())
        return nullptr;

    for (Base::List::reverse_iterator iter = Children.rbegin(); iter != Children.rend(); ++iter)
    {
        Base* child = *iter;
        Base* found = nullptr;
        found = child->GetControlAt(x-child->X(), y-child->Y(), bOnlyIfMouseEnabled);

        if (found)
            return found;
    }

    if (bOnlyIfMouseEnabled && !GetMouseInputEnabled())
        return nullptr;

    return this;
}

void Base::Layout(Skin::Base* skin)
{
    if (skin->GetRender()->GetCTT() && IsCachedToTexture())
        skin->GetRender()->GetCTT()->CreateControlCacheTexture(this, this->GetBounds().GetSize());
}

void Base::RecurseLayout(Skin::Base* skin)
{
    if (m_skin)
        skin = m_skin;

    if (Hidden())
        return;

    if (NeedsLayout())
    {
        m_bNeedsLayout = false;
        Layout(skin);
    }

    Gwk::Rect rBounds = GetRenderBounds();

    // Adjust bounds for padding
    rBounds.x += m_padding.left;
    rBounds.w -= m_padding.left + m_padding.right;
    rBounds.y += m_padding.top;
    rBounds.h -= m_padding.top + m_padding.bottom;

    for (auto&& child : Children)
    {
        if (child->Hidden())
            continue;

        Position dock = child->GetDock();

        if (dock & Position::Fill)
            continue;

        if (dock & Position::Top)
        {
            const Margin& margin = child->GetMargin();
            child->SetBounds(rBounds.x+margin.left,
                              rBounds.y+margin.top,
                              rBounds.w-margin.left-margin.right,
                              child->Height());
            int iHeight = margin.top+margin.bottom+child->Height();
            rBounds.y += iHeight;
            rBounds.h -= iHeight;
        }

        if (dock & Position::Left)
        {
            const Margin& margin = child->GetMargin();
            child->SetBounds(rBounds.x+margin.left,
                              rBounds.y+margin.top,
                              child->Width(),
                              rBounds.h-margin.top-margin.bottom);
            int iWidth = margin.left+margin.right+child->Width();
            rBounds.x += iWidth;
            rBounds.w -= iWidth;
        }

        if (dock & Position::Right)
        {
            // TODO: THIS MARGIN CODE MIGHT NOT BE FULLY FUNCTIONAL
            const Margin& margin = child->GetMargin();
            child->SetBounds((rBounds.x+rBounds.w)-child->Width()-margin.right,
                              rBounds.y+margin.top,
                              child->Width(),
                              rBounds.h-margin.top-margin.bottom);
            int iWidth = margin.left+margin.right+child->Width();
            rBounds.w -= iWidth;
        }

        if (dock & Position::Bottom)
        {
            // TODO: THIS MARGIN CODE MIGHT NOT BE FULLY FUNCTIONAL
            const Margin& margin = child->GetMargin();
            child->SetBounds(rBounds.x+margin.left,
                              (rBounds.y+rBounds.h)-child->Height()-margin.bottom,
                              rBounds.w-margin.left-margin.right,
                              child->Height());
            rBounds.h -= child->Height()+margin.bottom+margin.top;
        }

        child->RecurseLayout(skin);
    }

    m_innerBounds = rBounds; 

    //
    // Fill uses the left over space, so do that now.
    //
    for (auto&& child : Children)
    {
        Position dock = child->GetDock();

        if (!(dock & Position::Fill))
            continue;

        const Margin& margin = child->GetMargin();
        child->SetBounds(rBounds.x+margin.left, rBounds.y+margin.top,
                         rBounds.w-margin.left-margin.right, rBounds.h-margin.top-margin.bottom);
        child->RecurseLayout(skin);
    }

    PostLayout(skin);

    if (IsTabable() && !IsDisabled())
    {
        if (!GetCanvas()->FirstTab)
            GetCanvas()->FirstTab = this;

        if (!GetCanvas()->NextTab)
            GetCanvas()->NextTab = this;
    }

    if (Gwk::KeyboardFocus == this)
        GetCanvas()->NextTab = nullptr;
}

bool Base::IsChild(Controls::Base* possiblechild)
{
    for (auto&& child : Children)
    {
        if (possiblechild == child)
            return true;
    }

    return false;
}

Gwk::Point Base::LocalPosToCanvas(const Gwk::Point& pnt)
{
    if (m_parent)
    {
        int x = pnt.x+X();
        int y = pnt.y+Y();

        // If our parent has an innerpanel and we're a child of it
        // add its offset onto us.
        //
        if (m_parent->m_innerPanel && m_parent->m_innerPanel->IsChild(this))
        {
            x += m_parent->m_innerPanel->X();
            y += m_parent->m_innerPanel->Y();
        }

        return m_parent->LocalPosToCanvas(Gwk::Point(x, y));
    }

    return pnt;
}

Gwk::Point Base::CanvasPosToLocal(const Gwk::Point& pnt)
{
    if (m_parent)
    {
        int x = pnt.x-X();
        int y = pnt.y-Y();

        // If our parent has an innerpanel and we're a child of it
        // add its offset onto us.
        //
        if (m_parent->m_innerPanel && m_parent->m_innerPanel->IsChild(this))
        {
            x -= m_parent->m_innerPanel->X();
            y -= m_parent->m_innerPanel->Y();
        }

        return m_parent->CanvasPosToLocal(Gwk::Point(x, y));
    }

    return pnt;
}

bool Base::IsMenuComponent()
{
    if (!m_parent)
        return false;

    return m_parent->IsMenuComponent();
}

void Base::CloseMenus()
{
    for (auto&& child : Children)
    {
        child->CloseMenus();
    }
}

void Base::UpdateRenderBounds()
{
    m_renderBounds.x = 0;
    m_renderBounds.y = 0;
    m_renderBounds.w = m_bounds.w;
    m_renderBounds.h = m_bounds.h;
}

void Base::UpdateCursor()
{
    Platform::SetCursor(m_cursor);
}

DragAndDrop::Package* Base::DragAndDrop_GetPackage(int /*x*/, int /*y*/)
{
    return m_dragAndDrop_Package;
}

bool Base::DragAndDrop_HandleDrop(Gwk::DragAndDrop::Package* /*package*/, int /*x*/, int /*y*/)
{
    DragAndDrop::SourceControl->SetParent(this);
    return true;
}

bool Base::DragAndDrop_Draggable()
{
    if (!m_dragAndDrop_Package)
        return false;

    return m_dragAndDrop_Package->draggable;
}

void Base::DragAndDrop_SetPackage(bool bDraggable, const String& strName, void* userData)
{
    if (!m_dragAndDrop_Package)
        m_dragAndDrop_Package = new Gwk::DragAndDrop::Package();

    m_dragAndDrop_Package->draggable = bDraggable;
    m_dragAndDrop_Package->name = strName;
    m_dragAndDrop_Package->userdata = userData;
}

void Base::DragAndDrop_StartDragging(Gwk::DragAndDrop::Package* package, int x, int y)
{
    package->holdoffset = CanvasPosToLocal(Gwk::Point(x, y));
    package->drawcontrol = this;
}

bool Base::SizeToChildren(bool w, bool h)
{
    Gwk::Point size = ChildrenSize();
    size.y += GetPadding().bottom;
    size.x += GetPadding().right;
    return SetSize(w ? size.x : Width(), h ? size.y : Height());
}

Gwk::Point Base::ChildrenSize()
{
    Gwk::Point size;

    for (auto&& child : Children)
    {
        if (child->Hidden())
            continue;

        if (!child->ShouldIncludeInSize())
            continue;

        size.x = std::max(size.x, child->Right());
        size.y = std::max(size.y, child->Bottom());
    }

    return size;
}

void Base::SetPadding(const Padding& padding)
{
    if (m_padding.left == padding.left &&
        m_padding.top == padding.top &&
        m_padding.right == padding.right &&
        m_padding.bottom == padding.bottom)
        return;

    m_padding = padding;
    Invalidate();
    InvalidateParent();
}

void Base::SetMargin(const Margin& margin)
{
    if (m_margin.top == margin.top &&
        m_margin.left == margin.left &&
        m_margin.bottom == margin.bottom &&
        m_margin.right == margin.right)
        return;

    m_margin = margin;
    Invalidate();
    InvalidateParent();
}

bool Base::HandleAccelerator(Gwk::String& accelerator)
{
    if (Gwk::KeyboardFocus == this || !AccelOnlyFocus())
    {
        AccelMap::iterator iter = m_accelerators.find(accelerator);

        if (iter != m_accelerators.end())
        {
            iter->second->Call(this);
            return true;
        }
    }

    for (auto&& child : Children)
    {
        if (child->HandleAccelerator(accelerator))
            return true;
    }

    return false;
}

bool Base::OnKeyPress(int iKey, bool bPress)
{
    bool bHandled = false;

    switch (iKey)
    {
    case Key::Tab:
        bHandled = OnKeyTab(bPress);
        break;

    case Key::Space:
        bHandled = OnKeySpace(bPress);
        break;

    case Key::Home:
        bHandled = OnKeyHome(bPress);
        break;

    case Key::End:
        bHandled = OnKeyEnd(bPress);
        break;

    case Key::Return:
        bHandled = OnKeyReturn(bPress);
        break;

    case Key::Backspace:
        bHandled = OnKeyBackspace(bPress);
        break;

    case Key::Delete:
        bHandled = OnKeyDelete(bPress);
        break;

    case Key::Right:
        bHandled = OnKeyRight(bPress);
        break;

    case Key::Left:
        bHandled = OnKeyLeft(bPress);
        break;

    case Key::Up:
        bHandled = OnKeyUp(bPress);
        break;

    case Key::Down:
        bHandled = OnKeyDown(bPress);
        break;

    case Key::Escape:
        bHandled = OnKeyEscape(bPress);
        break;

    default:
        break;
    }

    if (!bHandled && GetParent())
        GetParent()->OnKeyPress(iKey, bPress);

    return bHandled;
}

bool Base::OnKeyRelease(int iKey)
{
    return OnKeyPress(iKey, false);
}

bool Base::OnKeyTab(bool bDown)
{
    if (!bDown)
        return true;

    if (GetCanvas()->NextTab)
    {
        GetCanvas()->NextTab->Focus();
        Redraw();
    }

    return true;
}

void Base::RenderFocus(Gwk::Skin::Base* skin)
{
    if (Gwk::KeyboardFocus != this)
        return;

    if (!IsTabable())
        return;

    skin->DrawKeyboardHighlight(this, GetRenderBounds(), 3);
}

void Base::SetTooltipText(const String& strText)
{
    Label* tooltip = new Label(this);
    tooltip->SetText(strText);
    tooltip->SetTextColorOverride(GetSkin()->Colors.TooltipText);
    tooltip->SetPadding(Padding(5, 3, 5, 3));
    tooltip->SizeToContents();
    SetTooltip(tooltip);
}

String Base::GetChildValue(const Gwk::String& strName)
{
    Base* child = FindChildByName(strName, true);

    if (!child)
        return "";

    return child->GetValue();
}

String Base::GetValue()
{
    // Generic value accessor should be filled in if we have a value to give.
    return "";
}

void Base::SetValue(const String& strValue)
{
}

int Base::GetNamedChildren(Gwk::ControlList& list, const Gwk::String& strName, bool bDeep)
{
    int iFound = 0;

    for (auto&& child : Children)
    {
        if (!child->GetName().empty() && child->GetName() == strName)
        {
            list.Add(child);
            iFound++;
        }

        if (!bDeep)
            continue;

        iFound += child->GetNamedChildren(list, strName, bDeep);
    }

    return iFound;
}

Gwk::ControlList Base::GetNamedChildren(const Gwk::String& strName, bool bDeep)
{
    Gwk::ControlList list;
    GetNamedChildren(list, strName, bDeep);
    return list;
}

#if GWK_ANIMATE

void Base::Anim_widthIn(float fLength, float fDelay, float fEase)
{
    Gwk::Anim::Add(this, new Gwk::Anim::Size::Width(0, Width(), fLength, false, fDelay, fEase));
    SetWidth(0);
}

void Base::Anim_heightIn(float fLength, float fDelay, float fEase)
{
    Gwk::Anim::Add(this,
                    new Gwk::Anim::Size::Height(0, Height(), fLength, false, fDelay, fEase));
    SetHeight(0);
}

void Base::Anim_widthOut(float fLength, bool bHide, float fDelay, float fEase)
{
    Gwk::Anim::Add(this, new Gwk::Anim::Size::Width(Width(), 0, fLength, bHide, fDelay, fEase));
}

void Base::Anim_heightOut(float fLength, bool bHide, float fDelay, float fEase)
{
    Gwk::Anim::Add(this,
                    new Gwk::Anim::Size::Height(Height(), 0, fLength, bHide, fDelay, fEase));
}

#endif // if GWK_ANIMATE

    
} // namespace Gwk
} // namespace Controls

