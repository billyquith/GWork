#include "SelectionLayer.h"
#include "Cage.h"
#include <Gwork/Util/ControlFactory.h>
#include "Utility.h"

GWK_CONTROL_CONSTRUCTOR( SelectionLayer )
{
	SetShouldDrawBackground( true );
}

void SelectionLayer::ClearSelection()
{
	m_selected.Clear();
	RemoveAllChildren();
	Redraw();
}

void SelectionLayer::AddSelection( Controls::Base* control )
{
	Cage* cage = new Cage( this );
	cage->Setup( control );
	cage->onMoved.Add( this, &ThisClass::OnControlDragged );
	cage->onPress.Add( this, &ThisClass::OnCagePressed );
	cage->onMoving.Add( this, &ThisClass::OnCageMoving );
	cage->onDragStart.Add( this, &ThisClass::OnDragStart );

	m_selected.Add( control );

	Redraw();
}

void SelectionLayer::RemoveSelection( Controls::Base* control )
{
	m_selected.Remove( control );

	for ( int i=0; i<NumChildren(); i++ )
	{
		Cage* cage = gwk_cast<Cage>( GetChild( i ) );
		if  (!cage ) continue;

		if ( cage->Target() == control )
			cage->DelayedDelete();
	}

	Redraw();
}

void SelectionLayer::OnMouseClickLeft( int x, int y, bool bDown )
{
	if ( !bDown ) return;

	Gwk::Point pos = GetParent()->CanvasPosToLocal( Gwk::Point( x, y ) );
	
	SetMouseInputEnabled( false );

	Controls::Base* child = GetParent()->GetControlAt( pos.x, pos.y );
	Controls::Base* ctrl = FindParentControlFactoryControl( child );

	SetMouseInputEnabled( true );

	bool bPanelsWereSelected = !m_selected.list.empty();


	if ( !Gwk::Input::IsShiftDown() )
	{
		ClearSelection();
	}

	if ( ctrl )
	{

		if ( ctrl != child )
		{
			Gwk::ControlFactory::Base* factory =
				ctrl->UserData.Get<Gwk::ControlFactory::Base*>( "ControlFactory" );

			if ( factory->ChildTouched( ctrl, child ) )
				return;
		}

			if ( ctrl == GetParent() )
			{
				//
				// We only select the canvas if no other panels were selected before
				//
				if ( !bPanelsWereSelected )
					AddSelection( ctrl );
			}
			else
			{
				AddSelection( ctrl );
			}
	}

	Event::Information info;
	info.ControlList = m_selected;

	onSelectionChanged.Call( this, info );
}

void SelectionLayer::OnControlDragged( Event::Info info )
{
	for ( ControlList::List::const_iterator it = m_selected.list.cbegin(); it != m_selected.list.cend(); ++it )
	{
		Controls::Base* control = (*it);
		control->MoveBy( info.Point.x, info.Point.y );
	}

	Event::Information inf;
	inf.ControlList = m_selected;
	onPropertiesChanged.Call( this, inf );
}

void SelectionLayer::OnCagePressed( Event::Info info )
{
	Cage* cage = gwk_cast<Cage>( info.ControlCaller );
	if ( !cage ) return;

	RemoveSelection( cage->Target() );
	cage->DelayedDelete();	

	// Let everything else know the selection changed
	{
		Event::Information info;
		info.ControlList = m_selected;
		onSelectionChanged.Call( this, info );
	}
}

void SelectionLayer::OnCageMoving( Event::Info info )
{
	//
	// If we have the canvas selected - then don't do anything.
	//
	if ( m_selected.Contains( GetParent() ) )
		return;

	//
	// Convert the passed canvas pos to a pos local to the canvas
	//
	Gwk::Point pos = GetParent()->CanvasPosToLocal( info.Point );
	
	// Hide all of the selected panels, and this selection layer
	{
		for ( ControlList::List::const_iterator it = m_selected.list.cbegin();
			  it != m_selected.list.cend(); ++it )
		{
			(*it)->SetHidden( true );
		}

		SetHidden( true );
	}

	// Find out which control is under our cursor
	Controls::Base* child = GetParent()->GetControlAt( pos.x, pos.y );
	Controls::Base* ctrl = FindParentControlFactoryControl( child );
		
	bool bHierachyChanged = false;

	// Show all the hidden panels
	{
		SetHidden( false );
		
		for ( ControlList::List::iterator it = m_selected.list.begin();
              it != m_selected.list.end(); ++it )
		{
			(*it)->SetHidden( false );
			
			if ( (*it) == GetParent() ) continue;

			if ( !ctrl ) continue;
			if ( !ctrl->UserData.Exists( "ControlFactory" ) ) continue;

			Gwk::ControlFactory::Base* factory =
                ctrl->UserData.Get<Gwk::ControlFactory::Base*>( "ControlFactory" );

			Controls::Base* oldParent = (*it)->GetParent();

			// If the panel we're dragging doesn't have the parent thats underneath
			// then make it have it. Tweak positions so they're the same
			if ( ctrl && ctrl != (*it)->GetParent() )
			{
				Gwk::Point pos = (*it)->LocalPosToCanvas();
				factory->AddChild( ctrl, *it, ctrl->CanvasPosToLocal( info.Point ) );
				(*it)->SetPos( (*it)->GetParent()->CanvasPosToLocal( pos ) );

				bHierachyChanged = bHierachyChanged || ( oldParent != (*it)->GetParent() );
			}
		}
	}

	//
	// If any parents changed then fire an event to update the tree
	//
	if ( bHierachyChanged )
	{
		onHierachyChanged.Call( this );
	}

}

void SelectionLayer::OnDragStart()
{
	//
	// If shift dragging, duplicate the selected panels
	//
	if ( !Gwk::Input::IsShiftDown() ) return;

	ControlList NewList;

	for ( ControlList::List::const_iterator it = m_selected.list.cbegin(); it != m_selected.list.cend(); ++it )
	{
		Gwk::ControlFactory::Base* factory = (*it)->UserData.Get<Gwk::ControlFactory::Base*>( "ControlFactory" );
		Controls::Base* control = ControlFactory::Clone( *it, factory );
		control->UserData.Set( "ControlFactory", factory );
		control->SetMouseInputEnabled( true );
		NewList.Add( control );

		SwitchCage( *it, control );
	}

	m_selected = NewList;
}

void SelectionLayer::SwitchCage( Controls::Base* control, Controls::Base* to )
{
	for ( int i=0; i<NumChildren(); i++ )
	{
		Cage* cage = gwk_cast<Cage>( GetChild( i ) );
		if  (!cage ) continue;

		if ( cage->Target() == control )
		{
			cage->Setup( to );
		}
	}
}
