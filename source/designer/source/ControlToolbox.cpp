
#include "ControlToolbox.h"
#include <Gwork/Util/ControlFactory.h>

GWK_CONTROL_CONSTRUCTOR( ControlToolbox )
{
	SetWidth( 150 );
	SetMargin( Margin( 5, 5, 5, 5 ) );
	SetText( "Controls" );

	Controls::Layout::Tile* tileLayout = new Controls::Layout::Tile( this );
	tileLayout->SetTileSize( 22, 22 );

	for (ControlFactory::List::iterator it = ControlFactory::GetList().begin();
		 it != ControlFactory::GetList().end();
		 ++it)
	{
		ControlFactory::Base* controlFactory = *it;

		if ( controlFactory->Name() == "DesignerCanvas" )
			continue;

		Controls::Button* button = new Controls::Button( tileLayout );
		button->SetSize( 20, 20 );
		button->SetTooltip( controlFactory->Name() );
		button->SetImage( "img/controls/" + controlFactory->Name() + ".png" );
		button->SetShouldDrawBackground( false );

		// Make drag and droppable. Pass the ControlFactory as the userdata
		button->DragAndDrop_SetPackage( true, "ControlSpawn", controlFactory );
	}

}
