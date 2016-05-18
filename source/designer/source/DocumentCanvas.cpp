#include "DocumentCanvas.h"
#include <Gwork/Util/ControlFactory.h>
#include "SelectionLayer.h"
#include "Utility.h"

GWK_CONTROL_CONSTRUCTOR( DocumentCanvas )
{
	SetShouldDrawBackground( true );

	m_selectionLayer = new SelectionLayer( this );
	m_selectionLayer->onSelectionChanged.Add( this, &ThisClass::OnSelectionChanged );
	m_selectionLayer->onPropertiesChanged.Add( this, &ThisClass::OnPropertiesChanged );
	m_selectionLayer->onHierachyChanged.Add( this, &ThisClass::OnHierachyChanged );

	ControlFactory::Base* controlFactory = Gwk::ControlFactory::Find( "DesignerCanvas" );
	UserData.Set( "ControlFactory", controlFactory );
}

void DocumentCanvas::Render( Gwk::Skin::Base* skin )
{
	skin->DrawGenericPanel( this );
}

void DocumentCanvas::PostLayout( Skin::Base* skin )
{
	m_selectionLayer->BringToFront();
	m_selectionLayer->SetBounds( 0, 0, Width(), Height() );
}

bool DocumentCanvas::DragAndDrop_CanAcceptPackage( Gwk::DragAndDrop::Package* package )
{
	return package->name == "ControlSpawn";
}

bool DocumentCanvas::DragAndDrop_HandleDrop( Gwk::DragAndDrop::Package* package, int x, int y )
{
	Gwk::Point pos = CanvasPosToLocal( Gwk::Point( x, y ) );

	m_selectionLayer->SetHidden( true );
	Controls::Base* droppedOn = GetControlAt( pos.x, pos.y );
	droppedOn = FindParentControlFactoryControl( droppedOn );
	m_selectionLayer->SetHidden( false );

	if ( !droppedOn ) droppedOn = this;

	pos = droppedOn->CanvasPosToLocal( Gwk::Point( x, y ) );
	

	if ( package->name == "ControlSpawn" )
	{
		ControlFactory::Base* controlFactory = static_cast<ControlFactory::Base*>(package->userdata);
		Controls::Base* control = controlFactory->CreateInstance( droppedOn );
		control->SetPos( pos );
		control->SetMouseInputEnabled( true );

		control->UserData.Set( "ControlFactory", controlFactory );
		onChildAdded.Call( this, Event::Information( control ) );

		return true;
	}

	return false;
}

void DocumentCanvas::SelectControls( ControlList& CtrlList )
{
	m_selectionLayer->ClearSelection();

	for (auto const& ctrl : CtrlList.list)
	{
		m_selectionLayer->AddSelection( ctrl );
	}

	// Let everything else know the selection changed
	{
		Event::Information info;
		info.ControlList = CtrlList;
		onSelectionChanged.Call( this, info );
	}
}

void DocumentCanvas::OnSelectionChanged( Event::Info info )
{
	onSelectionChanged.Call( this, info );
}

void DocumentCanvas::OnPropertiesChanged( Event::Info info )
{
	onPropertiesChanged.Call( this, info );
}

void DocumentCanvas::OnHierachyChanged( Event::Info info )
{
	onHierachyChanged.Call( this, info );
}

void DocumentCanvas::Command( const Gwk::String& str )
{
	if ( str == "delete" )
	{
		for ( auto&& selected : m_selectionLayer->GetSelected().list)
		{
			if ( selected == this ) continue;
			selected->DelayedDelete();
		}

		m_selectionLayer->ClearSelection();
	}

	if ( str == "bringforward" )
	{
		for ( auto&& selected : m_selectionLayer->GetSelected().list)
		{
			if ( selected == this ) continue;
			selected->BringToFront();
		}
	}

	if ( str == "sendback" )
	{
		for ( auto&& selected : m_selectionLayer->GetSelected().list)
		{
			if ( selected == this ) continue;
			selected->SendToBack();
		}
	}
}
