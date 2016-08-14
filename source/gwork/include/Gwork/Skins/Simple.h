/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_SKINS_SIMPLE_H
#define GWK_SKINS_SIMPLE_H

#include <Gwork/Skin.h>
#include <Gwork/Gwork.h>
#include <Gwork/Controls/Base.h>

namespace Gwk
{
    namespace Skin
    {
        class Simple : public Gwk::Skin::Base
        {
        public:

            Simple(Gwk::Renderer::Base* renderer)
                : Gwk::Skin::Base(renderer)
            {}

            Gwk::Color m_colBorderColor;
            Gwk::Color m_colControlOutlineLight;
            Gwk::Color m_colControlOutlineLighter;
            Gwk::Color m_colBG;
            Gwk::Color m_colBGDark;
            Gwk::Color m_colControl;
            Gwk::Color m_colControlBorderHighlight;
            Gwk::Color m_colControlDarker;
            Gwk::Color m_colControlOutlineNormal;
            Gwk::Color m_colControlBright;
            Gwk::Color m_colControlDark;
            Gwk::Color m_colHighlightBG;
            Gwk::Color m_colHighlightBorder;
            Gwk::Color m_colTooltipBackground;
            Gwk::Color m_colTooltipBorder;
            Gwk::Color m_colModal;

            Simple()
            {
                m_colBorderColor            = Gwk::Color(80, 80, 80, 255);
                m_colBG                     = Gwk::Color(248, 248, 248, 255);
                m_colBGDark                 = Gwk::Color(235, 235, 235, 255);
                m_colControl                = Gwk::Color(240, 240, 240, 255);
                m_colControlBright          = Gwk::Color(255, 255, 255, 255);
                m_colControlDark            = Gwk::Color(214, 214, 214, 255);
                m_colControlDarker          = Gwk::Color(180, 180, 180, 255);
                m_colControlOutlineNormal   = Gwk::Color(112, 112, 112, 255);
                m_colControlOutlineLight    = Gwk::Color(144, 144, 144, 255);
                m_colControlOutlineLighter  = Gwk::Color(210, 210, 210, 255);
                m_colHighlightBG            = Gwk::Color(192, 221, 252, 255);
                m_colHighlightBorder        = Gwk::Color(51, 153, 255, 255);
                m_colTooltipBackground      = Gwk::Color(255, 255, 225, 255);
                m_colTooltipBorder          = Gwk::Color(0, 0, 0, 255);
                m_colModal = Gwk::Color(25, 25, 25, 150);
                m_defaultFont.facename  = "Microsoft Sans Serif";
                m_defaultFont.size      = 11;
            }

            void DrawGenericPanel(Controls::Base* control) override
            {
                // TODO.
            }

            void DrawButton(Gwk::Controls::Base* control, bool bDepressed, bool bHovered,
                            bool bDisabled) override
            {
                int w = control->Width();
                int h = control->Height();
                DrawButton(w, h, bDepressed, bHovered);
            }

            void DrawWindowCloseButton(Gwk::Controls::Base* control, bool bDepressed,
                                               bool bHovered, bool bDisabled) override
            {
                // TODO.
                DrawButton(control, bDepressed, bHovered, bDisabled);
            }

            void DrawWindowMaximizeButton(Gwk::Controls::Base* control, bool bDepressed,
                                          bool bHovered, bool bDisabled, bool bMaximized) override
            {
                // TODO.
                DrawButton(control, bDepressed, bHovered, bDisabled);
            }

            void DrawWindowMinimizeButton(Gwk::Controls::Base* control, bool bDepressed,
                                          bool bHovered, bool bDisabled) override
            {
                // TODO.
                DrawButton(control, bDepressed, bHovered, bDisabled);
            }

            void DrawMenuItem(Gwk::Controls::Base* control, bool bSubmenuOpen,
                              bool bChecked) override
            {
                if (bSubmenuOpen || control->IsHovered())
                {
                    m_render->SetDrawColor(m_colHighlightBG);
                    m_render->DrawFilledRect(control->GetRenderBounds());
                    m_render->SetDrawColor(m_colHighlightBorder);
                    m_render->DrawLinedRect(control->GetRenderBounds());
                }

                if (bChecked)
                {
                    m_render->SetDrawColor(Color(0, 0, 0, 255));
                    Gwk::Rect r(control->Width()/2-2, control->Height()/2-2, 5, 5);
                    DrawCheck(r);
                }
            }

