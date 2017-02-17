/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Controls/TextBox.h>
#include <Gwork/Skin.h>
#include <Gwork/Anim.h>
#include <Gwork/Utility.h>
#include <Gwork/Platform.h>
#include <Gwork/InputHandler.h>
#include <cmath>

namespace Gwk
{
    
using namespace Gwk::Controls;

#if GWK_ANIMATE
class ChangeCaretColor : public Gwk::Anim::Animation
{
public:

    void Think() override
    {
        gwk_cast<TextBox>(m_control)->UpdateCaretColor();
    }

};
#endif // if GWK_ANIMATE


GWK_CONTROL_CONSTRUCTOR(TextBox)
{
    SetSize(200, 20);
    SetMouseInputEnabled(true);
    SetKeyboardInputEnabled(true);
    SetAlignment(Position::Left|Position::CenterV);
    SetPadding(Padding(4, 2, 4, 2));
    m_cursorPos = 0;
    m_cursorEnd = 0;
    m_cursorLine = 0;
    m_bEditable = true;
    m_bSelectAll = false;
    SetTextColor(Gwk::Color(50, 50, 50, 255));         // TODO: From Skin
    SetTabable(true);
    AddAccelerator("Ctrl + C", &TextBox::OnCopy);
    AddAccelerator("Ctrl + X", &TextBox::OnCut);
    AddAccelerator("Ctrl + V", &TextBox::OnPaste);
    AddAccelerator("Ctrl + A", &TextBox::OnSelectAll);
    Gwk::Anim::Add(this, new ChangeCaretColor());
}

bool TextBox::OnChar(Gwk::UnicodeChar c)
{
    if (c == '\t')
        return false;

    InsertText(String{c});
    return true;
}

void TextBox::InsertText(const Gwk::String& strInsert)
{
    if (!m_bEditable)
        return;

    // TODO: Make sure fits (implement maxlength)
    if (HasSelection())
        EraseSelection();

    if (m_cursorPos > TextLength())
        m_cursorPos = TextLength();

    if (!IsTextAllowed(strInsert, m_cursorPos))
        return;

    String str = GetText();
    str.insert(m_cursorPos, strInsert);
    SetText(str);
    m_cursorPos += static_cast<int>(strInsert.size());
    m_cursorEnd = m_cursorPos;
    m_cursorLine = 0;
    RefreshCursorBounds();
}

#if GWK_ANIMATE
void TextBox::UpdateCaretColor()
{
    if (m_fNextCaretColorChange > Gwk::Platform::GetTimeInSeconds())
        return;

    if (!IsFocussed())
    {
        m_fNextCaretColorChange = Gwk::Platform::GetTimeInSeconds()+0.5f; return;
    }

    Gwk::Color targetcolor = Gwk::Color(230, 230, 230, 255);

    if (m_caretColor == targetcolor)
        targetcolor = Gwk::Color(20, 20, 20, 255);

    m_fNextCaretColorChange = Gwk::Platform::GetTimeInSeconds()+0.5f;
    m_caretColor = targetcolor;
    Redraw();
}

#endif // if GWK_ANIMATE

void TextBox::Render(Skin::Base* skin)
{
    if (ShouldDrawBackground())
        skin->DrawTextBox(this);

    if (!IsFocussed())
        return;

    // Draw selection.. if selected..
    if (m_cursorPos != m_cursorEnd)
    {
        skin->GetRender()->SetDrawColor(Gwk::Color(50, 170, 255, 200));
        skin->GetRender()->DrawFilledRect(m_rectSelectionBounds);
    }

    // Draw caret
    skin->GetRender()->SetDrawColor(m_caretColor);
    skin->GetRender()->DrawFilledRect(m_rectCaretBounds);
}

void TextBox::RefreshCursorBounds()
{
    m_fNextCaretColorChange = Gwk::Platform::GetTimeInSeconds()+1.5f;
    m_caretColor = Gwk::Color(30, 30, 30, 255);
    MakeCaretVisible();
    Gwk::Rect pA = GetCharacterPosition(m_cursorPos);
    Gwk::Rect pB = GetCharacterPosition(m_cursorEnd);
    m_rectSelectionBounds.x = std::min(pA.x, pB.x);
    m_rectSelectionBounds.y = m_text->Y()-1;
    m_rectSelectionBounds.w = std::max(pA.x, pB.x)-m_rectSelectionBounds.x;
    m_rectSelectionBounds.h = m_text->Height()+2;
    m_rectCaretBounds.x = pA.x;
    m_rectCaretBounds.y = pA.y;
    m_rectCaretBounds.w = 1;
    m_rectCaretBounds.h = pA.h;
    Redraw();
}

void TextBox::OnPaste(Event::Info)
{
    InsertText(Platform::GetClipboardText());
}

void TextBox::OnCopy(Event::Info)
{
    if (!HasSelection())
        return;

    Platform::SetClipboardText(GetSelection());
}

void TextBox::OnCut(Event::Info)
{
    if (!HasSelection())
        return;

    Platform::SetClipboardText(GetSelection());
    EraseSelection();
}

void TextBox::OnSelectAll(Event::Info)
{
    m_cursorEnd = 0;
    m_cursorPos = TextLength();
    m_cursorLine = 0;
    RefreshCursorBounds();
}

void TextBox::OnMouseDoubleClickLeft(int /*x*/, int /*y*/)
{
    OnSelectAll(this);
}

String TextBox::GetSelection()
{
    if (!HasSelection())
        return "";

    int iStart = std::min(m_cursorPos, m_cursorEnd);
    int iEnd = std::max(m_cursorPos, m_cursorEnd);
    const String& str = GetText();
    return str.substr(iStart, iEnd-iStart);
}

bool TextBox::OnKeyReturn(bool bDown)
{
    if (bDown)
        return true;

    // Try to move to the next control, as if tab had been pressed
    OnKeyTab(true);

    // If we still have focus, unfocus it.
    if (IsFocussed())
        Unfocus();

    // This is called AFTER the unfocusring so you can
    // refocus in your onReturnPressed hook.
    OnEnter();
    return true;
}

bool TextBox::OnKeyBackspace(bool bDown)
{
    if (!bDown)
        return true;

    if (HasSelection())
    {
        EraseSelection();
        return true;
    }

    if (m_cursorPos == 0)
        return true;

    DeleteText(m_cursorPos-1, 1);
    return true;
}

bool TextBox::OnKeyDelete(bool bDown)
{
    if (!bDown)
        return true;

    if (HasSelection())
    {
        EraseSelection();
        return true;
    }

    if (m_cursorPos >= TextLength())
        return true;

    DeleteText(m_cursorPos, 1);
    return true;
}

bool TextBox::OnKeyLeft(bool bDown)
{
    if (!bDown)
        return true;

    if (m_cursorPos > 0)
        m_cursorPos--;

    if (!Gwk::Input::IsShiftDown())
        m_cursorEnd = m_cursorPos;

    RefreshCursorBounds();
    return true;
}

bool TextBox::OnKeyRight(bool bDown)
{
    if (!bDown)
        return true;

    if (m_cursorPos < TextLength())
        m_cursorPos++;

    if (!Gwk::Input::IsShiftDown())
        m_cursorEnd = m_cursorPos;

    RefreshCursorBounds();
    return true;
}

bool TextBox::OnKeyHome(bool bDown)
{
    if (!bDown)
        return true;

    m_cursorPos = 0;

    if (!Gwk::Input::IsShiftDown())
        m_cursorEnd = m_cursorPos;

    RefreshCursorBounds();
    return true;
}

bool TextBox::OnKeyEnd(bool /*bDown*/)
{
    m_cursorPos = TextLength();

    if (!Gwk::Input::IsShiftDown())
        m_cursorEnd = m_cursorPos;

    RefreshCursorBounds();
    return true;
}

void TextBox::SetCursorPos(int i)
{
    if (m_cursorPos == i)
        return;

    m_cursorPos = i;
    m_cursorLine = 0;
    RefreshCursorBounds();
}

void TextBox::SetCursorEnd(int i)
{
    if (m_cursorEnd == i)
        return;

    m_cursorEnd = i;
    RefreshCursorBounds();
}

void TextBox::DeleteText(int iStartPos, int iLength)
{
    if (!m_bEditable)
        return;

    String str = GetText();
    str.erase(iStartPos, iLength);
    SetText(str);

    if (m_cursorPos > iStartPos)
        SetCursorPos(m_cursorPos-iLength);

    SetCursorEnd(m_cursorPos);
}

bool TextBox::HasSelection()
{
    return m_cursorPos != m_cursorEnd;
}

void TextBox::EraseSelection()
{
    int iStart = std::min(m_cursorPos, m_cursorEnd);
    int iEnd = std::max(m_cursorPos, m_cursorEnd);
    DeleteText(iStart, iEnd-iStart);
    // Move the cursor to the start of the selection,
    // since the end is probably outside of the string now.
    m_cursorPos = iStart;
    m_cursorEnd = iStart;
}

void TextBox::OnMouseClickLeft(int x, int y, bool bDown)
{
    if (m_bSelectAll)
    {
        OnSelectAll(this);
        m_bSelectAll = false;
        return;
    }

    int iChar = m_text->GetClosestCharacter(m_text->CanvasPosToLocal(Gwk::Point(x, y)));

    if (bDown)
    {
        SetCursorPos(iChar);

        if (!Gwk::Input::IsShiftDown())
            SetCursorEnd(iChar);

        Gwk::MouseFocus = this;
    }
    else
    {
        if (Gwk::MouseFocus == this)
        {
            SetCursorPos(iChar);
            Gwk::MouseFocus = nullptr;
        }
    }
}

void TextBox::OnMouseMoved(int x, int y, int /*deltaX*/, int /*deltaY*/)
{
    if (Gwk::MouseFocus != this)
        return;

    int iChar = m_text->GetClosestCharacter(m_text->CanvasPosToLocal(Gwk::Point(x, y)));
    SetCursorPos(iChar);
}

void TextBox::MakeCaretVisible()
{
    if (m_text->Width() < Width())
    {
        m_text->SetPosition(m_align);
    }
    else
    {
        int iCaretPos = m_text->GetCharacterPosition(m_cursorPos).x;
        int iRealCaretPos = iCaretPos+m_text->X();
        int iSlidingZone = m_text->GetFont()->size+1;   // Width()*0.1f

        // If the carat is already in a semi-good position, leave it.
        if (iRealCaretPos >= iSlidingZone && iRealCaretPos <= Width()-iSlidingZone)
            return;

        int x = 0;

        if (iRealCaretPos > Width()-iSlidingZone)
            x = Width()-iCaretPos-iSlidingZone;

        if (iRealCaretPos < iSlidingZone)
            x = -iCaretPos+iSlidingZone;

        // Don't show too much whitespace to the right
        if (x+m_text->Width() < Width()-GetPadding().right)
            x = -m_text->Width()+(Width()-GetPadding().right);

        // Or the left
        if (x > GetPadding().left)
            x = GetPadding().left;

        int y = 0;

        if (m_align&Position::Top)
            y = GetPadding().top;

        if (m_align&Position::Bottom)
            y = Height()-m_text->Height()-GetPadding().bottom;

        if (m_align&Position::CenterV)
            y = (Height()-m_text->Height()) / 2;

        m_text->SetPos(x, y);
    }
}

void TextBox::Layout(Skin::Base* skin)
{
    ParentClass::Layout(skin);
    RefreshCursorBounds();
}

void TextBox::PostLayout(Skin::Base* skin)
{
}

void TextBox::OnTextChanged()
{
    if (m_cursorPos > TextLength())
        m_cursorPos = TextLength();

    if (m_cursorEnd > TextLength())
        m_cursorEnd = TextLength();

    onTextChanged.Call(this);
}

void TextBox::OnEnter()
{
    onReturnPressed.Call(this);
}

void TextBox::MoveCaretToEnd()
{
    m_cursorPos = TextLength();
    m_cursorEnd = TextLength();
    RefreshCursorBounds();
}

void TextBox::MoveCaretToStart()
{
    m_cursorPos = 0;
    m_cursorEnd = 0;
    RefreshCursorBounds();
}

GWK_CONTROL_CONSTRUCTOR(TextBoxMultiline)
{
    SetWrap(true);
    SetAlignment(Position::Left|Position::Top);
}

bool TextBoxMultiline::OnKeyReturn(bool bDown)
{
    if (bDown)
        InsertText("\n");

    return true;
}

void TextBoxMultiline::Render(Skin::Base* skin)
{
    if (ShouldDrawBackground())
        skin->DrawTextBox(this);

    if (!IsFocussed())
        return;

    if (m_cursorPos != m_cursorEnd)
    {
        int iCursorStartLine = m_text->GetLineFromChar(m_cursorPos);
        int iCursorEndLine = m_text->GetLineFromChar(m_cursorEnd);

        if (iCursorStartLine > m_text->NumLines()-1)
            iCursorStartLine =  m_text->NumLines()-1;
        if (iCursorEndLine > m_text->NumLines()-1)
            iCursorEndLine =  m_text->NumLines()-1;

        int iSelectionStartLine = (m_cursorPos < m_cursorEnd) ? iCursorStartLine : iCursorEndLine;
        int iSelectionEndLine =   (m_cursorPos < m_cursorEnd) ? iCursorEndLine : iCursorStartLine;

        int iSelectionStartPos =  (m_cursorPos < m_cursorEnd) ? m_cursorPos : m_cursorEnd;
        int iSelectionEndPos =    (m_cursorPos < m_cursorEnd) ? m_cursorEnd : m_cursorPos;

        skin->GetRender()->SetDrawColor(Gwk::Color(50, 170, 255, 200));
        m_rectSelectionBounds.h = m_text->GetFont()->size+2;

        for (int iLine = iSelectionStartLine; iLine <= iSelectionEndLine; ++iLine)
        {
            Gwk::Rect box = m_text->GetLineBox(iLine);
            box.x += m_text->X();
            box.y += m_text->Y();

            if (iLine == iSelectionStartLine)
            {
                Gwk::Rect pos = GetCharacterPosition(iSelectionStartPos);
                m_rectSelectionBounds.x = pos.x;
                m_rectSelectionBounds.y = pos.y-1;
            }
            else
            {
                m_rectSelectionBounds.x = box.x;
                m_rectSelectionBounds.y = box.y-1;
            }

            if (iLine == iSelectionEndLine)
            {
                Gwk::Rect pos = GetCharacterPosition(iSelectionEndPos);
                m_rectSelectionBounds.w = pos.x - m_rectSelectionBounds.x;
            }
            else
            {
                m_rectSelectionBounds.w = box.x + box.w - m_rectSelectionBounds.x;
            }
            if (m_rectSelectionBounds.w < 1)
                m_rectSelectionBounds.w = 1;

            skin->GetRender()->DrawFilledRect(m_rectSelectionBounds);
        }
    }

    // Draw selection.. if selected..
    if (m_cursorPos != m_cursorEnd)
    {
        //skin->GetRender()->SetDrawColor( Gwk::Color( 50, 170, 255, 200 ) );
        //skin->GetRender()->DrawFilledRect( m_rectSelectionBounds );
    }

    // Draw caret
    skin->GetRender()->SetDrawColor(m_caretColor);
    skin->GetRender()->DrawFilledRect(m_rectCaretBounds);
}

void TextBoxMultiline::MakeCaretVisible()
{
    if (m_text->Height() < Height())
    {
        m_text->SetPosition(m_align);
    }
    else
    {
        //const Rect& bounds = GetInnerBounds();

        //if ( pos & Position::Top ) y = bounds.y + ypadding;
        //if ( pos & Position::Bottom ) y = bounds.y + ( bounds.h - Height() - ypadding );
        //if ( pos & Position::CenterV ) y = bounds.y + ( bounds.h - Height() )  * 0.5;

        Rect pos = m_text->GetCharacterPosition(m_cursorPos);
        int iCaretPos = pos.y; // + pos.h;
        int iRealCaretPos = iCaretPos+m_text->Y();
        //int iSlidingZone =  m_text->GetFont()->size; //Width()*0.1f

        // If the carat is already in a semi-good position, leave it.
//        int mi = GetPadding().top;
//        int ma = Height()-pos.h-GetPadding().bottom;
        if (iRealCaretPos >= GetPadding().top && iRealCaretPos <= Height()-pos.h-
            GetPadding().bottom)
            return;

        int y = 0;

        // bottom of carat too low
        if (iRealCaretPos > Height()-pos.h-GetPadding().bottom)
        {
            //align bottom
            y = Height()-iCaretPos-pos.h-GetPadding().bottom;
        }

        // top of carat too low
        if (iRealCaretPos < GetPadding().top)
            y = -iCaretPos+GetPadding().top;

        // Don't show too much whitespace to the bottom
        if (y+m_text->Height() < Height()-GetPadding().bottom)
            y = -m_text->Height()+(Height()-GetPadding().bottom);

        // Or the top
        if (y > GetPadding().top)
            y = GetPadding().top;

        int x = 0;
        if (m_align&Position::Left)
            x = GetPadding().left;
        if (m_align&Position::Right)
            x = Width()-m_text->Width()-GetPadding().right;
        if (m_align&Position::CenterH)
            x = (Width()-m_text->Width())*0.5f;

        m_text->SetPos(x, y);
    }
}

int TextBoxMultiline::GetCurrentLine()
{
    return m_text->GetLineFromChar(m_cursorPos);
}

bool TextBoxMultiline::OnKeyHome(bool bDown)
{
    if (!bDown)
        return true;

    int iCurrentLine = GetCurrentLine();
    int iChar = m_text->GetStartCharFromLine(iCurrentLine);
    m_cursorLine = 0;
    m_cursorPos = iChar;

    if (!Gwk::Input::IsShiftDown())
        m_cursorEnd = m_cursorPos;

    RefreshCursorBounds();
    return true;
}

bool TextBoxMultiline::OnKeyEnd(bool bDown)
{
    if (!bDown)
        return true;

    int iCurrentLine = GetCurrentLine();
    int iChar = m_text->GetEndCharFromLine(iCurrentLine);
    m_cursorLine = 0;
    m_cursorPos = iChar;

    int iLastLine = m_text->NumLines()-1;

    if (iCurrentLine < iLastLine && iChar > 0)
        m_cursorPos = iChar-1; // NAUGHTY
    else
        m_cursorPos = m_text->Length();

    if (!Gwk::Input::IsShiftDown())
        m_cursorEnd = m_cursorPos;

    RefreshCursorBounds();
    return true;
}

bool TextBoxMultiline::OnKeyUp(bool bDown)
{
    if (!bDown)
        return true;
    //if ( m_cursorLine == 0 )
    m_cursorLine = m_text->GetCharPosOnLine(m_cursorPos);

    int iLine = m_text->GetLineFromChar(m_cursorPos);

    if (iLine == 0)
        return true;

    m_cursorPos = m_text->GetStartCharFromLine(iLine-1);
    m_cursorPos += Clamp(m_cursorLine, 0, m_text->GetLine(iLine-1)->Length()-1);
    m_cursorPos = Clamp(m_cursorPos, 0, m_text->Length());

    if (!Gwk::Input::IsShiftDown())
        m_cursorEnd = m_cursorPos;

    RefreshCursorBounds();
    return true;
}

bool TextBoxMultiline::OnKeyDown(bool bDown)
{
    if (!bDown)
        return true;
    //if ( m_cursorLine == 0 )
    m_cursorLine = m_text->GetCharPosOnLine(m_cursorPos);

    int iLine = m_text->GetLineFromChar(m_cursorPos);
    int iLastLine = m_text->NumLines()-1;
    if (iLine >= iLastLine || iLastLine < 1)
        return true;

    m_cursorPos = m_text->GetStartCharFromLine(iLine+1);
    if (iLine+1 >= iLastLine)
        m_cursorPos += Clamp(m_cursorLine, 0, m_text->GetLine(iLine+1)->Length());
    else
        m_cursorPos += Clamp(m_cursorLine, 0, m_text->GetLine(iLine+1)->Length()-1);
    m_cursorPos = Clamp(m_cursorPos, 0, m_text->Length());

    if (!Gwk::Input::IsShiftDown())
        m_cursorEnd = m_cursorPos;

    RefreshCursorBounds();
    return true;
}

GWK_CONTROL_CONSTRUCTOR(PasswordTextBox)
{
    m_realText = "";
    m_passwordChar = '*';
}

void PasswordTextBox::SetText(const String& str, bool bDoEvents)
{
    if (m_realText == str)
        return;

    m_realText = str;
    String passwordChars;
    for (size_t i = 0; i < m_realText.length(); i++)
    {
        passwordChars += m_passwordChar;
    }

    m_text->SetString(passwordChars);
    Redraw();

    if (bDoEvents)
        OnTextChanged();
}

void PasswordTextBox::SetPasswordChar(const char c)
{
    m_passwordChar = c;
}

} // namespace Gwk
