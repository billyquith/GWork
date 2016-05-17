#include "Properties.h"
#include "DocumentCanvas.h"
#include <Gwork/Util/ControlFactory.h>

GWK_CONTROL_CONSTRUCTOR( Properties )
{
	SetSize( 200, 200 );

	m_props = new Controls::PropertyTree( this );
	m_props->Dock( Docking::Fill );


}

void Properties::WatchCanvas( DocumentCanvas* canvas )
{
	m_canvas = canvas;
	m_canvas->onSelectionChanged.Add( this, &ThisClass::OnCanvasSelectionChanged );
	m_canvas->onPropertiesChanged.Add( this, &ThisClass::OnPropertiesChanged );

	//m_canvas->onChildAdded.Add( this, &ThisClass::OnCanvasChildAdded );
}


void Properties::OnCanvasSelectionChanged( Event::Info info )
{
	m_props->Clear();

	m_selectedControls = info.ControlList;

	for ( ControlList::List::const_iterator it = m_selectedControls.list.cbegin(); it != m_selectedControls.list.cend(); ++it )
	{
		AddPropertiesFromControl( *it, true );
	}

	m_props->ExpandAll();
}

void Properties::AddPropertiesFromControl( Controls::Base* control, bool bAllowDifferent )
{
	if ( !control->UserData.Exists( "ControlFactory" ) )
		return;

	ControlFactory::Base* cf = control->UserData.Get<ControlFactory::Base*>( "ControlFactory" );

	//
	// Foreach control control factory on this control
	//
	while ( cf )
	{
		Controls::Properties* properties = m_props->Find( cf->Name() );

		if ( !properties ) properties = m_props->Add( cf->Name() );

		//
		// And foreach property in that control factory
		//
		ControlFactory::Property::List::const_iterator it = cf->Properties().cbegin();
		ControlFactory::Property::List::const_iterator itEnd = cf->Properties().cend();
		for ( ; it != itEnd; ++it )
		{
			Gwk::String strPropertyName = (*it)->Name();
			Gwk::String strValue = (*it)->GetValueAsString(control);

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
	for ( ControlList::List::const_iterator it = m_selectedControls.list.cbegin(); it != m_selectedControls.list.cend(); ++it )
	{
		Controls::Base* control = (*it);
		ControlFactory::Base* cf = control->UserData.Get<ControlFactory::Base*>( "ControlFactory" );

		while ( cf )
		{
			cf->SetControlValue( control, info.ControlCaller->GetName(), info.String );
			cf = cf->GetBaseFactory();
		}
	}
}

void Properties::OnPropertiesChanged( Event::Info info )
{
	m_selectedControls = info.ControlList;
	
	for ( ControlList::List::const_iterator it = m_selectedControls.list.cbegin(); it != m_selectedControls.list.cend(); ++it )
	{
		AddPropertiesFromControl( *it, it != m_selectedControls.list.cbegin() );
	}

	m_props->ExpandAll();
}