            void DrawMenuStrip(Gwk::Controls::Base* control) override
            {
                int w = control->Width();
                int h = control->Height();
                m_render->SetDrawColor(Gwk::Color(246, 248, 252, 255));
                m_render->DrawFilledRect(Gwk::Rect(0, 0, w, h));
                m_render->SetDrawColor(Gwk::Color(218, 224, 241, 150));
                m_render->DrawFilledRect(Gwk::Rect(0, h*0.4f, w, h*0.6f));
                m_render->DrawFilledRect(Gwk::Rect(0, h/2, w, h/2));
            }

            void DrawMenu(Gwk::Controls::Base* control, bool bPaddingDisabled) override
            {
                int w = control->Width();
                int h = control->Height();
                m_render->SetDrawColor(m_colControlBright);
                m_render->DrawFilledRect(Gwk::Rect(0, 0, w, h));

                if (!bPaddingDisabled)
                {
                    m_render->SetDrawColor(m_colControl);
                    m_render->DrawFilledRect(Gwk::Rect(1, 0, 22, h));
                }

                m_render->SetDrawColor(m_colControlOutlineNormal);
                m_render->DrawLinedRect(Gwk::Rect(0, 0, w, h));
            }

            void DrawShadow(Gwk::Controls::Base* control) override
            {
                const int w = control->Width(), h = control->Height();
                int x = 4, y = 6;
                m_render->SetDrawColor(Gwk::Color(0, 0, 0, 10));
                m_render->DrawFilledRect(Gwk::Rect(x, y, w, h));
                x += 2;
                m_render->DrawFilledRect(Gwk::Rect(x, y, w, h));
                y += 2;
                m_render->DrawFilledRect(Gwk::Rect(x, y, w, h));
            }

            virtual void DrawButton(int w, int h, bool bDepressed, bool bHovered,
                                    bool bSquared = false) 
            {
                if (bDepressed)
                    m_render->SetDrawColor(m_colControlDark);
                else if (bHovered)
                    m_render->SetDrawColor(m_colControlBright);
                else
                    m_render->SetDrawColor(m_colControl);
                m_render->DrawFilledRect(Gwk::Rect(1, 1, w-2, h-2));

                if (bDepressed)
                    m_render->SetDrawColor(m_colControlDark);
                else if (bHovered)
                    m_render->SetDrawColor(m_colControl);
                else
                    m_render->SetDrawColor(m_colControlDark);
                m_render->DrawFilledRect(Gwk::Rect(1, h/2, w-2, h/2-2));

                if (!bDepressed)
                {
                    m_render->SetDrawColor(m_colControlBright);
                    m_render->DrawShavedCornerRect(Gwk::Rect(1, 1, w-2, h-2), bSquared);
                }
                else
                {
                    m_render->SetDrawColor(m_colControlDarker);
                    m_render->DrawShavedCornerRect(Gwk::Rect(1, 1, w-2, h-2), bSquared);
                }

                // Border
                m_render->SetDrawColor(m_colControlOutlineNormal);
                m_render->DrawShavedCornerRect(Gwk::Rect(0, 0, w, h), bSquared);
            }

