/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Controls/Text.h>
#include <Gwork/Skin.h>
#include <Gwork/Utility.h>

using namespace Gwk;
using namespace Gwk::ControlsInternal;

GWK_CONTROL_CONSTRUCTOR(Text)
{
    m_font = nullptr;
    m_colorOverride = Color(255, 255, 255, 0);
    m_color = GetSkin()->Colors.Label.Default;
    SetMouseInputEnabled(false);
    SetWrap(false);
}

Text::~Text()
{
    // NOTE: This font doesn't need to be released
    // Because it's a pointer to another font somewhere.
}

void Text::Layout(Skin::Base* skin)
{
    if (m_bTextChanged)
    {
        RefreshSize();
        m_bTextChanged = false;
    }
}

Gwk::Font* Text::GetFont()
{
    return m_font;
}

void Text::SetFont(Gwk::Font* font)
{
    if (m_font == font)
        return;

    m_font = font;
    m_bTextChanged = true;
    // Change the font of multilines too!
    for (auto&& line : m_lines)
    {
        line->SetFont(m_font);
    }

    Invalidate();
}

void Text::SetString(const String& str)
{
    if (m_string == str)
        return;

    m_string = str;
    m_bTextChanged = true;
    Invalidate();
}

void Text::Render(Skin::Base* skin)
{
    if (m_bWrap)
        return;

    if (Length() == 0 || !GetFont())
        return;

    if (m_colorOverride.a == 0)
        skin->GetRender()->SetDrawColor(m_color);
    else
        skin->GetRender()->SetDrawColor(m_colorOverride);

    skin->GetRender()->RenderText(GetFont(),
                                  Gwk::Point(GetPadding().left, GetPadding().top),
                                  m_string);
}

Gwk::Rect Text::GetCharacterPosition(unsigned int iChar)
{
    if (!m_lines.empty())
    {
        TextLines::iterator it = m_lines.begin();
        TextLines::iterator itEnd = m_lines.end();
        int iChars = 0;

        Text* line = nullptr;
        while (it != itEnd)
        {
            line = *it;
            ++it;
            iChars += line->Length();

            if (iChars <= iChar)
                continue;

            iChars -= line->Length();
            Gwk::Rect rect = line->GetCharacterPosition(iChar-iChars);
            rect.x += line->X();
            rect.y += line->Y();
            return rect;
        }

        //manage special case of the last character
        Gwk::Rect rect = line->GetCharacterPosition(line->Length());
        rect.x += line->X();
        rect.y += line->Y();
        return rect;
    }

    if (Length() == 0 || iChar == 0)
    {
        Gwk::Point p = GetSkin()->GetRender()->MeasureText(GetFont(), " ");
        return Gwk::Rect(0, 0, 0, p.y);
    }

    String sub = m_string.substr(0, iChar);
    Gwk::Point p = GetSkin()->GetRender()->MeasureText(GetFont(), sub);
    return Rect(p.x, 0, 0, p.y);
}

Gwk::Rect Text::GetLineBox(int i)
{
    Text* line = GetLine(i);
    if (line != nullptr)
    {
        Gwk::Point p = GetSkin()->GetRender()->MeasureText(GetFont(), line->m_string);
        return Gwk::Rect(line->X(), line->Y(), Clamp(p.x, 1,p.x), Clamp(p.y, 1,p.y));
    }
    else
    {
        Gwk::Point p = GetSkin()->GetRender()->MeasureText(GetFont(), m_string);
        return Gwk::Rect(0, 0, Clamp(p.x, 1,p.x), Clamp(p.y, 1,p.y));
    }
}

int Text::GetClosestCharacter(Gwk::Point p)
{
    if (!m_lines.empty())
    {
        TextLines::iterator it = m_lines.begin();
        TextLines::iterator itEnd = m_lines.end();
        int iChars = 0;

        Text *line = nullptr;
        while (it != itEnd)
        {
            line = *it;
            ++it;
            iChars += line->Length();

            if (p.y < line->Y())
                continue;
            if (p.y > line->Bottom())
                continue;
            if (p.y < line->Bottom())
                break;
        }

        iChars -= line->Length();
        int iLinePos = line->GetClosestCharacter(Gwk::Point(p.x-line->X(), p.y-line->Y()));        
        return iChars+iLinePos;
    }

    int iDistance = 4096;
    int iChar = 0;

    for (unsigned i = 0; i < m_string.length()+1; i++)
    {
        Gwk::Rect cp = GetCharacterPosition(i);
        const int iDist = std::abs(cp.x-p.x) + std::abs(cp.y-p.y);  // this isn't proper

        if (iDist > iDistance)
            continue;

        iDistance = iDist;
        iChar = i;
    }

    return iChar;
}

void Text::OnScaleChanged()
{
    Invalidate();
}

void Text::RefreshSize()
{
    if (m_bWrap)
        return RefreshSizeWrap();

    if (!GetFont())
    {
        Debug::AssertCheck(0, "Text::RefreshSize() - No Font!!\n");
        return;
    }

    Gwk::Point p(1, GetFont()->size);

    if (Length() > 0)
        p = GetSkin()->GetRender()->MeasureText(GetFont(), m_string);

    p.x += GetPadding().left+GetPadding().right;
    p.y += GetPadding().top+GetPadding().bottom;

    if (p.x == Width() && p.y == Height())
        return;

    if (p.y < GetFont()->size)
        p.y = GetFont()->size;

    SetSize(p.x, p.y);
    InvalidateParent();
    Invalidate();
}

