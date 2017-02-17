/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_RICHLABEL_H
#define GWK_CONTROLS_RICHLABEL_H

#include <Gwork/BaseRender.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Text.h>


namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT RichLabel : public Controls::Base
        {
        public:

            GWK_CONTROL(RichLabel, Gwk::Controls::Base);

            void AddLineBreak();
            void AddText(const Gwk::String& text, Gwk::Color color, Gwk::Font* font = nullptr);

            bool SizeToChildren(bool w = true, bool h = true) override;

        protected:

            struct DividedText
            {
                typedef std::list<DividedText> List;
                DividedText()
                {
                    type = 0;
                    font = nullptr;
                }

                unsigned char       type;
                Gwk::String text;
                Gwk::Color         color;
                Gwk::Font        * font;
            };


            void Layout(Gwk::Skin::Base* skin) override;
            void SplitLabel(const Gwk::String& text, Gwk::Font* font,
                            const DividedText& txt, int& x, int& y, int& lineheight);
            void CreateNewline(int& x, int& y, int& lineheight);
            void CreateLabel(const Gwk::String& text, const DividedText& txt, int& x,
                             int& y, int& lineheight, bool NoSplit);
            void Rebuild();

            void OnBoundsChanged(Gwk::Rect oldBounds) override;

            DividedText::List m_textBlocks;
            bool m_bNeedsRebuild;
        };


    }
}
#endif // ifndef GWK_CONTROLS_RICHLABEL_H
