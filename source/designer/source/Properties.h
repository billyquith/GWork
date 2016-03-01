
#pragma once

#include <Gwork/Gwork.h>
#include <Gwork/Controls.h>

using namespace Gwk;

class DocumentCanvas;

class Properties : public Controls::Base 
{
	GWK_CONTROL( Properties, Controls::Base );

		void WatchCanvas( DocumentCanvas* canvas );

	protected:

		void OnCanvasSelectionChanged( Event::Info info );
		void OnPropertiesChanged( Event::Info info );

		void OnPropertyChanged( Event::Info info );

		void AddPropertiesFromControl( Controls::Base* control, bool bAllowDifferent );

		DocumentCanvas*				m_canvas;
		Controls::PropertyTree*		m_props;
		ControlList					m_selectedControls;


};
