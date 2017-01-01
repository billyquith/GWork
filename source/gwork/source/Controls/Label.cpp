/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Utility.h>

using namespace Gwk;
using namespace Gwk::Controls;

GWK_CONTROL_CONSTRUCTOR(Label)
{
    m_createdFont = nullptr;
    m_text = new ControlsInternal::Text(this);
    m_text->SetFont(GetSkin()->GetDefaultFont());
    SetMouseInputEnabled(false);
    SetBounds(0, 0, 100, 10);
    SetAlignment(Position::Left | Position::Top);
}

void Label::PreDelete(Gwk::Skin::Base* skin)
{
    if (m_createdFont)
    {
        skin->ReleaseFont(m_createdFont);
        delete m_createdFont;
        m_createdFont = nullptr;
        SetFont(nullptr);
    }
}

void Label::PostLayout(Skin::Base* /*skin*/)
{
    m_text->SetPosition(m_align);
}

void Label::SetAlignment(Position iAlign)
{
    if (m_align == iAlign)
        return;

    m_align = iAlign;
    Invalidate();
}

Position Label::GetAlignment()
{
    return m_align;
}

void Label::SetText(const String& str, bool bDoEvents)
{
    if (m_text->GetText() == str)
        return;

    m_text->SetString(str);
    Redraw();

    if (bDoEvents)
        OnTextChanged();
}

void Label::SizeToContents()
{
    m_text->SetPos(m_padding.left, m_padding.top);
    m_text->RefreshSize();
    SetSize(m_text->Width()+m_padding.left+m_padding.right,
            m_text->Height()+m_padding.top+m_padding.bottom);
}

Gwk::Rect Label::GetCharacterPosition(int iChar)
{
    Gwk::Rect p = m_text->GetCharacterPosition(iChar);
    p.x += m_text->X();
    p.y += m_text->Y();
    return p;
}

void Label::OnBoundsChanged(Gwk::Rect oldChildBounds)
{
    ParentClass::OnBoundsChanged(oldChildBounds);

    if (m_text->Wrap())
    {
        m_text->RefreshSize();
        Invalidate();
    }
}

void Label::SetFont(Gwk::String strFacename, int iSize, bool bBold)
{
    if (m_createdFont)
    {
        GetSkin()->ReleaseFont(m_createdFont);
        delete m_createdFont;
        m_createdFont = nullptr;
        SetFont(nullptr);
    }

    m_createdFont = new Gwk::Font();
    Debug::AssertCheck(m_createdFont != nullptr, "Couldn't Create Font!");
    m_createdFont->bold = bBold;
    m_createdFont->facename = strFacename;
    m_createdFont->size = iSize;
    SetFont(m_createdFont);
    m_text->RefreshSize();
}
