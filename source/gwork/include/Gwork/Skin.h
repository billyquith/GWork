/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_SKIN_H
#define GWK_SKIN_H

#include <Gwork/BaseRender.h>
#include <Gwork/UiTypes.h>

namespace Gwk
{
    namespace Controls
    {
        class Base;
    }

    namespace Skin
    {
        namespace Symbol
        {
            enum {
                None,
                ArrowRight,
                Check,
                Dot
            };
        }

        class GWK_EXPORT Base
        {
        public:

            Base(Gwk::Renderer::Base* renderer = nullptr)
            {
                m_defaultFont.facename = "Arial";
                m_defaultFont.size = 10.0f;
                m_render = renderer;
            }

            virtual ~Base()
            {
                ReleaseFont(&m_defaultFont);
            }

            virtual void ReleaseFont(Gwk::Font* fnt)
            {
                if (!fnt)
                    return;

                if (!m_render)
                    return;

                m_render->FreeFont(fnt);
            }

            virtual void DrawGenericPanel(Controls::Base* control) = 0;

            virtual void DrawButton(Controls::Base* control, bool bDepressed, bool bHovered,
                                    bool bDisabled) = 0;
            virtual void DrawTabButton(Controls::Base* control, bool bActive, Position dir) = 0;
            virtual void DrawTabControl(Controls::Base* control) = 0;
            virtual void DrawTabTitleBar(Controls::Base* control) = 0;


            virtual void DrawMenuItem(Controls::Base* control, bool bSubmenuOpen,
                                      bool bChecked) = 0;
            virtual void DrawMenuStrip(Controls::Base* control) = 0;
            virtual void DrawMenu(Controls::Base* control, bool bPaddingDisabled) = 0;
            virtual void DrawMenuRightArrow(Controls::Base* control) = 0;

            virtual void DrawRadioButton(Controls::Base* control, bool bSelected,
                                         bool bDepressed) = 0;
            virtual void DrawCheckBox(Controls::Base* control, bool bSelected, bool bDepressed) = 0;
            virtual void DrawGroupBox(Controls::Base* control, int textStart, int textHeight,
                                      int textWidth) = 0;
            virtual void DrawTextBox(Controls::Base* control) = 0;

            virtual void DrawWindow(Controls::Base* control, int topHeight, bool inFocus) = 0;
            virtual void DrawWindowCloseButton(Gwk::Controls::Base* control, bool bDepressed,
                                               bool bHovered, bool bDisabled) = 0;
            virtual void DrawWindowMaximizeButton(Gwk::Controls::Base* control, bool bDepressed,
                                                  bool bHovered, bool bDisabled,
                                                  bool bMaximized) = 0;
            virtual void DrawWindowMinimizeButton(Gwk::Controls::Base* control, bool bDepressed,
                                                  bool bHovered, bool bDisabled) = 0;


            virtual void DrawHighlight(Controls::Base* control) = 0;
            virtual void DrawStatusBar(Controls::Base* control) = 0;

            virtual void DrawShadow(Controls::Base* control) = 0;
            virtual void DrawScrollBarBar(Controls::Base* control, bool bDepressed, bool isHovered,
                                          bool isHorizontal) = 0;
            virtual void DrawScrollBar(Controls::Base* control, bool isHorizontal,
                                       bool bDepressed) = 0;
            virtual void DrawScrollButton(Controls::Base* control, Position direction, bool bDepressed,
                                          bool bHovered, bool bDisabled) = 0;
            virtual void DrawProgressBar(Controls::Base* control, bool isHorizontal,
                                         float progress) = 0;

            virtual void DrawListBox(Controls::Base* control) = 0;
            virtual void DrawListBoxLine(Controls::Base* control, bool bSelected, bool bEven) = 0;

            virtual void DrawSlider(Controls::Base* control, bool bIsHorizontal, int numNotches,
                                    int barSize) = 0;
            virtual void DrawSlideButton(Gwk::Controls::Base* control, bool bDepressed,
                                         bool bHorizontal) = 0;

            virtual void DrawComboBox(Controls::Base* control, bool bIsDown, bool bIsMenuOpen) = 0;
            virtual void DrawComboDownArrow(Gwk::Controls::Base* control, bool bHovered,
                                            bool bDown, bool bOpen, bool bDisabled) = 0;
            virtual void DrawKeyboardHighlight(Controls::Base* control, const Gwk::Rect& rect,
                                               int offset) = 0;
            virtual void DrawTooltip(Controls::Base* control) = 0;

            virtual void DrawNumericUpDownButton(Controls::Base* control, bool bDepressed,
                                                 bool bUp) = 0;

