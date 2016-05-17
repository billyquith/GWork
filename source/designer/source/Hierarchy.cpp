#include "Hierarchy.h"
#include "DocumentCanvas.h"
#include <Gwork/Util/ControlFactory.h>

GWK_CONTROL_CONSTRUCTOR( Hierarchy )
{
	SetSize( 200, 200 );

	m_tree = new Controls::TreeControl( this );
	m_tree->Dock( Docking::Fill );
}

void Hierarchy::WatchCanvas( DocumentCanvas* canvas )
{
	m_canvas = canvas;
	m_canvas->onChildAdded.Add( this, &ThisClass::OnCanvasChildAdded );
	m_canvas->onSelectionChanged.Add( this, &ThisClass::OnCanvasSelectionChanged );

	CompleteRefresh();
}

void Hierarchy::CompleteRefresh()
{
	m_tree->Clear();

	UpdateNode( m_tree, m_canvas );

	m_tree->ExpandAll();
}

void Hierarchy::UpdateNode( Controls::TreeNode* node, Controls::Base* control )
{	
	Controls::TreeNode* childNode = nullptr;

	if ( !control->UserData.Exists( "ControlFactory" ) )
	{
		childNode = node;
	}
	else
	{
		Gwk::String strName = control->GetName();
		if ( strName == "" ) strName = "[" + Gwk::String( control->GetTypeName() ) + "]";

		childNode = node->AddNode( strName );
		childNode->SetImage( "img/controls/" + Gwk::String(control->GetTypeName()) + ".png" );
		childNode->onSelect.Add( this, &ThisClass::OnNodeSelected );
		childNode->UserData.Set<Controls::Base*>( "TargetControl", control );
	}


	for ( int i=0; i<control->NumChildren(); i++ )
	{
		UpdateNode( childNode, control->GetChild( i ) );
	}
}

void Hierarchy::OnCanvasChildAdded( Event::Info info )
{
	//info.Control;
	CompleteRefresh();
}

void Hierarchy::OnNodeSelected( Event::Info info )
{
	if ( !info.ControlCaller->UserData.Exists( "TargetControl" ) ) return;

	Controls::Base* ctrl = info.ControlCaller->UserData.Get<Controls::Base*>( "TargetControl" );

	ControlList list;
	list.Add( ctrl );

	m_canvas->SelectControls( list );
}

void Hierarchy::OnCanvasSelectionChanged( Event::Info info )
{
	m_tree->DeselectAll();

	for ( ControlList::List::const_iterator it = info.ControlList.list.cbegin();
		  it != info.ControlList.list.cend(); ++it )
	{
		SelectNodeRepresentingControl( (*it), m_tree );
	}
}

void Hierarchy::SelectNodeRepresentingControl( Controls::Base* control, Controls::TreeNode* node )
{
	if ( node == nullptr ) node = m_tree;

	if ( node->UserData.Exists( "TargetControl" ) &&
		 node->UserData.Get<Controls::Base*>( "TargetControl" ) == control )
	{
		node->SetSelected( true, false );
	}
	
	Base::List& children = node->GetChildNodes();
	for ( Base::List::iterator iter = children.begin(); iter != children.end(); ++iter )
	{
		Controls::TreeNode* childNode = gwk_cast<Controls::TreeNode>( *iter );
		if ( !childNode ) continue;

		SelectNodeRepresentingControl( control, childNode );
	}
}
