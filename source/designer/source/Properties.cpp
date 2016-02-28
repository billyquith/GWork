#include "Properties.h"
#include "DocumentCanvas.h"
#include "Gwork/Util/ControlFactory.h"

GWK_CONTROL_CONSTRUCTOR( Properties )
{
	SetSize( 200, 200 );

	m_Props = new Controls::PropertyTree( this );
	m_Props->Dock( Docking::Fill );


}

void Properties::WatchCanvas( DocumentCanvas* pCanvas )
{
	m_pCanvas = pCanvas;
	m_pCanvas->onSelectionChanged.Add( this, &ThisClass::OnCanvasSelectionChanged );
	m_pCanvas->onPropertiesChanged.Add( this, &ThisClass::OnPropertiesChanged );

	
	//m_pCanvas->onChildAdded.Add( this, &ThisClass::OnCanvasChildAdded );

}


void Properties::OnCanvasSelectionChanged( Event::Info info )
{
	m_Props->Clear();

	m_SelectedControls = info.ControlList;

	for ( ControlList::List::const_iterator it = m_SelectedControls.list.begin(); it != m_SelectedControls.list.end(); ++it )
	{
		AddPropertiesFromControl( *it, true );
	}

	m_Props->ExpandAll();
}

void Properties::AddPropertiesFromControl( Controls::Base* pControl, bool bAllowDifferent )
{
	if ( !pControl->UserData.Exists( "ControlFactory" ) )
		return;

	ControlFactory::Base* cf = pControl->UserData.Get<ControlFactory::Base*>( "ControlFactory" );

	//
	// Foreach control control factory on this control
	//
	while ( cf )
	{
		Controls::Properties* properties = m_Props->Find( cf->Name() );

		if ( !properties ) properties = m_Props->Add( cf->Name() );

		//
		// And foreach property in that control factory
		//
		ControlFactory::Property::List::const_iterator it = cf->Properties().begin();
		ControlFactory::Property::List::const_iterator itEnd = cf->Properties().end();
		for ( ; it != itEnd; ++it )
		{
			Gwk::String strPropertyName = (*it)->Name();
			Gwk::String strValue = (*it)->GetValueAsString(pControl);

			//
			// Add a property row to our control
			//
			Controls::PropertyRow* row = properties->Find( strPropertyName );
			if ( !row )
			{
				if ( (*it)->OptionCount() > 0 )
				{
					//row = properties->Add( strPropertyName, new Gwk::Controls::Property::Checkbox( properties ), strValue );

					Gwk::Controls::Property::ComboBox* dd = new Gwk::Controls::Property::ComboBox( properties );

					for (size_t i = 0, nb = (*it)->OptionCount(); i < nb; i++)
					{
						dd->GetComboBox()->AddItem( (*it)->OptionGet(i), (*it)->OptionGet(i) );
					}

					row = properties->Add( strPropertyName, dd, strValue );
				}
				else
				{	
					row = properties->Add( strPropertyName, strValue );
				}

				row->SetName( (*it)->Name() );
				row->onChange.Add( this, &ThisClass::OnPropertyChanged );
			}
			else
			{
				if ( bAllowDifferent && row->GetProperty()->GetPropertyValue() != strValue )
				{
					row->GetProperty()->SetPropertyValue( "different" );
				}
				else 
				{
					row->GetProperty()->SetPropertyValue( strValue, false );
				}
			}
		}

		cf = cf->GetBaseFactory();
	}
}

void Properties::OnPropertyChanged( Event::Info info )
{
	for ( ControlList::List::const_iterator it = m_SelectedControls.list.begin(); it != m_SelectedControls.list.end(); ++it )
	{
		Controls::Base* pControl = (*it);
		ControlFactory::Base* cf = pControl->UserData.Get<ControlFactory::Base*>( "ControlFactory" );

		while ( cf )
		{
			cf->SetControlValue( pControl, info.ControlCaller->GetName(), info.String );
			cf = cf->GetBaseFactory();
		}
	}
}

void Properties::OnPropertiesChanged( Event::Info info )
{
	m_SelectedControls = info.ControlList;
	
	for ( ControlList::List::const_iterator it = m_SelectedControls.list.begin(); it != m_SelectedControls.list.end(); ++it )
	{
		AddPropertiesFromControl( *it, it != m_SelectedControls.list.begin() );
	}

	m_Props->ExpandAll();
}
