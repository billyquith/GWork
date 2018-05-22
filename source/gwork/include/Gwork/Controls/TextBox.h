/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_TEXTBOX_H
#define GWK_CONTROLS_TEXTOBX_H

#include <Gwork/BaseRender.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Controls/ScrollControl.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT TextBox : public Label
        {
            GWK_CONTROL(TextBox, Label);

            void Render(Skin::Base* skin) override;
            void RenderFocus(Gwk::Skin::Base* /*skin*/) override {}

            void Layout(Skin::Base* skin) override;
            void PostLayout(Skin::Base* skin) override;

#if GWK_ANIMATE
            virtual void UpdateCaretColor();
#endif

            bool OnChar(Gwk::UnicodeChar c) override;

            virtual void InsertText(const Gwk::String& str);
            virtual void DeleteText(int iStartPos, int iLength);

            virtual void RefreshCursorBounds();

            bool OnKeyReturn(bool bDown) override;
            bool OnKeyBackspace(bool bDown) override;
            bool OnKeyDelete(bool bDown) override;
            bool OnKeyRight(bool bDown) override;
            bool OnKeyLeft(bool bDown) override;
            bool OnKeyHome(bool bDown) override;
            bool OnKeyEnd(bool bDown) override;

            bool AccelOnlyFocus() override { return true; }

            void OnPaste(Gwk::Controls::Base* ctrl) override;
            void OnCopy(Gwk::Controls::Base* ctrl) override;
            void OnCut(Gwk::Controls::Base* ctrl) override;
            void OnSelectAll(Gwk::Controls::Base* ctrl) override;

            void OnMouseDoubleClickLeft(int x, int y) override;

            virtual void EraseSelection();
            virtual bool HasSelection();
            virtual String GetSelection();

            virtual void SetCursorPos(int i);
            virtual void SetCursorEnd(int i);

            void SetMaxTextLength(int maxLength) { m_maxTextLength = maxLength; }
            int GetMaxTextLength() const { return m_maxTextLength; }

            void OnMouseClickLeft(int x, int y, bool bDown) override;
            void OnMouseMoved(int x, int y, int deltaX, int deltaY) override;

            virtual void SetEditable(bool b) { m_bEditable = b; }

            virtual void SetSelectAllOnFocus(bool b)
            {
                m_bSelectAll = b;
                if (b)
                    OnSelectAll(this);
            }

            virtual void MakeCaretVisible();

            virtual void OnEnter();

            bool NeedsInputChars() override { return true; }

            virtual void MoveCaretToEnd();
            virtual void MoveCaretToStart();

            Event::Caller onTextChanged;
            Event::Caller onReturnPressed;

            static constexpr int NO_MAX_LENGTH = -1;

        protected:

            void OnTextChanged() override;
            virtual bool IsTextAllowed(const Gwk::String& /*str*/, int /*iPos*/)
            {
                return true;
            }

            bool m_bEditable;
            bool m_bSelectAll;

            int m_cursorPos;
            int m_cursorEnd;
            int m_cursorLine;

            int m_maxTextLength;

            Gwk::Rect m_rectSelectionBounds;
            Gwk::Rect m_rectCaretBounds;

            float m_fNextCaretColorChange;
            Gwk::Color m_caretColor;
        };


        class GWK_EXPORT TextBoxNumeric : public TextBox
        {
        public:

            GWK_CONTROL(TextBoxNumeric, TextBox);

            virtual float GetFloatFromText();

        private:

            bool IsTextAllowed(const Gwk::String& str, int iPos) override;

        };


        class GWK_EXPORT TextBoxMultiline : public TextBox
        {
        public:

            GWK_CONTROL(TextBoxMultiline, TextBox);

            bool OnKeyReturn(bool bDown) override;
            void Render(Skin::Base* skin) override;
            void MakeCaretVisible() override;

            bool OnKeyHome(bool bDown) override;
            bool OnKeyEnd(bool bDown) override;
            bool OnKeyUp(bool bDown) override;
            bool OnKeyDown(bool bDown) override;

            virtual int GetCurrentLine();

        protected:

        };


        class GWK_EXPORT PasswordTextBox : public TextBox
        {
        public:

            GWK_CONTROL(PasswordTextBox, TextBox);

            void SetText(const String& str, bool bDoEvents = true) override;
            virtual void SetPasswordChar(const char c);

            const String& GetText() const override { return m_realText; }

        protected:

            char m_passwordChar;
            String m_realText;
        };


    }
}
#endif // ifndef GWK_CONTROLS_TEXTBOX_H
