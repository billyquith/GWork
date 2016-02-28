/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include "Gwork/Controls/Rectangle.h"

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(Rectangle)
{
    m_Color = Gwk::Color(255, 255, 255, 255);
}

void Rectangle::Render(Skin::Base* skin)
{
    skin->GetRender()->SetDrawColor(m_Color);
    skin->GetRender()->DrawFilledRect(GetRenderBounds());
}
