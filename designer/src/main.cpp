
#include "Gwen/Gwen.h"
#include "Gwen/Controls.h"
#include "Gwen/Skins/TexturedBase.h"
#include "DesignerBase.h"
#ifdef WIN32
	#include "Gwen/Renderers/DirectX9.h"
#else
	#include "Gwen/Renderers/Allegro.h"
#endif

int main(int, char **)
{
#ifdef WIN32
	Gwen::Renderer::DirectX9	renderer;
#else
	Gwen::Renderer::Allegro		renderer;
#endif
	Gwen::Skin::TexturedBase	skin( &renderer );
	skin.SetRender( &renderer );

	Gwen::Controls::WindowCanvas window_canvas( -1, -1, 1024, 600, &skin, "GWEN Designer" );
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
