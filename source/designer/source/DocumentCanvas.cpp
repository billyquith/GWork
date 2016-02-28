#include "DocumentCanvas.h"
#include "Gwork/Util/ControlFactory.h"
#include "SelectionLayer.h"
#include "Utility.h"

GWK_CONTROL_CONSTRUCTOR( DocumentCanvas )
{
	SetShouldDrawBackground( true );

	m_SelectionLayer = new SelectionLayer( this );
	m_SelectionLayer->onSelectionChanged.Add( this, &ThisClass::OnSelectionChanged );
	m_SelectionLayer->onPropertiesChanged.Add( this, &ThisClass::OnPropertiesChanged );
	m_SelectionLayer->onHierachyChanged.Add( this, &ThisClass::OnHierachyChanged );

	ControlFactory::Base* pControlFactory = Gwk::ControlFactory::Find( "DesignerCanvas" );
	UserData.Set( "ControlFactory", pControlFactory );
}


void DocumentCanvas::Render( Gwk::Skin::Base* skin )
{
	skin->DrawGenericPanel( this );
}

void DocumentCanvas::PostLayout( Skin::Base* skin )
{
	m_SelectionLayer->BringToFront();
	m_SelectionLayer->SetBounds( 0, 0, Width(), Height() );
}

bool DocumentCanvas::DragAndDrop_CanAcceptPackage( Gwk::DragAndDrop::Package* pPackage )
{
	return pPackage->name == "ControlSpawn";
}

bool DocumentCanvas::DragAndDrop_HandleDrop( Gwk::DragAndDrop::Package* pPackage, int x, int y )
{
	Gwk::Point pPos = CanvasPosToLocal( Gwk::Point( x, y ) );

	m_SelectionLayer->SetHidden( true );
	Controls::Base* pDroppedOn = GetControlAt( pPos.x, pPos.y );
	pDroppedOn = FindParentControlFactoryControl( pDroppedOn );
	m_SelectionLayer->SetHidden( false );

	if ( !pDroppedOn ) pDroppedOn = this;

	pPos = pDroppedOn->CanvasPosToLocal( Gwk::Point( x, y ) );
	

	if ( pPackage->name == "ControlSpawn" )
	{
		ControlFactory::Base* pControlFactory = static_cast<ControlFactory::Base*>(pPackage->userdata);
		Controls::Base* pControl = pControlFactory->CreateInstance( pDroppedOn );
		pControl->SetPos( pPos );
		pControl->SetMouseInputEnabled( true );

		pControl->UserData.Set( "ControlFactory", pControlFactory );
		onChildAdded.Call( this, Event::Information( pControl ) );

		return true;
	}

	return false;
}

void DocumentCanvas::SelectControls( ControlList& CtrlList )
{
	m_SelectionLayer->ClearSelection();

	for ( ControlList::List::const_iterator it = CtrlList.list.begin(); it != CtrlList.list.end(); ++it )
	{
		m_SelectionLayer->AddSelection( (*it) );
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
		for ( ControlList::List::const_iterator it = m_SelectionLayer->GetSelected().list.begin(); it != m_SelectionLayer->GetSelected().list.end(); ++it )
		{
			if ( *it == this ) continue;
			(*it)->DelayedDelete();
		}

		m_SelectionLayer->ClearSelection();
	}

	if ( str == "bringforward" )
	{
		for ( ControlList::List::const_iterator it = m_SelectionLayer->GetSelected().list.begin(); it != m_SelectionLayer->GetSelected().list.end(); ++it )
		{
			if ( *it == this ) continue;
			(*it)->BringToFront();
		}
	}

	if ( str == "sendback" )
	{
		for ( ControlList::List::const_iterator it = m_SelectionLayer->GetSelected().list.begin(); it != m_SelectionLayer->GetSelected().list.end(); ++it )
		{
			if ( *it == this ) continue;
			(*it)->SendToBack();
		}
	}
}