            void DrawRadioButton(Gwk::Controls::Base* control, bool bSelected,
                                 bool bDepressed) override
            {
                Gwk::Rect rect = control->GetRenderBounds();

                // Inside colour
                if (control->IsHovered())
                    m_render->SetDrawColor(Gwk::Color(220, 242, 254, 255));
                else
                    m_render->SetDrawColor(m_colControlBright);
                m_render->DrawFilledRect(Gwk::Rect(1, 1, rect.w-2, rect.h-2));

                // Border
                if (control->IsHovered())
                    m_render->SetDrawColor(Gwk::Color(85, 130, 164, 255));
                else
                    m_render->SetDrawColor(m_colControlOutlineLight);
                m_render->DrawShavedCornerRect(rect);
                m_render->SetDrawColor(Gwk::Color(0,  50,  60, 15));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+2, rect.y+2, rect.w-4, rect.h-4));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+2, rect.y+2, rect.w*0.3f, rect.h-4));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+2, rect.y+2, rect.w-4, rect.h*0.3f));

                if (control->IsHovered())
                    m_render->SetDrawColor(Gwk::Color(121, 198, 249, 255));
                else
                    m_render->SetDrawColor(Gwk::Color(0, 50, 60, 50));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+2, rect.y+3, 1, rect.h-5));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+3, rect.y+2, rect.w-5, 1));

                if (bSelected)
                {
                    m_render->SetDrawColor(Gwk::Color(40, 230, 30, 255));
                    m_render->DrawFilledRect(Gwk::Rect(rect.x+2, rect.y+2, rect.w-4, rect.h-4));
                }
            }

            void DrawCheckBox(Gwk::Controls::Base* control, bool bSelected,
                              bool bDepressed) override
            {
                Gwk::Rect rect = control->GetRenderBounds();

                // Inside colour
                if (control->IsHovered())
                    m_render->SetDrawColor(Gwk::Color(220, 242, 254, 255));
                else
                    m_render->SetDrawColor(m_colControlBright);
                m_render->DrawFilledRect(rect);

                // Border
                if (control->IsHovered())
                    m_render->SetDrawColor(Gwk::Color(85, 130, 164, 255));
                else
                    m_render->SetDrawColor(m_colControlOutlineLight);
                m_render->DrawLinedRect(rect);
                m_render->SetDrawColor(Gwk::Color(0,  50,  60, 15));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+2, rect.y+2, rect.w-4, rect.h-4));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+2, rect.y+2, rect.w*0.3f, rect.h-4));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+2, rect.y+2, rect.w-4, rect.h*0.3f));

                if (control->IsHovered())
                    m_render->SetDrawColor(Gwk::Color(121, 198, 249, 255));
                else
                    m_render->SetDrawColor(Gwk::Color(0, 50, 60, 50));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+2, rect.y+2, 1, rect.h-4));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+2, rect.y+2, rect.w-4, 1));

                if (bDepressed)
                {
                    m_render->SetDrawColor(Color(100, 100, 100, 255));
                    Gwk::Rect r(control->Width()/2-2, control->Height()/2-2, 5, 5);
                    DrawCheck(r);
                }
                else if (bSelected)
                {
                    m_render->SetDrawColor(Color(0, 0, 0, 255));
                    Gwk::Rect r(control->Width()/2-2, control->Height()/2-2, 5, 5);
                    DrawCheck(r);
                }
            }

            void DrawGroupBox(Gwk::Controls::Base* control, int textStart, int textHeight,
                              int textWidth) override
            {
                Gwk::Rect rect = control->GetRenderBounds();
                rect.y += textHeight/2;
                rect.h -= textHeight/2;
                Gwk::Color m_colDarker         = Gwk::Color(0,  50,  60, 50);
                Gwk::Color m_colLighter        = Gwk::Color(255, 255, 255, 150);
                m_render->SetDrawColor(m_colLighter);
                m_render->DrawFilledRect(Gwk::Rect(rect.x+1, rect.y+1, textStart-3, 1));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+1+textStart+textWidth, rect.y+1,
                                                    rect.w-textStart+textWidth-2, 1));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+1, (rect.y+rect.h)-1, rect.w-2, 1));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+1, rect.y+1, 1, rect.h));
                m_render->DrawFilledRect(Gwk::Rect((rect.x+rect.w)-2, rect.y+1, 1, rect.h-1));
                m_render->SetDrawColor(m_colDarker);
                m_render->DrawFilledRect(Gwk::Rect(rect.x+1, rect.y, textStart-3, 1));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+1+textStart+textWidth, rect.y,
                                                    rect.w-textStart-textWidth-2, 1));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+1, (rect.y+rect.h)-1, rect.w-2, 1));
                m_render->DrawFilledRect(Gwk::Rect(rect.x, rect.y+1, 1, rect.h-1));
                m_render->DrawFilledRect(Gwk::Rect((rect.x+rect.w)-1, rect.y+1, 1, rect.h-1));
            }

            void DrawTextBox(Gwk::Controls::Base* control) override
            {
                Gwk::Rect rect = control->GetRenderBounds();
                bool bIsFocussed = control->IsFocussed();

                // Box inside
                m_render->SetDrawColor(Gwk::Color(255, 255, 255, 255));
                m_render->DrawFilledRect(Gwk::Rect(1, 1, rect.w-2, rect.h-2));
                m_render->SetDrawColor(m_colControlOutlineLight);
                m_render->DrawFilledRect(Gwk::Rect(rect.x+1, rect.y, rect.w-2, 1));
                m_render->DrawFilledRect(Gwk::Rect(rect.x, rect.y+1, 1, rect.h-2));
                m_render->SetDrawColor(m_colControlOutlineLighter);
                m_render->DrawFilledRect(Gwk::Rect(rect.x+1, (rect.y+rect.h)-1, rect.w-2, 1));
                m_render->DrawFilledRect(Gwk::Rect((rect.x+rect.w)-1, rect.y+1, 1, rect.h-2));

                if (bIsFocussed)
                {
                    m_render->SetDrawColor(Gwk::Color(50, 200, 255, 150));
                    m_render->DrawLinedRect(rect);
                }
            }

            void DrawTabButton(Gwk::Controls::Base* control, bool bActive, Position dir) override
            {
                Gwk::Rect rect = control->GetRenderBounds();
                bool bHovered = control->IsHovered();

                if (bHovered)
                    m_render->SetDrawColor(m_colControlBright);
                else
                    m_render->SetDrawColor(m_colControl);
                m_render->DrawFilledRect(Gwk::Rect(1, 1, rect.w-2, rect.h-1));

                if (bHovered)
                    m_render->SetDrawColor(m_colControl);
                else
                    m_render->SetDrawColor(m_colControlDark);
                m_render->DrawFilledRect(Gwk::Rect(1, rect.h/2, rect.w-2, rect.h/2-1));
                m_render->SetDrawColor(m_colControlBright);
                m_render->DrawShavedCornerRect(Gwk::Rect(1, 1, rect.w-2, rect.h));
                m_render->SetDrawColor(m_colBorderColor);
                m_render->DrawShavedCornerRect(Gwk::Rect(0, 0, rect.w, rect.h));
            }

            void DrawTabControl(Gwk::Controls::Base* control) override
            {
                Gwk::Rect rect = control->GetRenderBounds();
                m_render->SetDrawColor(m_colControl);
                m_render->DrawFilledRect(rect);
                m_render->SetDrawColor(m_colBorderColor);
                m_render->DrawLinedRect(rect);
                //m_render->SetDrawColor( m_colControl );
                //m_render->DrawFilledRect( CurrentButtonRect );
            }

            void DrawWindow(Gwk::Controls::Base* control, int topHeight, bool inFocus) override
            {
                Gwk::Rect rect = control->GetRenderBounds();

                // Titlebar
                if (inFocus)
                    m_render->SetDrawColor(Gwk::Color(87, 164, 232, 230));
                else
                    m_render->SetDrawColor(Gwk::Color(87*0.70, 164*0.70, 232*0.70, 230));
                int iBorderSize = 5;
                m_render->DrawFilledRect(Gwk::Rect(rect.x+1, rect.y+1, rect.w-2, topHeight-1));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+1, rect.y+topHeight-1,
                                                    iBorderSize, rect.h-2-topHeight));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+rect.w-iBorderSize, rect.y+topHeight-1,
                                                    iBorderSize, rect.h-2-topHeight));
                m_render->DrawFilledRect(Gwk::Rect(rect.x+1, rect.y+rect.h-iBorderSize,
                                                    rect.w-2, iBorderSize));
                // Main inner
                m_render->SetDrawColor(Gwk::Color(m_colControlDark.r,
                                                   m_colControlDark.g,
                                                   m_colControlDark.b,
                                                   230));
                m_render->DrawFilledRect(Gwk::Rect(rect.x + iBorderSize + 1,
                                                    rect.y + topHeight,
                                                    rect.w - iBorderSize*2 - 2,
                                                    rect.h - topHeight - iBorderSize - 1));
                // Light inner border
                m_render->SetDrawColor(Gwk::Color(255, 255, 255, 100));
                m_render->DrawShavedCornerRect(Gwk::Rect(rect.x+1, rect.y+1, rect.w-2, rect.h-2));
                // Dark line between titlebar and main
                m_render->SetDrawColor(m_colBorderColor);
                // Inside border
                m_render->SetDrawColor(m_colControlOutlineNormal);
                m_render->DrawLinedRect(Gwk::Rect(rect.x+iBorderSize, rect.y+topHeight-1,
                                                   rect.w-10, rect.h-topHeight-(iBorderSize-1)));
                // Dark outer border
                m_render->SetDrawColor(m_colBorderColor);
                m_render->DrawShavedCornerRect(Gwk::Rect(rect.x, rect.y, rect.w, rect.h));
            }

            void DrawHighlight(Gwk::Controls::Base* control) override
            {
                Gwk::Rect rect = control->GetRenderBounds();
                m_render->SetDrawColor(Gwk::Color(255, 100, 255, 255));
                m_render->DrawFilledRect(rect);
            }

            void DrawScrollBar(Gwk::Controls::Base* control, bool isHorizontal,
                               bool bDepressed) override
            {
                Gwk::Rect rect = control->GetRenderBounds();

                if (bDepressed)
                    m_render->SetDrawColor(m_colControlDarker);
                else
                    m_render->SetDrawColor(m_colControlBright);
                m_render->DrawFilledRect(rect);
            }

            void DrawScrollBarBar(Controls::Base* control, bool bDepressed, bool isHovered,
                                  bool isHorizontal) override
            {
                //TODO: something specialized
                DrawButton(control, bDepressed, isHovered, false);
            }

            void DrawTabTitleBar(Gwk::Controls::Base* control) override
            {
                Gwk::Rect rect = control->GetRenderBounds();
                m_render->SetDrawColor(Gwk::Color(177, 193, 214, 255));
                m_render->DrawFilledRect(rect);
                m_render->SetDrawColor(m_colBorderColor);
                rect.h += 1;
                m_render->DrawLinedRect(rect);
            }

            void DrawProgressBar(Gwk::Controls::Base* control, bool isHorizontal,
                                 float progress) override
            {
                Gwk::Rect rect = control->GetRenderBounds();
                Gwk::Color FillColour(0, 211, 40, 255);

                if (isHorizontal)
                {
                    //Background
                    m_render->SetDrawColor(m_colControlDark);
                    m_render->DrawFilledRect(Gwk::Rect(1, 1, rect.w-2, rect.h-2));
                    //Right half
                    m_render->SetDrawColor(FillColour);
                    m_render->DrawFilledRect(Gwk::Rect(1, 1, rect.w*progress-2, rect.h-2));
                    m_render->SetDrawColor(Gwk::Color(255, 255, 255, 150));
                    m_render->DrawFilledRect(Gwk::Rect(1, 1, rect.w-2, rect.h*0.45f));
                }
                else
                {
                    //Background
                    m_render->SetDrawColor(m_colControlDark);
                    m_render->DrawFilledRect(Gwk::Rect(1, 1, rect.w-2, rect.h-2));
                    //Top half
                    m_render->SetDrawColor(FillColour);
                    m_render->DrawFilledRect(Gwk::Rect(1, 1+(rect.h*(1-progress)),
                                                        rect.w-2, rect.h*progress-2));
                    m_render->SetDrawColor(Gwk::Color(255, 255, 255, 150));
                    m_render->DrawFilledRect(Gwk::Rect(1, 1, rect.w*0.45f, rect.h-2));
                }

                m_render->SetDrawColor(Gwk::Color(255, 255, 255, 150));
                m_render->DrawShavedCornerRect(Gwk::Rect(1, 1, rect.w-2, rect.h-2));
                m_render->SetDrawColor(Gwk::Color(255, 255, 255, 70));
                m_render->DrawShavedCornerRect(Gwk::Rect(2, 2, rect.w-4, rect.h-4));
                m_render->SetDrawColor(m_colBorderColor);
                m_render->DrawShavedCornerRect(rect);
            }

            void DrawListBox(Gwk::Controls::Base* control) override
            {
                Gwk::Rect rect = control->GetRenderBounds();
                m_render->SetDrawColor(m_colControlBright);
                m_render->DrawFilledRect(rect);
                m_render->SetDrawColor(m_colBorderColor);
                m_render->DrawLinedRect(rect);
            }

            void DrawListBoxLine(Gwk::Controls::Base* control, bool bSelected, bool bEven) override
            {
                Gwk::Rect rect = control->GetRenderBounds();

                if (bSelected)
                {
                    m_render->SetDrawColor(m_colHighlightBorder);
                    m_render->DrawFilledRect(rect);
                }
                else if (control->IsHovered())
                {
                    m_render->SetDrawColor(m_colHighlightBG);
                    m_render->DrawFilledRect(rect);
                }
            }

            void DrawSlider(Gwk::Controls::Base* control, bool bIsHorizontal,
                            int numNotches, int barSize) override
            {
                Gwk::Rect rect = control->GetRenderBounds();

                if (bIsHorizontal)
                {
                    rect.y += rect.h*0.4f;
                    rect.h -= rect.h*0.8f;
                }
                else
                {
                    rect.x += rect.w*0.4f;
                    rect.w -= rect.w*0.8f;
                }

                m_render->SetDrawColor(m_colBGDark);
                m_render->DrawFilledRect(rect);
                m_render->SetDrawColor(m_colControlDarker);
                m_render->DrawLinedRect(rect);
            }

            void DrawComboBox(Gwk::Controls::Base* control, bool bIsDown, bool bIsMenuOpen) override
            {
                DrawTextBox(control);
            }

            void DrawKeyboardHighlight(Gwk::Controls::Base* control, const Gwk::Rect& r,
                                       int iOffset) override
            {
                Gwk::Rect rect = r;
                rect.x += iOffset;
                rect.y += iOffset;
                rect.w -= iOffset*2;
                rect.h -= iOffset*2;

                //draw the top and bottom
                bool skip = true;

                for (int i = 0; i < rect.w/2; i++)
                {
                    m_render->SetDrawColor(Gwk::Color(0, 0, 0, 255));

                    if (!skip)
                    {
                        m_render->DrawPixel(rect.x+(i*2), rect.y);
                        m_render->DrawPixel(rect.x+(i*2), rect.y+rect.h-1);
                    }
                    else
                    {
                        skip = !skip;
                    }
                }

                skip = false;

                for (int i = 0; i < rect.h/2; i++)
                {
                    m_render->SetDrawColor(Gwk::Color(0, 0, 0, 255));

                    if (!skip)
                    {
                        m_render->DrawPixel(rect.x, rect.y+i*2);
                        m_render->DrawPixel(rect.x+rect.w-1, rect.y+i*2);
                    }
                    else
                    {
                        skip = !skip;
                    }
                }
            }

            void DrawTooltip(Gwk::Controls::Base* control) override
            {
                Gwk::Rect rct = control->GetRenderBounds();
                rct.x -= 3;
                rct.y -= 3;
                rct.w += 6;
                rct.h += 6;
                m_render->SetDrawColor(m_colTooltipBackground);
                m_render->DrawFilledRect(rct);
                m_render->SetDrawColor(m_colTooltipBorder);
                m_render->DrawLinedRect(rct);
            }

            void DrawScrollButton(Gwk::Controls::Base* control, Position direction,
                                  bool bDepressed, bool bHovered, bool bDisabled) override
            {
                DrawButton(control, bDepressed, false, false);
                m_render->SetDrawColor(Gwk::Color(0, 0, 0, 240));
                Rect r(control->Width()/2 - 2, control->Height() / 2 - 2, 5, 5);

                if (direction == Position::Top)
                    DrawArrowUp(r);
                else if (direction == Position::Bottom)
                    DrawArrowDown(r);
                else if (direction == Position::Left)
                    DrawArrowLeft(r);
                else
                    DrawArrowRight(r);
            }

            void DrawComboDownArrow(Gwk::Controls::Base* control, bool bHovered,
                                    bool bDown, bool bOpen, bool bDisabled) override
            {
                //DrawButton( control->Width(), control->Height(), bDepressed, false, true );
                m_render->SetDrawColor(Gwk::Color(0, 0, 0, 240));
                Gwk::Rect r(control->Width()/2-2, control->Height()/2-2, 5, 5);
                DrawArrowDown(r);
            }

            void DrawNumericUpDownButton(Gwk::Controls::Base* control, bool bDepressed,
                                         bool bUp) override
            {
                //DrawButton( control->Width(), control->Height(), bDepressed, false, true );
                m_render->SetDrawColor(Gwk::Color(0, 0, 0, 240));
                Gwk::Rect r(control->Width()/2-2, control->Height()/2-2, 5, 5);

                if (bUp)
                    DrawArrowUp(r);
                else
                    DrawArrowDown(r);
            }

            void DrawTreeButton(Controls::Base* control, bool bOpen) override
            {
                Gwk::Rect rect = control->GetRenderBounds();
                rect.x += 2;
                rect.y += 2;
                rect.w -= 4;
                rect.h -= 4;
                m_render->SetDrawColor(m_colControlBright);
                m_render->DrawFilledRect(rect);
                m_render->SetDrawColor(m_colBorderColor);
                m_render->DrawLinedRect(rect);
                m_render->SetDrawColor(m_colBorderColor);

                if (!bOpen)   // ! because the button shows intention, not the current state
                    m_render->DrawFilledRect(Gwk::Rect(rect.x+rect.w/2,
                                                        rect.y+2,
                                                        1,
                                                        rect.h-4));

                m_render->DrawFilledRect(Gwk::Rect(rect.x+2,
                                                    rect.y+rect.h/2,
                                                    rect.w-4,
                                                    1));
            }

            void DrawTreeControl(Controls::Base* control) override
            {
                Gwk::Rect rect = control->GetRenderBounds();
                m_render->SetDrawColor(m_colControlBright);
                m_render->DrawFilledRect(rect);
                m_render->SetDrawColor(m_colBorderColor);
                m_render->DrawLinedRect(rect);
            }

            void DrawTreeNode(Controls::Base* ctrl, bool bOpen, bool bSelected, int iLabelHeight,
                              int iLabelWidth, int iHalfWay, int iLastBranch, bool bIsRoot) override
            {
                if (bSelected)
                {
                    GetRender()->SetDrawColor(Color(0, 150, 255, 100));
                    GetRender()->DrawFilledRect(Gwk::Rect(17, 0, iLabelWidth+2, iLabelHeight-1));
                    GetRender()->SetDrawColor(Color(0, 150, 255, 200));
                    GetRender()->DrawLinedRect(Gwk::Rect(17, 0, iLabelWidth+2, iLabelHeight-1));
                }

                Base::DrawTreeNode(ctrl, bOpen, bSelected, iLabelHeight, iLabelWidth, iHalfWay,
                                   iLastBranch, bIsRoot);
            }

            void DrawStatusBar(Controls::Base* control) override
            {
                // TODO.
            }

            void DrawColorDisplay(Controls::Base* control, Gwk::Color color) override
            {
                Gwk::Rect rect = control->GetRenderBounds();

                if (color.a != 255)
                {
                    GetRender()->SetDrawColor(Gwk::Color(255, 255, 255, 255));
                    GetRender()->DrawFilledRect(rect);
                    GetRender()->SetDrawColor(Gwk::Color(128, 128, 128, 128));
                    GetRender()->DrawFilledRect(Gwk::Rect(0, 0, rect.w/2, rect.h/2));
                    GetRender()->DrawFilledRect(Gwk::Rect(rect.w/2, rect.h/2, rect.w/2, rect.h/2));
                }

                GetRender()->SetDrawColor(color);
                GetRender()->DrawFilledRect(rect);
                GetRender()->SetDrawColor(Gwk::Color(0, 0, 0, 255));
                GetRender()->DrawLinedRect(rect);
            }

            void DrawModalControl(Controls::Base* control) override
            {
                if (control->ShouldDrawBackground())
                {
                    Gwk::Rect rect = control->GetRenderBounds();
                    GetRender()->SetDrawColor(m_colModal);
                    GetRender()->DrawFilledRect(rect);
                }
            }

            void DrawMenuDivider(Controls::Base* control) override
            {
                Gwk::Rect rect = control->GetRenderBounds();
                GetRender()->SetDrawColor(m_colBGDark);
                GetRender()->DrawFilledRect(rect);
                GetRender()->SetDrawColor(m_colControlDarker);
                GetRender()->DrawLinedRect(rect);
            }

            void DrawMenuRightArrow(Controls::Base* control) override
            {
                DrawArrowRight(control->GetRenderBounds());
            }

            void DrawSlideButton(Gwk::Controls::Base* control, bool bDepressed,
                                 bool bHorizontal) override
            {
                DrawButton(control, bDepressed, control->IsHovered(), control->IsDisabled());
            }

            void DrawCategoryHolder(Controls::Base* ctrl) override {}
            void DrawCategoryInner(Controls::Base* ctrl, bool bCollapsed) override {}
        };


    }
}


#endif // ifndef GWK_SKINS_SIMPLE_H
