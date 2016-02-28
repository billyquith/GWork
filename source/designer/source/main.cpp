
#include <Gwork/Gwork.h>
#include <Gwork/Controls.h>
#include <Gwork/Skins/TexturedBase.h>
#include "DesignerBase.h"
#ifdef WIN32
#	include "Gwork/Renderers/DirectX9.h"
#else
#	include "Gwork/Renderers/Allegro.h"
#endif

int main(int, char **)
{
#ifdef WIN32
	Gwk::Renderer::DirectX9	renderer;
#else
	Gwk::Renderer::Allegro		renderer;
#endif
	
	Gwk::Skin::TexturedBase skin( &renderer );
	skin.SetRender( &renderer );
	
	Gwk::Controls::WindowCanvas window_canvas( -1, -1, 1024, 600, &skin, "Gwork Designer" );
	skin.Init( "img/DesignerSkin.png" );

	skin.SetDefaultFont( "OpenSans.ttf", 11 );

	DesignerBase* pDesignerBase = new DesignerBase( &window_canvas );

	while ( !window_canvas.WantsQuit() )
	{
		window_canvas.DoThink();
	}
	
	delete pDesignerBase;
	
	return 0;
}
