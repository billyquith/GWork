
#pragma once

#include <Gwork/Gwork.h>
#include <Gwork/Controls.h>

using namespace Gwk;

class SelectionLayer;

class DocumentCanvas : public Controls::Base 
{
	GWK_CONTROL( DocumentCanvas, Controls::Base );

	virtual void Render( Gwk::Skin::Base* skin );
	virtual void PostLayout( Skin::Base* skin );

	virtual void SelectControls( ControlList& list );

	virtual bool DragAndDrop_CanAcceptPackage( Gwk::DragAndDrop::Package* package );
	virtual bool DragAndDrop_HandleDrop( Gwk::DragAndDrop::Package* package, int x, int y );

	void Command( const Gwk::String& str );

	Event::Caller	onChildAdded;
	Event::Caller	onChildRemoved;
	Event::Caller	onSelectionChanged;
	Event::Caller	onPropertiesChanged;
	Event::Caller	onHierachyChanged;

	protected:

		void OnSelectionChanged( Event::Info info );
		void OnPropertiesChanged( Event::Info info );
		void OnHierachyChanged( Event::Info info );
		

		SelectionLayer*	m_selectionLayer;
};
