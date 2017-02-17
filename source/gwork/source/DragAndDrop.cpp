/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/DragAndDrop.h>
#include <Gwork/Utility.h>
#include <Gwork/Platform.h>

namespace Gwk
{
    
using namespace Gwk::DragAndDrop;

DragAndDrop::Package* DragAndDrop::CurrentPackage = nullptr;
Gwk::Controls::Base* DragAndDrop::HoveredControl = nullptr;
Gwk::Controls::Base* DragAndDrop::SourceControl = nullptr;

static Gwk::Controls::Base* LastPressedControl = nullptr;
static Gwk::Controls::Base* NewHoveredControl = nullptr;
static Gwk::Point LastPressedPos;

void DragAndDrop::ControlDeleted(Gwk::Controls::Base* control)
{
    if (SourceControl == control)
    {
        SourceControl = nullptr;
        CurrentPackage = nullptr;
        HoveredControl = nullptr;
        LastPressedControl = nullptr;
    }

    if (LastPressedControl == control)
        LastPressedControl = nullptr;

    if (HoveredControl == control)
        HoveredControl = nullptr;

    if (NewHoveredControl == control)
        NewHoveredControl = nullptr;
}

static int m_mouseX = 0;
static int m_mouseY = 0;

bool DragAndDrop::Start(Gwk::Controls::Base* control, Package* package)
{
    if (CurrentPackage)
        return false;

    CurrentPackage = package;
    SourceControl = control;
    return true;
}

bool OnDrop(int x, int y)
{
    bool bSuccess = false;

    if (DragAndDrop::HoveredControl)
    {
        DragAndDrop::HoveredControl->DragAndDrop_HoverLeave(DragAndDrop::CurrentPackage);
        bSuccess = DragAndDrop::HoveredControl->DragAndDrop_HandleDrop(DragAndDrop::CurrentPackage,
                                                                       x, y);
    }

    // Report back to the source control, to tell it if we've been successful.
    DragAndDrop::SourceControl->DragAndDrop_EndDragging(bSuccess, x, y);
    DragAndDrop::SourceControl->Redraw();
    DragAndDrop::CurrentPackage = nullptr;
    DragAndDrop::SourceControl = nullptr;
    return true;
}

bool DragAndDrop::OnMouseButton(Gwk::Controls::Base* hoveredControl, int x, int y, bool bDown)
{
    if (!bDown)
    {
        LastPressedControl = nullptr;

        // Not carrying anything, allow normal actions
        if (!CurrentPackage)
            return false;

        // We were carrying something, drop it.
        OnDrop(x, y);
        return true;
    }

    if (!hoveredControl)
        return false;

    if (!hoveredControl->DragAndDrop_Draggable())
        return false;

    // Store the last clicked on control. Don't do anything yet,
    // we'll check it in OnMouseMoved, and if it moves further than
    // x pixels with the mouse down, we'll start to drag.
    LastPressedPos = Gwk::Point(x, y);
    LastPressedControl = hoveredControl;
    return false;
}

bool ShouldStartDraggingControl(int x, int y)
{
    // We're not holding a control down..
    if (!LastPressedControl)
        return false;

    // Not been dragged far enough
    int iLength = std::abs(x-LastPressedPos.x) + std::abs(y-LastPressedPos.y);

    if (iLength < 5)
        return false;

    // Create the dragging package
    DragAndDrop::CurrentPackage = LastPressedControl->DragAndDrop_GetPackage(LastPressedPos.x,
                                                                             LastPressedPos.y);

    // We didn't create a package!
    if (!DragAndDrop::CurrentPackage)
    {
        LastPressedControl = nullptr;
        DragAndDrop::SourceControl = nullptr;
        return false;
    }

    // Now we're dragging something!
    DragAndDrop::SourceControl = LastPressedControl;
    Gwk::MouseFocus = nullptr;
    LastPressedControl = nullptr;
    DragAndDrop::CurrentPackage->drawcontrol = nullptr;

    // Some controls will want to decide whether they should be dragged at that
    // moment.
    // This function is for them (it defaults to true)
    if (!DragAndDrop::SourceControl->DragAndDrop_ShouldStartDrag())
    {
        DragAndDrop::SourceControl = nullptr;
        DragAndDrop::CurrentPackage = nullptr;
        return false;
    }

    DragAndDrop::SourceControl->DragAndDrop_StartDragging(DragAndDrop::CurrentPackage,
                                                          LastPressedPos.x, LastPressedPos.y);
    return true;
}

void UpdateHoveredControl(Gwk::Controls::Base* ctrl, int x, int y)
{
    //
    // We use this global variable to represent our hovered control
    // That way, if the new hovered control gets deleted in one of the
    // Hover callbacks, we won't be left with a hanging pointer.
    // This isn't ideal - but it's minimal.
    //
    NewHoveredControl = ctrl;

    // Nothing to change..
    if (DragAndDrop::HoveredControl == NewHoveredControl)
        return;

    // We changed - tell the old hovered control that it's no longer hovered.
    if (DragAndDrop::HoveredControl && DragAndDrop::HoveredControl != NewHoveredControl)
        DragAndDrop::HoveredControl->DragAndDrop_HoverLeave(DragAndDrop::CurrentPackage);

    // If we're hovering where the control came from, just forget it.
    // By changing it to nullptr here we're not going to show any error cursors
    // it will just do nothing if you drop it.
    if (NewHoveredControl == DragAndDrop::SourceControl)
        NewHoveredControl = nullptr;

    // Check to see if the new potential control can accept this type of
    // package.
    // If not, ignore it and show an error cursor.
    while (NewHoveredControl &&
           !NewHoveredControl->DragAndDrop_CanAcceptPackage(DragAndDrop::CurrentPackage))
    {
        // We can't drop on this control, so lets try to drop
        // onto its parent..
        NewHoveredControl = NewHoveredControl->GetParent();

        // Its parents are dead. We can't drop it here.
        // Show the NO WAY cursor.
        if (!NewHoveredControl)
            Platform::SetCursor(CursorType::No);
    }

    // Become out new hovered control
    DragAndDrop::HoveredControl = NewHoveredControl;

    // If we exist, tell us that we've started hovering.
    if (DragAndDrop::HoveredControl)
        DragAndDrop::HoveredControl->DragAndDrop_HoverEnter(DragAndDrop::CurrentPackage, x, y);

    NewHoveredControl = nullptr;
}

void DragAndDrop::OnMouseMoved(Gwk::Controls::Base* hoveredControl, int x, int y)
{
    // Always keep these up to date, they're used to draw the dragged control.
    m_mouseX = x;
    m_mouseY = y;

    // If we're not carrying anything, then check to see if we should
    // pick up from a control that we're holding down. If not, then forget it.
    if (!CurrentPackage && !ShouldStartDraggingControl(x, y))
        return;

    // Make sure the canvas redraws when we move
    if (CurrentPackage && CurrentPackage->drawcontrol)
        CurrentPackage->drawcontrol->Redraw();

    // Swap to this new hovered control and notify them of the change.
    UpdateHoveredControl(hoveredControl, x, y);

    if (!HoveredControl)
        return;

    // Update the hovered control every mouse move, so it can show where
    // the dropped control will land etc..
    HoveredControl->DragAndDrop_Hover(CurrentPackage, x, y);
    // Override the cursor - since it might have been set my underlying controls
    // Ideally this would show the 'being dragged' control. TODO
    Platform::SetCursor(CursorType::Normal);
    hoveredControl->Redraw();
}

void DragAndDrop::RenderOverlay(Gwk::Controls::Canvas* /*canvas*/, Skin::Base* skin)
{
    if (!CurrentPackage)
        return;

    if (!CurrentPackage->drawcontrol)
        return;

    Gwk::Point pntOld = skin->GetRender()->GetRenderOffset();
    skin->GetRender()->AddRenderOffset(Gwk::Rect(m_mouseX-SourceControl->X()-
                                                  CurrentPackage->holdoffset.x, m_mouseY-
                                                  SourceControl->Y()-CurrentPackage->holdoffset.y,
                                                  0, 0));
    CurrentPackage->drawcontrol->DoRender(skin);
    skin->GetRender()->SetRenderOffset(pntOld);
}

} // namespace Gwk