            virtual void DrawTreeButton(Controls::Base* control, bool bOpen) = 0;
            virtual void DrawTreeControl(Controls::Base* control) = 0;
            virtual void DrawTreeNode(Controls::Base* ctrl, bool bOpen, bool bSelected,
                                      int iLabelHeight, int iLabelWidth, int iHalfWay,
                                      int iLastBranch, bool bIsRoot);

            virtual void DrawPropertyRow(Controls::Base* control, int iWidth, bool bBeingEdited,
                                         bool bHovered);
            virtual void DrawPropertyTreeNode(Controls::Base* control, int BorderLeft,
                                              int BorderTop);
            virtual void DrawColorDisplay(Controls::Base* control, Gwk::Color color) = 0;
            virtual void DrawModalControl(Controls::Base* control) = 0;
            virtual void DrawMenuDivider(Controls::Base* control) = 0;

            virtual void DrawCategoryHolder(Controls::Base* ctrl) = 0;
            virtual void DrawCategoryInner(Controls::Base* ctrl, bool bCollapsed) = 0;


            virtual void SetRender(Gwk::Renderer::Base* renderer)
            {
                m_render = renderer;
            }

            virtual Gwk::Renderer::Base* GetRender()
            {
                return m_render;
            }

            virtual void DrawArrowDown(Gwk::Rect rect);
            virtual void DrawArrowUp(Gwk::Rect rect);
            virtual void DrawArrowLeft(Gwk::Rect rect);
            virtual void DrawArrowRight(Gwk::Rect rect);
            virtual void DrawCheck(Gwk::Rect rect);


            struct
            {
                struct Window_t
                {
                    Gwk::Color TitleActive;
                    Gwk::Color TitleInactive;

                } Window;


                struct Label_t
                {
                    Gwk::Color Default;
                    Gwk::Color Bright;
                    Gwk::Color Dark;
                    Gwk::Color Highlight;

                } Label;


                struct Tree_t
                {
                    Gwk::Color Lines;
                    Gwk::Color Normal;
                    Gwk::Color Hover;
                    Gwk::Color Selected;

                } Tree;


                struct Properties_t
                {
                    Gwk::Color Line_Normal;
                    Gwk::Color Line_Selected;
                    Gwk::Color Line_Hover;
                    Gwk::Color Column_Normal;
                    Gwk::Color Column_Selected;
                    Gwk::Color Column_Hover;
                    Gwk::Color Label_Normal;
                    Gwk::Color Label_Selected;
                    Gwk::Color Label_Hover;
                    Gwk::Color Border;
                    Gwk::Color Title;

                } Properties;


                struct Button_t
                {
                    Gwk::Color Normal;
                    Gwk::Color Hover;
                    Gwk::Color Down;
                    Gwk::Color Disabled;

                } Button;


                struct Tab_t
                {
                    struct Active_t
                    {
                        Gwk::Color Normal;
                        Gwk::Color Hover;
                        Gwk::Color Down;
                        Gwk::Color Disabled;
                    } Active;


                    struct Inactive_t
                    {
                        Gwk::Color Normal;
                        Gwk::Color Hover;
                        Gwk::Color Down;
                        Gwk::Color Disabled;
                    } Inactive;


                } Tab;


                struct Category_t
                {
                    Gwk::Color Header;
                    Gwk::Color Header_Closed;

                    struct Line_t
                    {
                        Gwk::Color Text;
                        Gwk::Color Text_Hover;
                        Gwk::Color Text_Selected;
                        Gwk::Color Button;
                        Gwk::Color Button_Hover;
                        Gwk::Color Button_Selected;
                    } Line;


                    struct LineAlt_t
                    {
                        Gwk::Color Text;
                        Gwk::Color Text_Hover;
                        Gwk::Color Text_Selected;
                        Gwk::Color Button;
                        Gwk::Color Button_Hover;
                        Gwk::Color Button_Selected;
                    } LineAlt;


                } Category;


                Gwk::Color ModalBackground;
                Gwk::Color TooltipText;

            } Colors;


        public:

            virtual Gwk::Font* GetDefaultFont()
            {
                return &m_defaultFont;
            }

            virtual void SetDefaultFont(const Gwk::String& strFacename, float fSize = 10.0f)
            {
                m_defaultFont.facename = strFacename;
                m_defaultFont.size = fSize;
            }

        protected:

            Gwk::Font m_defaultFont;
            Gwk::Renderer::Base* m_render;

        };


    }
}
#endif // ifndef GWK_SKIN_H
