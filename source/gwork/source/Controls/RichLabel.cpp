/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Controls/RichLabel.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Utility.h>

using namespace Gwk;
using namespace Gwk::Controls;

const unsigned char Type_Text = 0;
const unsigned char Type_Newline = 1;

GWK_CONTROL_CONSTRUCTOR(RichLabel)
{
    m_bNeedsRebuild = false;
}

void RichLabel::AddLineBreak()
{
    DividedText t;
    t.type = Type_Newline;
    m_textBlocks.push_back(t);
}

void RichLabel::AddText(const Gwk::String& text, Gwk::Color color, Gwk::Font* font)
{
    if (text.length() == 0)
        return;

    Gwk::Utility::Strings::List lst;
    Gwk::Utility::Strings::Split(text, "\n", lst, false);

    for (size_t i = 0; i < lst.size(); i++)
    {
        if (i > 0)
            AddLineBreak();

        DividedText t;
        t.type = Type_Text;
        t.text = lst[i];
        t.color = color;
        t.font = font;
        m_textBlocks.push_back(t);
        m_bNeedsRebuild = true;
        Invalidate();
    }
}

bool RichLabel::SizeToChildren(bool w, bool h)
{
    Rebuild();
    return ParentClass::SizeToChildren(w, h);
}

void RichLabel::SplitLabel(const Gwk::String& text, Gwk::Font* font,
                           const DividedText& txt, int& x, int& y, int& lineheight)
{
    Gwk::Utility::Strings::List lst;
    Gwk::Utility::Strings::Split(text, " ", lst, true);

    if (lst.size() == 0)
        return;

    int iSpaceLeft = Width()-x;
    // Does the whole word fit in?
    {
        Gwk::Point StringSize = GetSkin()->GetRender()->MeasureText(font, text);

        if (iSpaceLeft > StringSize.x)
            return CreateLabel(text, txt, x, y, lineheight, true);
    }
    // If the first word is bigger than the line, just give up.
    {
        Gwk::Point WordSize = GetSkin()->GetRender()->MeasureText(font, lst[0]);

        if (WordSize.x >= iSpaceLeft)
        {
            CreateLabel(lst[0], txt, x, y, lineheight, true);

            if (lst[0].size() >= text.size())
                return;

            Gwk::String LeftOver = text.substr(lst[0].size()+1);
            return SplitLabel(LeftOver, font, txt, x, y, lineheight);
        }
    }
    Gwk::String strNewString = "";

    for (size_t i = 0; i < lst.size(); i++)
    {
        Gwk::Point WordSize = GetSkin()->GetRender()->MeasureText(font, strNewString+lst[i]);

        if (WordSize.x > iSpaceLeft)
        {
            CreateLabel(strNewString, txt, x, y, lineheight, true);
            x = 0;
            y += lineheight;
            break;
        }

        strNewString += lst[i];
    }

    Gwk::String LeftOver = text.substr(strNewString.size()+1);
    return SplitLabel(LeftOver, font, txt, x, y, lineheight);
}

void RichLabel::CreateLabel(const Gwk::String& text, const DividedText& txt, int& x, int& y,
                            int& lineheight, bool NoSplit)
{
    //
    // Use default font or is one set?
    //
    Gwk::Font* font = GetSkin()->GetDefaultFont();

    if (txt.font)
        font = txt.font;

    //
    // This string is too long for us, split it up.
    //
    Gwk::Point p = GetSkin()->GetRender()->MeasureText(font, text);

    if (lineheight == -1)
        lineheight = p.y;

    if (!NoSplit)
    {
        if (x+p.x > Width())
            return SplitLabel(text, font, txt, x, y, lineheight);
    }

    //
    // Wrap
    //
    if (x+p.x >= Width())
        CreateNewline(x, y, lineheight);

    Gwk::Controls::Label*  label = new Gwk::Controls::Label(this);
    label->SetText(x == 0 ? Gwk::Utility::Strings::TrimLeft<Gwk::String>(text,
                                                                                   " ") : text);
    label->SetTextColor(txt.color);
    label->SetFont(font);
    label->SizeToContents();
    label->SetPos(x, y);
    // lineheight = (lineheight + label->Height()) / 2;
    x += label->Width();

    if (x >= Width())
        CreateNewline(x, y, lineheight);
}

void RichLabel::CreateNewline(int& x, int& y, int& lineheight)
{
    x = 0;
    y += lineheight;
}

void RichLabel::Rebuild()
{
    RemoveAllChildren();
    int x = 0;
    int y = 0;
    int lineheight = -1;

    for (auto&& text : m_textBlocks)
    {
        if (text.type == Type_Newline)
        {
            CreateNewline(x, y, lineheight);
            continue;
        }

        if (text.type == Type_Text)
        {
            CreateLabel(text.text, text, x, y, lineheight, false);
            continue;
        }
    }

    m_bNeedsRebuild = false;
}

void RichLabel::OnBoundsChanged(Gwk::Rect oldBounds)
{
    ParentClass::OnBoundsChanged(oldBounds);
    Rebuild();
}

void RichLabel::Layout(Gwk::Skin::Base* skin)
{
    ParentClass::Layout(skin);

    if (m_bNeedsRebuild)
        Rebuild();
}
