
#pragma once

#include "Gwork/Gwork.h"
#include "Gwork/Controls.h"
#include "Gwork/Util/ImportExport.h"

using namespace Gwk;

class Document;

/*

	The base panel

*/

class DesignerBase : public Controls::Base 
{
	GWK_CONTROL( DesignerBase, Controls::Base );

	protected:

		void CreateMenu();
		void CreateToolBar();
		void CreateDocumentHolder();
		void CreateControlToolbox();

		void NewDocument();
		void CloseDocument();
		void OpenDocument();
		void DoOpenDocument( Event::Info info );
		void SaveDocument();
		void SaveAsDocument();

		void DeleteSelected();
		void SendBack();
		void BringForward();

		Document* CurrentDocument();


		Controls::TabControl*			m_DocumentHolder;
};
