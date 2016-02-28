
#pragma once

#include "Gwork/Gwork.h"
#include "Gwork/Controls.h"


class Cage : public Gwk::Controls::Button
{
	GWK_CONTROL( Cage, Gwk::Controls::Button );
	
	void Setup( Gwk::Controls::Base* pControl );
	
	virtual void Render( Gwk::Skin::Base* skin );
	virtual void PostLayout( Gwk::Skin::Base* skin );
	
	virtual void OnMouseMoved( int x, int y, int deltaX, int deltaY );
	virtual void SetDepressed( bool b );
	
	virtual void OnPress();
	
	virtual Gwk::Controls::Base* Target() { return m_Control; }
	
	Gwk::Event::Caller		onMoved;
	Gwk::Event::Caller		onDragStart;
	Gwk::Event::Caller		onMoving;
	
protected:
	
	bool					m_bDragged;
	Gwk::Controls::Base*	m_Control;
	int						m_iBorder;
	
	Gwk::Point				m_DragPoint;
};
