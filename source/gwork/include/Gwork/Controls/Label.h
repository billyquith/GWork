/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_LABEL_H
#define GWK_CONTROLS_LABEL_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Text.h>

namespace Gwk
{
    namespace Controls
    {
        //
        //! A control that contains a text item.
        //
        class GWK_EXPORT Label : public Controls::Base
        {
        public:
            GWK_CONTROL(Label, Controls::Base);
            
            void PreDelete(Gwk::Skin::Base* skin) override;

            virtual void SetText(const String& str, bool bDoEvents = true);

            virtual const String& GetText() const
            {
                return m_text->GetText();
            }

            void Render(Skin::Base* /*skin*/) override
            {}

            void PostLayout(Skin::Base* skin) override;

            virtual void SizeToContents();

            virtual void SetAlignment(Position area);
            virtual Position GetAlignment();

            virtual void SetFont(Gwk::String strFacename, int iSize, bool bBold);

            virtual void SetFont(Gwk::Font* font)
            {
                m_text->SetFont(font);
            }

            virtual Gwk::Font* GetFont()
            {
                return m_text->GetFont();
            }

            virtual void SetTextColor(const Gwk::Color& col)
            {
                m_text->SetTextColor(col);
            }

            virtual void SetTextColorOverride(const Gwk::Color& col)
            {
                m_text->SetTextColorOverride(col);
            }

            inline const Gwk::Color& GetTextColor() const
            {
                return m_text->TextColor();
            }

            virtual int TextWidth()         { return m_text->Width(); }
            virtual int TextRight()         { return m_text->Right(); }
            virtual int TextHeight()        { return m_text->Height(); }

            virtual int TextX()             { return m_text->X(); }
            virtual int TextY()             { return m_text->Y(); }

            virtual int TextLength()        { return m_text->Length(); }
            
            Gwk::Rect GetCharacterPosition(int iChar);

            virtual void SetTextPadding(const Padding& padding)
            {
                m_text->SetPadding(padding);
                Invalidate();
                InvalidateParent();
            }

            virtual const Padding& GetTextPadding()
            {
                return m_text->GetPadding();
            }

            Position Alignment() const
            {
                return m_align;
            }

            virtual void MakeColorNormal()
            {
                SetTextColor(GetSkin()->Colors.Label.Default);
            }

            virtual void MakeColorBright()
            {
                SetTextColor(GetSkin()->Colors.Label.Bright);
            }

            virtual void MakeColorDark()
            {
                SetTextColor(GetSkin()->Colors.Label.Dark);
            }

            virtual void MakeColorHighlight()
            {
                SetTextColor(GetSkin()->Colors.Label.Highlight);
            }

            String GetValue() override
            {
                return GetText();
            }

            void SetValue(const String& strValue) override
            {
                return SetText(strValue);
            }

            virtual bool Wrap()
            {
                return m_text->Wrap();
            }

            virtual void SetWrap(bool b)
            {
                m_text->SetWrap(b);
            }

            void OnBoundsChanged(Gwk::Rect oldChildBounds) override;

        protected:

            virtual void OnTextChanged() {}

            Gwk::Font*                  m_createdFont;
            ControlsInternal::Text*     m_text;
            Position                 m_align;

        };


    }
}

#endif // ifndef GWK_CONTROLS_LABEL_H
