#include "Document.h"
#include "DocumentCanvas.h"
#include "Hierarchy.h"
#include "Properties.h"

class DocumentInner : public Gwk::Controls::ScrollControl
{
	GWK_CONTROL_INLINE( DocumentInner, Gwk::Controls::ScrollControl )
	{
	}

	void Render( Gwk::Skin::Base* skin )
	{
		skin->GetRender()->SetDrawColor( Color( 255, 255, 255, 255 ) );
		skin->GetRender()->DrawFilledRect( GetRenderBounds() );
	}

	void RenderOver( Gwk::Skin::Base* skin )
	{
		skin->GetRender()->SetDrawColor( Color( 90, 90, 90, 255 ) );
		skin->GetRender()->DrawLinedRect( GetRenderBounds() );
	}
};

GWK_CONTROL_CONSTRUCTOR( Document )
{
	Dock( Docking::Fill );
	SetPadding( Padding( 1, 1, 1, 1 ) );

	// The main horizontal splitter separates the document from the tree/properties
	Controls::SplitterHorizontal* splitter = new Controls::SplitterHorizontal( this );
	splitter->Dock( Docking::Fill );
	splitter->SetScaling( true, 200 );

	// The white background
	DocumentInner* inner = new DocumentInner( this );
	inner->Dock( Docking::Fill );

	// The vertical splitter on the right containing the tree/properties
	Controls::SplitterVertical* rightSplitter = new Controls::SplitterVertical( this );
	rightSplitter->Dock( Docking::Fill );
	rightSplitter->SetSize( 200, 200 );
	rightSplitter->SetScaling( false, 200 );

	splitter->SetPanels( inner, rightSplitter );

	
	// The actual canvas onto which we drop controls
	{
		m_canvas = new DocumentCanvas( inner );
		m_canvas->Dock( Docking::Fill );
		m_canvas->onHierachyChanged.Add( this, &ThisClass::OnHierachyChanged );
	}


	// The controls on the right
	{
		m_hierarchy = new Hierarchy( rightSplitter );
		m_hierarchy->WatchCanvas( m_canvas );
		m_hierarchy->Dock( Docking::Fill );

		Properties* properties = new Properties( rightSplitter );
		properties->WatchCanvas( m_canvas );
		properties->Dock( Docking::Fill );

		rightSplitter->SetPanels( m_hierarchy, properties );
	}

}

void Document::Initialize( Controls::TabButton* tab )
{

}

void Document::DoSaveFromDialog( Event::Info info )
{
	if ( info.String == "" ) return;
	if ( !m_exporter ) return;

	m_strFilename = info.String;
	
	m_exporter->Export( m_canvas, m_strFilename );
	m_exporter = NULL;
}

void Document::DoSave( ImportExport::Base* exporter )
{
	// We don't have a previous filename.. do save as
	if ( m_strFilename == "" )
	{
		return DoSaveAs( exporter );
	}

	exporter->Export( m_canvas, m_strFilename );
}

void Document::DoSaveAs( ImportExport::Base* exporter )
{
	m_exporter = exporter;

	Gwk::Dialogs::FileSave( true, m_strFilename, "", "Gwork Designer File|*.gwk", this, &ThisClass::DoSaveFromDialog );
}

void Document::LoadFromFile( const Gwk::String& str, ImportExport::Base* exporter )
{
	exporter->Import( m_canvas, str );

	m_hierarchy->CompleteRefresh();
}

void Document::OnHierachyChanged( Event::Info info )
{
	m_hierarchy->CompleteRefresh();
}

void Document::Command( const Gwk::String& str )
{
	m_canvas->Command( str );

	m_hierarchy->CompleteRefresh();
}
