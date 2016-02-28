

inline Gwk::Controls::Base* FindParentControlFactoryControl( Gwk::Controls::Base* pControl )
{
	while ( pControl )
	{
		if ( pControl->UserData.Exists( "ControlFactory" ) )
		{
			return pControl;
		}

		pControl = pControl->GetParent();
	}

	return NULL;
}
