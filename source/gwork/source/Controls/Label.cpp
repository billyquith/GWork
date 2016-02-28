/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Utility.h>

using namespace Gwk;
using namespace Gwk::Controls;

GWK_CONTROL_CONSTRUCTOR(Label)
{
    m_CreatedFont = NULL;
    m_Text = new ControlsInternal::Text(this);
    m_Text->SetFont(GetSkin()->GetDefaultFont());
    SetMouseInputEnabled(false);
    SetBounds(0, 0, 100, 10);
    SetAlignment(Gwk::Docking::Left|Gwk::Docking::Top);
}

void Label::PreDelete(Gwk::Skin::Base* skin)
{
    if (m_CreatedFont)
    {
        skin->ReleaseFont(m_CreatedFont);
        delete m_CreatedFont;
        m_CreatedFont = NULL;
        SetFont(NULL);
    }
}

void Label::PostLayout(Skin::Base* /*skin*/)
{
    m_Text->Position(m_iAlign);
}

void Label::SetAlignment(int iAlign)
{
    if (m_iAlign == iAlign)
        return;

    m_iAlign = iAlign;
    Invalidate();
}

int Label::GetAlignment()
{
    return m_iAlign;
}

void Label::SetText(const String& str, bool bDoEvents)
{
    if (m_Text->GetText() == str)
        return;

    m_Text->SetString(str);
    Redraw();

    if (bDoEvents)
        OnTextChanged();
}

void Label::SizeToContents()
{
    m_Text->SetPos(m_Padding.left, m_Padding.top);
    m_Text->RefreshSize();
    SetSize(m_Text->Width()+m_Padding.left+m_Padding.right,
            m_Text->Height()+m_Padding.top+m_Padding.bottom);
}

Gwk::Rect Label::GetCharacterPosition(int iChar)
{
    Gwk::Rect p = m_Text->GetCharacterPosition(iChar);
    p.x += m_Text->X();
    p.y += m_Text->Y();
    return p;
}

void Label::OnBoundsChanged(Gwk::Rect oldChildBounds)
{
    ParentClass::OnBoundsChanged(oldChildBounds);

    if (m_Text->Wrap())
    {
        m_Text->RefreshSize();
        Invalidate();
    }
}

void Label::SetFont(Gwk::String strFacename, int iSize, bool bBold)
{
    if (m_CreatedFont)
    {
        GetSkin()->ReleaseFont(m_CreatedFont);
        delete m_CreatedFont;
        m_CreatedFont = NULL;
        SetFont(NULL);
    }

    m_CreatedFont = new Gwk::Font();
    Debug::AssertCheck(m_CreatedFont != NULL, "Couldn't Create Font!");
    m_CreatedFont->bold = bBold;
    m_CreatedFont->facename = strFacename;
    m_CreatedFont->size = iSize;
    SetFont(m_CreatedFont);
    m_Text->RefreshSize();
}
