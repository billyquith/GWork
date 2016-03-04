
#pragma once

#include <Gwork/Gwork.h>
#include <Gwork/Controls.h>

using namespace Gwk;

class DocumentCanvas;

class Hierarchy : public Controls::Base 
{
	GWK_CONTROL( Hierarchy, Controls::Base );

		void WatchCanvas( DocumentCanvas* canvas );

		void SelectNodeRepresentingControl( Controls::Base* control, Controls::TreeNode* node = nullptr );

		void CompleteRefresh();

	protected:

		
		void UpdateNode( Controls::TreeNode* node, Controls::Base* controls );

		void OnCanvasChildAdded( Event::Info info );
		void OnCanvasSelectionChanged( Event::Info info );

		void OnNodeSelected( Event::Info info );

		DocumentCanvas*			m_canvas;
		Controls::TreeControl*	m_tree;

};