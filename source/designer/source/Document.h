
#pragma once

#include <Gwork/Gwork.h>
#include <Gwork/Controls.h>
#include <Gwork/Util/ImportExport.h>

using namespace Gwk;

class DocumentCanvas;
class Hierarchy;


class Document : public Controls::Base
{
	GWK_CONTROL( Document, Controls::Base );

	virtual void Initialize( Controls::TabButton* tab );

	virtual void DoSaveAs( ImportExport::Base* exporter );
	virtual void DoSave( ImportExport::Base* exporter );
	virtual void LoadFromFile( const Gwk::String& str, ImportExport::Base* exporter );
	
	virtual void Command( const Gwk::String& str );

protected:

	void DoSaveFromDialog( Event::Info info );
	void OnHierachyChanged( Event::Info info );

	Controls::TabButton*	m_tab;
	DocumentCanvas*			m_canvas;
	Hierarchy*				m_hierarchy;

	Gwk::String			m_strFilename;

	ImportExport::Base*		m_exporter;
	ImportExport::Base*		m_importer;
};
