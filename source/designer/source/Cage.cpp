
#include "Cage.h"

using namespace Gwk;

GWK_CONTROL_CONSTRUCTOR( Cage )
{
	m_control = nullptr;
	m_border = 5;
}

void Cage::Render( Gwk::Skin::Base* skin )
{
	Gwk::Rect bounds = GetRenderBounds();

	bounds.x += m_border;
	bounds.y += m_border;
	bounds.w -= m_border * 2;
	bounds.h -= m_border * 2;

	skin->GetRender()->SetDrawColor( Color( 130, 30, 30, 10 ) );
	skin->GetRender()->DrawFilledRect( bounds );

	skin->GetRender()->SetDrawColor( Color( 20, 150, 255, 255 ) );
	skin->GetRender()->DrawLinedRect( bounds );
	bounds.x += 1; bounds.y += 1; bounds.w -= 2; bounds.h -= 2; 
	skin->GetRender()->DrawLinedRect( bounds );
}

void Cage::PostLayout( Skin::Base* skin )
{
	if ( !m_control ) return;

	Gwk::Point canvaspos = m_control->LocalPosToCanvas();
	Gwk::Point parentpos = GetParent()->CanvasPosToLocal( canvaspos );

	parentpos.x -= m_border;
	parentpos.y -= m_border;

	SetPos( parentpos );

	Gwk::Point size = m_control->GetSize();
	size.x += m_border * 2;
	size.y += m_border * 2;

	SetSize( size );
}

void Cage::Setup( Controls::Base* control )
{
	m_control = control;
}

void Cage::SetDepressed( bool b )
{
	ParentClass::SetDepressed( b );

	m_dragPoint = m_control->GetPos();
	m_bDragged = false;
}

void Cage::OnMouseMoved( int x, int y, int deltaX, int deltaY )
{
	if ( !IsDepressed() ) return;

	Controls::Base*	controlParent = m_control->GetParent();
	Gwk::Point pntRemainder = m_dragPoint - m_control->GetPos();

	//
	// This event is used by the SelectionLayer to scan
	// and change the parent - if appropriate.
	//
	{
		Event::Information info;

		info.Point = Gwk::Point( x, y );
		onMoving.Call( this, info );
	}

	//
	// If the parent changed then fix up the drag position
	//
	if ( controlParent != m_control->GetParent() )
	{
		m_dragPoint = m_control->GetPos() + pntRemainder;

	}

	if ( !m_bDragged )
	{
		Event::Information info;
		onDragStart.Call( this, info );
	}
	m_bDragged = true;

	m_dragPoint += Gwk::Point( deltaX, deltaY );

	Gwk::Point pos = m_dragPoint;

	pos.x = static_cast<int>(pos.x / 10.0f) * 10;
	pos.y = static_cast<int>(pos.y / 10.0f) * 10;

	pos -= m_control->GetPos();

	{
		Event::Information info;
		info.Point = pos;
		onMoved.Call( this, info );
	}
}

void Cage::OnPress()
{
	if ( IsToggle() )
	{
		SetToggleState( !GetToggleState() );
	}

	if ( !m_bDragged )
		onPress.Call( this );
}
