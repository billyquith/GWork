

inline Gwk::Controls::Base* FindParentControlFactoryControl( Gwk::Controls::Base* control )
{
	while ( control )
	{
		if ( control->UserData.Exists( "ControlFactory" ) )
		{
			return control;
		}

		control = control->GetParent();
	}

	return nullptr;
}
