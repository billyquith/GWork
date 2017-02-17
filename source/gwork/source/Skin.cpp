/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>

namespace Gwk
{
namespace Skin
{
/*
 *
 *  Here we're drawing a few symbols such as the directional arrows and
 * the checkbox check
 *
 *  Texture'd skins don't generally use these - but the Simple skin
 * does. We did originally
 *  use the marlett font to draw these.. but since that's a Windows font
 * it wasn't a very
 *  good cross platform solution.
 *
 */

void Base::DrawArrowDown(Gwk::Rect rect)
{
    const float x = (rect.w/5.0f);
    const float y = (rect.h/5.0f);
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*0.0f, rect.y+y*1.0f, x, y*1.0f));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*1.0f, rect.y+y*1.0f, x, y*2.0f));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*2.0f, rect.y+y*1.0f, x, y*3.0f));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*3.0f, rect.y+y*1.0f, x, y*2.0f));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*4.0f, rect.y+y*1.0f, x, y*1.0f));
}

void Base::DrawArrowUp(Gwk::Rect rect)
{
    const float x = (rect.w/5.0f);
    const float y = (rect.h/5.0f);
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*0.0f, rect.y+y*3.0f, x, y*1.0f));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*1.0f, rect.y+y*2.0f, x, y*2.0f));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*2.0f, rect.y+y*1.0f, x, y*3.0f));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*3.0f, rect.y+y*2.0f, x, y*2.0f));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*4.0f, rect.y+y*3.0f, x, y*1.0f));
}

void Base::DrawArrowLeft(Gwk::Rect rect)
{
    const float x = (rect.w/5.0f);
    const float y = (rect.h/5.0f);
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*3.0f, rect.y+y*0.0f, x*1.0f, y));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*2.0f, rect.y+y*1.0f, x*2.0f, y));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*1.0f, rect.y+y*2.0f, x*3.0f, y));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*2.0f, rect.y+y*3.0f, x*2.0f, y));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*3.0f, rect.y+y*4.0f, x*1.0f, y));
}

void Base::DrawArrowRight(Gwk::Rect rect)
{
    const float x = (rect.w/5.0f);
    const float y = (rect.h/5.0f);
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*1.0f, rect.y+y*0.0f, x*1.0f, y));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*1.0f, rect.y+y*1.0f, x*2.0f, y));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*1.0f, rect.y+y*2.0f, x*3.0f, y));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*1.0f, rect.y+y*3.0f, x*2.0f, y));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*1.0f, rect.y+y*4.0f, x*1.0f, y));
}

void Base::DrawCheck(Gwk::Rect rect)
{
    const float x = (rect.w/5.0f);
    const float y = (rect.h/5.0f);
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*0.0f, rect.y+y*3.0f, x*2, y*2));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*1.0f, rect.y+y*4.0f, x*2, y*2));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*2.0f, rect.y+y*3.0f, x*2, y*2));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*3.0f, rect.y+y*1.0f, x*2, y*2));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+x*4.0f, rect.y+y*0.0f, x*2, y*2));
}

void Base::DrawTreeNode(Controls::Base* ctrl, bool bOpen, bool bSelected, int iLabelHeight,
                        int iLabelWidth, int iHalfWay, int iLastBranch, bool bIsRoot)
{
    GetRender()->SetDrawColor(Colors.Tree.Lines);

    if (!bIsRoot)
        GetRender()->DrawFilledRect(Gwk::Rect(8, iHalfWay, 16-9, 1));

    if (!bOpen)
        return;

    GetRender()->DrawFilledRect(Gwk::Rect(14+7, iLabelHeight+1, 1, iLastBranch+iHalfWay-
                                           iLabelHeight));
}

void Base::DrawPropertyTreeNode(Controls::Base* control, int BorderLeft, int BorderTop)
{
    const Gwk::Rect& rect = control->GetRenderBounds();
    m_render->SetDrawColor(Colors.Properties.Border);
    m_render->DrawFilledRect(Gwk::Rect(rect.x, rect.y, BorderLeft, rect.h));
    m_render->DrawFilledRect(Gwk::Rect(rect.x+BorderLeft, rect.y, rect.w-BorderLeft,
                                        BorderTop));
}

void Base::DrawPropertyRow(Controls::Base* control, int iWidth, bool bBeingEdited,
                           bool bHovered)
{
    Gwk::Rect rect = control->GetRenderBounds();

    if (bBeingEdited)
        m_render->SetDrawColor(Colors.Properties.Column_Selected);
    else if (bHovered)
        m_render->SetDrawColor(Colors.Properties.Column_Hover);
    else
        m_render->SetDrawColor(Colors.Properties.Column_Normal);

    m_render->DrawFilledRect(Gwk::Rect(0, rect.y, iWidth, rect.h));

    if (bBeingEdited)
        m_render->SetDrawColor(Colors.Properties.Line_Selected);
    else if (bHovered)
        m_render->SetDrawColor(Colors.Properties.Line_Hover);
    else
        m_render->SetDrawColor(Colors.Properties.Line_Normal);

    m_render->DrawFilledRect(Gwk::Rect(iWidth, rect.y, 1, rect.h));
    rect.y += rect.h-1;
    rect.h = 1;
    m_render->DrawFilledRect(rect);
}

} // namespace Skin
} // namespace Gwk