void Text::SplitWords(const Gwk::String& s, std::vector<Gwk::String>& elems)
{
    Gwk::String str;

    int w = GetParent()->Width()
                - (GetParent()->GetPadding().left + GetParent()->GetPadding().right);
    
    for (size_t i = 0; i < s.length(); i++)
    {
        if (s[i] == '\n')
        {
            if (!str.empty())
                elems.push_back(str);

            elems.push_back("\n");
            str.clear();
            continue;
        }

        if (s[i] == ' ')
        {
            str += s[i];
            elems.push_back(str);
            str.clear();
            continue;
        }

        str += s[i];

        // if adding character makes the word bigger than the textbox size
        Gwk::Point p = GetSkin()->GetRender()->MeasureText(GetFont(), str);
        if (p.x > w)
        {
            //split words
            str.pop_back();
            elems.push_back(str);
            str.clear();
            --i;
            continue;
        }
    }

    if (!str.empty())
        elems.push_back(str);
}

void Text::RefreshSizeWrap()
{
    RemoveAllChildren();

    for (auto&& line : m_lines)
    {
        delete line;
    }
    m_lines.clear();
    
    std::vector<Gwk::String> words;
    SplitWords(GetText(), words);

    // Adding a word to the end simplifies the code below
    // which is anything but simple.
    words.push_back("");

    if (!GetFont())
    {
        Debug::AssertCheck(0, "Text::RefreshSize() - No Font!!\n");
        return;
    }

    Point fontSize = GetSkin()->GetRender()->MeasureText(GetFont(), " ");
    int w = GetParent()->Width() - GetParent()->GetPadding().left - GetParent()->GetPadding().right;
    int x = 0, y = 0;
    Gwk::String strLine;

    for (auto&& it = words.begin(); it != words.end(); ++it)
    {
        bool bFinishLine = false;
        bool bWrapped = false;

        // If this word is a newline - make a newline (we still add it to the
        // text)
        if ((*it).c_str() [0] == '\n')
            bFinishLine = true;

        // Does adding this word drive us over the width?
        {
            strLine += *it;
            Gwk::Point p = GetSkin()->GetRender()->MeasureText(GetFont(), strLine);

            if (p.x > Width() && p.x > w)
            {
                bFinishLine = true;
                bWrapped = true;
            }
        }

        // If this is the last word then finish the line
        if (--words.end() == it)
            bFinishLine = true;

        if (bFinishLine)
        {
            Text* t = new Text(this);
            t->SetFont(GetFont());
            if (bWrapped)
            {
                t->SetString(strLine.substr(0, strLine.length()-(*it).length()));
                // newline should start with the word that was too big
                strLine = *it;
            }
            else
            {
                t->SetString(strLine.substr(0, strLine.length()));
                //new line is empty
                strLine.clear();
            }
            t->RefreshSize();
            t->SetPos(x, y);
            m_lines.push_back(t);
            // newline should start with the word that was too big
            // strLine = *it;

            // Position the newline
            y += fontSize.y;
            x = 0;
            // if ( strLine[0] == ' ' ) x -= fontSize.x;
        }
    }

    // Size to children height and parent width
    {
        Point childsize = ChildrenSize();
        SetSize(w, childsize.y);
    }
    InvalidateParent();
    Invalidate();
}

unsigned int Text::NumLines()
{
    return static_cast<unsigned int>(m_lines.size());
}

Text* Text::GetLine(int i)
{
    TextLines::iterator it = m_lines.begin();
    TextLines::iterator itEnd = m_lines.end();

    while (it != itEnd)
    {
        if (i == 0)
            return *it;

        ++it;
        i--;
    }

    return nullptr;
}

int Text::GetLineFromChar(int i)
{
    int iChars = 0;
    int iLine = 0;

    for (auto&& line : m_lines)
    {
        iChars += line->Length();

        if (iChars > i)
            return iLine;

        iLine++;
    }

    if (iLine > 0)
        return iLine-1;
    
    return iLine;
}

int Text::GetStartCharFromLine(int i)
{
    int iChars = 0;

    for (auto&& line : m_lines)
    {
        if (i == 0)
            return Gwk::Clamp(iChars, 0, Length());

        iChars += line->Length();
        i--;
    }

    return Gwk::Clamp(iChars, 0, Length());
}

int Text::GetEndCharFromLine(int i)
{
    int iStart = GetStartCharFromLine(i);
    Text* iLine = GetLine(i);

    if (iLine)
        iStart += iLine->Length();

    return Gwk::Clamp(iStart, 0, Length());
}

int Text::GetCharPosOnLine(int i)
{
    int iLine = GetLineFromChar(i);
    Text* line = GetLine(iLine);

    if (!line)
        return 0;

    int iStart = GetStartCharFromLine(iLine);
    return i-iStart;
}
