
#pragma once

#include <Gwork/Gwork.h>
#include <Gwork/Controls.h>

using namespace Gwk;

class SelectionLayer : public Controls::Base 
{
	GWK_CONTROL( SelectionLayer, Controls::Base );

	virtual void OnMouseClickLeft( int x, int y, bool bDown );

	virtual void ClearSelection();
	virtual void AddSelection( Controls::Base* control );
	virtual void RemoveSelection(  Controls::Base* control );

	ControlList& GetSelected(){ return m_selected; }

	Event::Caller onSelectionChanged;
	Event::Caller onPropertiesChanged;
	Event::Caller onHierachyChanged;

	protected:

		void OnCageMoving( Event::Info info );
		void OnControlDragged( Event::Info info );
		void OnCagePressed( Event::Info info );
		void OnDragStart();

		void SwitchCage( Controls::Base* control, Controls::Base* to );

		ControlList	m_selected;

};
