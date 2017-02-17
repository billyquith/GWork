/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/Rectangle.h>

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(Rectangle)
{
    m_color = Gwk::Color(255, 255, 255, 255);
}

void Rectangle::Render(Skin::Base* skin)
{
    skin->GetRender()->SetDrawColor(m_color);
    skin->GetRender()->DrawFilledRect(GetRenderBounds());
}
