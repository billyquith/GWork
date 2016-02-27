
#pragma once

#include "Gwen/Gwen.h"
#include "Gwen/Controls.h"


class Cage : public Gwen::Controls::Button
{
	GWEN_CONTROL( Cage, Gwen::Controls::Button );
	
	void Setup( Gwen::Controls::Base* pControl );
	
	virtual void Render( Gwen::Skin::Base* skin );
	virtual void PostLayout( Gwen::Skin::Base* skin );
	
	virtual void OnMouseMoved( int x, int y, int deltaX, int deltaY );
	virtual void SetDepressed( bool b );
	
	virtual void OnPress();
	
	virtual Gwen::Controls::Base* Target() { return m_Control; }
	
	Gwen::Event::Caller		onMoved;
	Gwen::Event::Caller		onDragStart;
	Gwen::Event::Caller		onMoving;
	
protected:
	
	bool					m_bDragged;
	Gwen::Controls::Base*	m_Control;
	int						m_iBorder;
	
	Gwen::Point				m_DragPoint;
};
