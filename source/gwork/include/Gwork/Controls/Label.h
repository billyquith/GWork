/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_LABEL_H
#define GWK_CONTROLS_LABEL_H

#include <Gwork/BaseRender.h>
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
            
            virtual void PreDelete(Gwk::Skin::Base* skin) override;

            virtual void SetText(const String& str, bool bDoEvents = true);

            virtual const String& GetText() const
            {
                return m_Text->GetText();
            }

            virtual void Render(Skin::Base* /*skin*/) override
            {
            }

            virtual void PostLayout(Skin::Base* skin) override;

            virtual void SizeToContents();

            virtual void SetAlignment(int iAlign);
            virtual int  GetAlignment();


            virtual void SetFont(Gwk::String strFacename, int iSize, bool bBold);

            virtual void SetFont(Gwk::Font* pFont)
            {
                m_Text->SetFont(pFont);
            }

            virtual Gwk::Font* GetFont()
            {
                return m_Text->GetFont();
            }

            virtual void SetTextColor(const Gwk::Color& col)
            {
                m_Text->SetTextColor(col);
            }

            virtual void SetTextColorOverride(const Gwk::Color& col)
            {
                m_Text->SetTextColorOverride(col);
            }

            inline const Gwk::Color& GetTextColor() const
            {
                return m_Text->TextColor();
            }

            virtual int TextWidth()         { return m_Text->Width(); }
            virtual int TextRight()         { return m_Text->Right(); }
            virtual int TextHeight()        { return m_Text->Height(); }

            virtual int TextX()             { return m_Text->X(); }
            virtual int TextY()             { return m_Text->Y(); }

            virtual int TextLength()        { return m_Text->Length(); }
            
            Gwk::Rect GetCharacterPosition(int iChar);

            virtual void SetTextPadding(const Padding& padding)
            {
                m_Text->SetPadding(padding);
                Invalidate();
                InvalidateParent();
            }

            virtual const Padding& GetTextPadding()
            {
                return m_Text->GetPadding();
            }

            int Alignment() const
            {
                return m_iAlign;
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

            virtual String GetValue() override
            {
                return GetText();
            }

            virtual void SetValue(const String& strValue) override
            {
                return SetText(strValue);
            }

            virtual bool Wrap()
            {
                return m_Text->Wrap();
            }

            virtual void SetWrap(bool b)
            {
                m_Text->SetWrap(b);
            }

            virtual void OnBoundsChanged(Gwk::Rect oldChildBounds) override;

        protected:

            virtual void OnTextChanged() {}

            Gwk::Font*                 m_CreatedFont;
            ControlsInternal::Text*     m_Text;
            int m_iAlign;

        };


    }
}

#endif // ifndef GWK_CONTROLS_LABEL_H
