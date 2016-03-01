#include "DesignerBase.h"
#include "Document.h"
#include "ControlToolbox.h"
#include <Gwork/Util/ImportExport.h>

GWK_CONTROL_CONSTRUCTOR( DesignerBase )
{
	Dock( Docking::Fill );

	CreateMenu();
	CreateToolBar();
	CreateControlToolbox();
	CreateDocumentHolder();

	new Controls::StatusBar( this, "StatusBar" );

	NewDocument();
}


void DesignerBase::CreateMenu()
{
	Controls::MenuStrip* strip = new Controls::MenuStrip( this );
	strip->Dock( Docking::Top );

	// File
	{
		Gwk::Controls::MenuItem* root = strip->AddItem( "File" );

		root->GetMenu()->AddItem( "New", "img/menu/new.png", "Ctrl + N" )->SetAction( this, &ThisClass::NewDocument );

		root->GetMenu()->AddItem( "Open", "img/menu/open.png", "Ctrl + O" )->SetAction( this, &ThisClass::OpenDocument );
		root->GetMenu()->AddItem( "Save", "img/menu/save.png", "Ctrl + S" )->SetAction( this, &ThisClass::SaveDocument );
		root->GetMenu()->AddItem( "Save As", "img/menu/save.png", "Ctrl + Shift + S" )->SetAction( this, &ThisClass::SaveAsDocument );

		root->GetMenu()->AddItem( "Close", "img/menu/close.png" )->SetAction( this, &ThisClass::CloseDocument );
	}
}

void DesignerBase::CreateToolBar()
{
	Controls::ToolBarStrip* strip = new Controls::ToolBarStrip( this );
	strip->Dock( Docking::Top );

	strip->Add( "New Document", "img/menu/new.png" )->onPress.Add( this, &ThisClass::NewDocument );

	strip->Add( "Open", "img/menu/open.png" )->onPress.Add( this, &ThisClass::OpenDocument );
	strip->Add( "Save", "img/menu/save.png" )->onPress.Add( this, &ThisClass::SaveDocument );

	// splitter

	strip->Add( "Delete", "img/menu/delete.png" )->onPress.Add( this, &ThisClass::DeleteSelected );
	strip->Add( "Send Back", "img/menu/back.png" )->onPress.Add( this, &ThisClass::SendBack );
	strip->Add( "Bring Forward", "img/menu/forward.png" )->onPress.Add( this, &ThisClass::BringForward );
}

void DesignerBase::CreateControlToolbox()
{
	ControlToolbox* controlBox = new ControlToolbox( this );
	controlBox->Dock( Docking::Left );
}

void DesignerBase::CreateDocumentHolder()
{
	m_documentHolder = new Controls::TabControl( this );
	m_documentHolder->Dock( Docking::Fill );
	m_documentHolder->SetMargin( Margin( -1, 2, -1, -1 ) );
}

void DesignerBase::NewDocument()
{
	Controls::TabButton* button = m_documentHolder->AddPage( "Untitled Design" );
	button->SetImage( "img/document_normal.png" );

	Document* doc = new Document( button->GetPage(), "Document" );
	doc->Initialize( button );

	button->OnPress();
}

void DesignerBase::CloseDocument()
{
	Document* doc = CurrentDocument();
	if ( !doc ) return;

	doc->DelayedDelete();

	Controls::TabButton* button = m_documentHolder->GetCurrentButton();
	if ( !button ) return;

	m_documentHolder->RemovePage( button );
	button->DelayedDelete();
}

void DesignerBase::OpenDocument()
{
	Gwk::Dialogs::FileOpen( true, "", "", "Gwork Designer File|*.gwk", this, &ThisClass::DoOpenDocument );
}

void DesignerBase::DoOpenDocument( Event::Info info )
{
	Controls::TabButton* button = m_documentHolder->AddPage( "Loaded" );
	button->SetImage( "img/document_normal.png" );

	Document* doc = new Document( button->GetPage(), "Document" );
	doc->Initialize( button );
	doc->LoadFromFile( info.String, ImportExport::Find( "Designer" ) );

	button->OnPress();
}

void DesignerBase::SaveDocument()
{
	Document* doc = CurrentDocument();
	if ( !doc ) return;

	doc->DoSave( ImportExport::Find( "Designer" ) );
}

void DesignerBase::SaveAsDocument()
{
	Document* doc = CurrentDocument();
	if ( !doc ) return;

	doc->DoSaveAs( ImportExport::Find( "Designer" ) );
}

Document* DesignerBase::CurrentDocument()
{
	Controls::TabButton* button = m_documentHolder->GetCurrentButton();
	if ( !button ) return NULL;

	Document* doc = gwk_cast<Document>(button->GetPage()->FindChildByName( "Document" ));
	if ( !doc ) return NULL;

	return doc;
}

void DesignerBase::DeleteSelected()
{
	Document* doc = CurrentDocument();
	if ( !doc ) return;

	doc->Command( "delete" );
}

void DesignerBase::SendBack()
{
	Document* doc = CurrentDocument();
	if ( !doc ) return;

	doc->Command( "sendback" );
}

void DesignerBase::BringForward()
{
	Document* doc = CurrentDocument();
	if ( !doc ) return;

	doc->Command( "bringforward" );
}
