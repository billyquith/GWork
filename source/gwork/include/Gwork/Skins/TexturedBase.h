/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_SKINS_TEXTUREDBASE_H
#define GWK_SKINS_TEXTUREDBASE_H

#include <Gwork/Skin.h>
#include <Gwork/Gwork.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/Skins/Texturing.h>

namespace Gwk
{
    namespace Skin
    {
        class TexturedBase : public Gwk::Skin::Base
        {
        public:

            TexturedBase( Gwk::Renderer::Base* renderer )
            :   Gwk::Skin::Base( renderer )
            {}

            ~TexturedBase()
            {
                m_render->FreeTexture(&m_texture);
            }

            Texture m_texture;

            struct Textures_t
            {
                Texturing::Bordered StatusBar;
                Texturing::Bordered Selection;
                Texturing::Bordered Shadow;
                Texturing::Bordered Tooltip;

                struct Panel_t
                {
                    Texturing::Bordered Normal;
                    Texturing::Bordered Bright;
                    Texturing::Bordered Dark;
                    Texturing::Bordered Highlight;

                } Panel;

                struct Window_t
                {
                    Texturing::Bordered Normal;
                    Texturing::Bordered Inactive;
                    Texturing::Single Close;
                    Texturing::Single Close_Hover;
                    Texturing::Single Close_Down;
                    Texturing::Single Maxi;
                    Texturing::Single Maxi_Hover;
                    Texturing::Single Maxi_Down;
                    Texturing::Single Mini;
                    Texturing::Single Mini_Hover;
                    Texturing::Single Mini_Down;
                    Texturing::Single Restore;
                    Texturing::Single Restore_Hover;
                    Texturing::Single Restore_Down;

                } Window;

                struct Checkbox_t
                {
                    struct Active_t
                    {
                        Texturing::Single Normal;
                        Texturing::Single Checked;

                    } Active;

                    struct Disabled_t
                    {
                        Texturing::Single Normal;
                        Texturing::Single Checked;

                    } Disabled;

                } Checkbox;

                struct RadioButton_t
                {
                    struct Active_t
                    {
                        Texturing::Single Normal;
                        Texturing::Single Checked;

                    } Active;

                    struct Disabled_t
                    {
                        Texturing::Single Normal;
                        Texturing::Single Checked;

                    } Disabled;

                } RadioButton;

                struct TextBox_t
                {
                    Texturing::Bordered Normal;
                    Texturing::Bordered Focus;
                    Texturing::Bordered Disabled;

                } TextBox;

                struct Tree_t
                {
                    Texturing::Bordered Background;
                    Texturing::Single Minus;
                    Texturing::Single Plus;

                } Tree;


                struct ProgressBar_t
                {
                    Texturing::Bordered Back;
                    Texturing::Bordered Front;

                } ProgressBar;

                struct Scroller_t
                {
                    Texturing::Bordered TrackV;
                    Texturing::Bordered ButtonV_Normal;
                    Texturing::Bordered ButtonV_Hover;
                    Texturing::Bordered ButtonV_Down;
                    Texturing::Bordered ButtonV_Disabled;
                    Texturing::Bordered TrackH;
                    Texturing::Bordered ButtonH_Normal;
                    Texturing::Bordered ButtonH_Hover;
                    Texturing::Bordered ButtonH_Down;
                    Texturing::Bordered ButtonH_Disabled;

                    struct Button_t
                    {
                        Texturing::Bordered Normal[4];
                        Texturing::Bordered Hover[4];
                        Texturing::Bordered Down[4];
                        Texturing::Bordered Disabled[4];

                    } Button;

                } Scroller;

                struct Menu_t
                {
                    Texturing::Single RightArrow;
                    Texturing::Single Check;

                    Texturing::Bordered Strip;
                    Texturing::Bordered Background;
                    Texturing::Bordered BackgroundWithMargin;
                    Texturing::Bordered Hover;

                } Menu;

                struct Input_t
                {
                    struct Button_t
                    {
                        Texturing::Bordered Normal;
                        Texturing::Bordered Hovered;
                        Texturing::Bordered Disabled;
                        Texturing::Bordered Pressed;

                    } Button;

                    struct ListBox_t
                    {
                        Texturing::Bordered Background;
                        Texturing::Bordered Hovered;

                        Texturing::Bordered EvenLine;
                        Texturing::Bordered OddLine;
                        Texturing::Bordered EvenLineSelected;
                        Texturing::Bordered OddLineSelected;

                    } ListBox;

                    struct UpDown_t
                    {
                        struct Up_t
                        {
                            Texturing::Single Normal;
                            Texturing::Single Hover;
                            Texturing::Single Down;
                            Texturing::Single Disabled;

                        } Up;

                        struct Down_t
                        {
                            Texturing::Single Normal;
                            Texturing::Single Hover;
                            Texturing::Single Down;
                            Texturing::Single Disabled;

                        } Down;

                    } UpDown;

                    struct ComboBox_t
                    {
                        Texturing::Bordered Normal;
                        Texturing::Bordered Hover;
                        Texturing::Bordered Down;
                        Texturing::Bordered Disabled;

                        struct Button_t
                        {
                            Texturing::Single Normal;
                            Texturing::Single Hover;
                            Texturing::Single Down;
                            Texturing::Single Disabled;

                        } Button;

                    } ComboBox;

                    struct Slider_t
                    {
                        struct H_t
                        {
                            Texturing::Single Normal;
                            Texturing::Single Hover;
                            Texturing::Single Down;
                            Texturing::Single Disabled;
                        } H;

                        struct V_t
                        {
                            Texturing::Single Normal;
                            Texturing::Single Hover;
                            Texturing::Single Down;
                            Texturing::Single Disabled;
                        } V;

                    } Slider;

                } Input;

                struct Tab_t
                {
                    struct Bottom_t
                    {
                        Texturing::Bordered Active;
                        Texturing::Bordered Inactive;
                    } Bottom;

                    struct Top_t
                    {
                        Texturing::Bordered Active;
                        Texturing::Bordered Inactive;
                    } Top;

                    struct Left_t
                    {
                        Texturing::Bordered Active;
                        Texturing::Bordered Inactive;
                    } Left;

                    struct Right_t
                    {
                        Texturing::Bordered Active;
                        Texturing::Bordered Inactive;
                    } Right;

                    Texturing::Bordered Control;
                    Texturing::Bordered HeaderBar;

                } Tab;

                struct CategoryList_t
                {
                    Texturing::Bordered Outer;
                    Texturing::Bordered Inner;
                    Texturing::Bordered Header;

                } CategoryList;

                Texturing::Bordered GroupBox;

            } Textures;


            virtual void Init( const String & TextureName )
            {
                m_defaultFont.facename  = "Microsoft Sans Serif";
                m_defaultFont.size      = 11;

                m_texture.name = TextureName;
                m_texture.readable = true;
                GetRender()->LoadTexture(&m_texture); // Readable texture

                Colors.Window.TitleActive   = GetRender()->PixelColour( &m_texture, 4 + 8 * 0, 508, Color( 255, 0, 0 ) );
                Colors.Window.TitleInactive = GetRender()->PixelColour( &m_texture, 4 + 8 * 1, 508, Color( 255, 255, 0 ) );
                Colors.Button.Normal        = GetRender()->PixelColour( &m_texture, 4 + 8 * 2, 508, Color( 255, 255, 0 ) );
                Colors.Button.Hover         = GetRender()->PixelColour( &m_texture, 4 + 8 * 3, 508, Color( 255, 255, 0 ) );
                Colors.Button.Down          = GetRender()->PixelColour( &m_texture, 4 + 8 * 2, 500, Color( 255, 255, 0 ) );
                Colors.Button.Disabled      = GetRender()->PixelColour( &m_texture, 4 + 8 * 3, 500, Color( 255, 255, 0 ) );
                Colors.Tab.Active.Normal        = GetRender()->PixelColour( &m_texture, 4 + 8 * 4, 508, Color( 255, 255, 0 ) );
                Colors.Tab.Active.Hover         = GetRender()->PixelColour( &m_texture, 4 + 8 * 5, 508, Color( 255, 255, 0 ) );
                Colors.Tab.Active.Down          = GetRender()->PixelColour( &m_texture, 4 + 8 * 4, 500, Color( 255, 255, 0 ) );
                Colors.Tab.Active.Disabled      = GetRender()->PixelColour( &m_texture, 4 + 8 * 5, 500, Color( 255, 255, 0 ) );
                Colors.Tab.Inactive.Normal      = GetRender()->PixelColour( &m_texture, 4 + 8 * 6, 508, Color( 255, 255, 0 ) );
                Colors.Tab.Inactive.Hover       = GetRender()->PixelColour( &m_texture, 4 + 8 * 7, 508, Color( 255, 255, 0 ) );
                Colors.Tab.Inactive.Down        = GetRender()->PixelColour( &m_texture, 4 + 8 * 6, 500, Color( 255, 255, 0 ) );
                Colors.Tab.Inactive.Disabled    = GetRender()->PixelColour( &m_texture, 4 + 8 * 7, 500, Color( 255, 255, 0 ) );
                Colors.Label.Default            = GetRender()->PixelColour( &m_texture, 4 + 8 * 8, 508, Color( 255, 255, 0 ) );
                Colors.Label.Bright             = GetRender()->PixelColour( &m_texture, 4 + 8 * 9, 508, Color( 255, 255, 0 ) );
                Colors.Label.Dark               = GetRender()->PixelColour( &m_texture, 4 + 8 * 8, 500, Color( 255, 255, 0 ) );
                Colors.Label.Highlight          = GetRender()->PixelColour( &m_texture, 4 + 8 * 9, 500, Color( 255, 255, 0 ) );
                Colors.Tree.Lines               = GetRender()->PixelColour( &m_texture, 4 + 8 * 10, 508, Color( 255, 255, 0 ) );
                Colors.Tree.Normal              = GetRender()->PixelColour( &m_texture, 4 + 8 * 11, 508, Color( 255, 255, 0 ) );
                Colors.Tree.Hover               = GetRender()->PixelColour( &m_texture, 4 + 8 * 10, 500, Color( 255, 255, 0 ) );
                Colors.Tree.Selected            = GetRender()->PixelColour( &m_texture, 4 + 8 * 11, 500, Color( 255, 255, 0 ) );
                Colors.Properties.Line_Normal       = GetRender()->PixelColour( &m_texture, 4 + 8 * 12, 508, Color( 255, 255, 0 ) );
                Colors.Properties.Line_Selected     = GetRender()->PixelColour( &m_texture, 4 + 8 * 13, 508, Color( 255, 255, 0 ) );
                Colors.Properties.Line_Hover        = GetRender()->PixelColour( &m_texture, 4 + 8 * 12, 500, Color( 255, 255, 0 ) );
                Colors.Properties.Title             = GetRender()->PixelColour( &m_texture, 4 + 8 * 13, 500, Color( 255, 255, 0 ) );
                Colors.Properties.Column_Normal     = GetRender()->PixelColour( &m_texture, 4 + 8 * 14, 508, Color( 255, 255, 0 ) );
                Colors.Properties.Column_Selected   = GetRender()->PixelColour( &m_texture, 4 + 8 * 15, 508, Color( 255, 255, 0 ) );
                Colors.Properties.Column_Hover      = GetRender()->PixelColour( &m_texture, 4 + 8 * 14, 500, Color( 255, 255, 0 ) );
                Colors.Properties.Border            = GetRender()->PixelColour( &m_texture, 4 + 8 * 15, 500, Color( 255, 255, 0 ) );
                Colors.Properties.Label_Normal      = GetRender()->PixelColour( &m_texture, 4 + 8 * 16, 508, Color( 255, 255, 0 ) );
                Colors.Properties.Label_Selected    = GetRender()->PixelColour( &m_texture, 4 + 8 * 17, 508, Color( 255, 255, 0 ) );
                Colors.Properties.Label_Hover       = GetRender()->PixelColour( &m_texture, 4 + 8 * 16, 500, Color( 255, 255, 0 ) );
                Colors.ModalBackground              = GetRender()->PixelColour( &m_texture, 4 + 8 * 18, 508, Color( 255, 255, 0 ) );
                Colors.TooltipText                  = GetRender()->PixelColour( &m_texture, 4 + 8 * 19, 508, Color( 255, 255, 0 ) );
                Colors.Category.Header              = GetRender()->PixelColour( &m_texture, 4 + 8 * 18, 500, Color( 255, 255, 0 ) );
                Colors.Category.Header_Closed       = GetRender()->PixelColour( &m_texture, 4 + 8 * 19, 500, Color( 255, 255, 0 ) );
                Colors.Category.Line.Text           = GetRender()->PixelColour( &m_texture, 4 + 8 * 20, 508, Color( 255, 255, 0 ) );
                Colors.Category.Line.Text_Hover     = GetRender()->PixelColour( &m_texture, 4 + 8 * 21, 508, Color( 255, 255, 0 ) );
                Colors.Category.Line.Text_Selected  = GetRender()->PixelColour( &m_texture, 4 + 8 * 20, 500, Color( 255, 255, 0 ) );
                Colors.Category.Line.Button         = GetRender()->PixelColour( &m_texture, 4 + 8 * 21, 500, Color( 255, 255, 0 ) );
                Colors.Category.Line.Button_Hover   = GetRender()->PixelColour( &m_texture, 4 + 8 * 22, 508, Color( 255, 255, 0 ) );
                Colors.Category.Line.Button_Selected = GetRender()->PixelColour( &m_texture, 4 + 8 * 23, 508, Color( 255, 255, 0 ) );
                Colors.Category.LineAlt.Text            = GetRender()->PixelColour( &m_texture, 4 + 8 * 22, 500, Color( 255, 255, 0 ) );
                Colors.Category.LineAlt.Text_Hover      = GetRender()->PixelColour( &m_texture, 4 + 8 * 23, 500, Color( 255, 255, 0 ) );
                Colors.Category.LineAlt.Text_Selected   = GetRender()->PixelColour( &m_texture, 4 + 8 * 24, 508, Color( 255, 255, 0 ) );
                Colors.Category.LineAlt.Button          = GetRender()->PixelColour( &m_texture, 4 + 8 * 25, 508, Color( 255, 255, 0 ) );
                Colors.Category.LineAlt.Button_Hover    = GetRender()->PixelColour( &m_texture, 4 + 8 * 24, 500, Color( 255, 255, 0 ) );
                Colors.Category.LineAlt.Button_Selected = GetRender()->PixelColour( &m_texture, 4 + 8 * 25, 500, Color( 255, 255, 0 ) );
                Textures.Shadow.Init( &m_texture, 448, 0,   31,     31,     Margin( 8, 8, 8, 8 ) );
                Textures.Tooltip.Init( &m_texture, 128, 320,    127,    31,     Margin( 8, 8, 8, 8 ) );
                Textures.StatusBar.Init( &m_texture, 128, 288, 127, 31, Margin( 8, 8, 8, 8 ) );
                Textures.Selection.Init( &m_texture, 384, 32, 31, 31, Margin( 4, 4, 4, 4 ) );
                Textures.Panel.Normal.Init( &m_texture, 256,        0,  63, 63,     Margin( 16, 16, 16, 16 ) );
                Textures.Panel.Bright.Init( &m_texture, 256 + 64,   0,  63, 63,     Margin( 16, 16, 16, 16 ) );
                Textures.Panel.Dark.Init( &m_texture, 256,      64, 63, 63,     Margin( 16, 16, 16, 16 ) );
                Textures.Panel.Highlight.Init( &m_texture, 256 + 64,    64, 63, 63,     Margin( 16, 16, 16, 16 ) );
                Textures.Window.Normal.Init( &m_texture, 0, 0, 127,     127,    Margin( 8, 32, 8, 8 ) );
                Textures.Window.Inactive.Init( &m_texture, 128, 0, 127,     127,    Margin( 8, 32, 8, 8 ) );
                Textures.Checkbox.Active.Checked.Init( &m_texture, 448, 32, 15, 15 );
                Textures.Checkbox.Active.Normal.Init( &m_texture, 464, 32, 15, 15 );
                Textures.Checkbox.Disabled.Checked.Init( &m_texture, 448, 48, 15, 15 );
                Textures.Checkbox.Disabled.Normal.Init( &m_texture, 464, 48, 15, 15 );
                Textures.RadioButton.Active.Checked.Init( &m_texture, 448, 64, 15, 15 );
                Textures.RadioButton.Active.Normal.Init( &m_texture, 464, 64, 15, 15 );
                Textures.RadioButton.Disabled.Checked.Init( &m_texture, 448, 80, 15, 15 );
                Textures.RadioButton.Disabled.Normal.Init( &m_texture, 464, 80, 15, 15 );
                Textures.TextBox.Normal.Init( &m_texture, 0, 150, 127, 21, Margin( 4, 4, 4, 4 ) );
                Textures.TextBox.Focus.Init( &m_texture, 0, 172, 127, 21, Margin( 4, 4, 4, 4 ) );
                Textures.TextBox.Disabled.Init( &m_texture, 0, 193, 127, 21, Margin( 4, 4, 4, 4 ) );
                Textures.Menu.Strip.Init( &m_texture, 0, 128, 127, 21, Margin( 1, 1, 1, 1 ) );
                Textures.Menu.BackgroundWithMargin.Init( &m_texture, 128, 128, 127, 63, Margin( 24, 8, 8, 8 ) );
                Textures.Menu.Background.Init( &m_texture, 128, 192, 127, 63, Margin( 8, 8, 8, 8 ) );
                Textures.Menu.Hover.Init( &m_texture, 128, 256, 127, 31, Margin( 8, 8, 8, 8 ) );
                Textures.Menu.RightArrow.Init( &m_texture, 464, 112, 15, 15 );
                Textures.Menu.Check.Init( &m_texture, 448, 112, 15, 15 );
                Textures.Tab.Control.Init( &m_texture, 0, 256, 127, 127, Margin( 8, 8, 8, 8 ) );
                Textures.Tab.Bottom.Active.Init( &m_texture, 0,     416, 63, 31, Margin( 8, 8, 8, 8 ) );
                Textures.Tab.Bottom.Inactive.Init( &m_texture, 0 + 128, 416, 63, 31, Margin( 8, 8, 8, 8 ) );
                Textures.Tab.Top.Active.Init( &m_texture, 0,        384, 63, 31, Margin( 8, 8, 8, 8 ) );
                Textures.Tab.Top.Inactive.Init( &m_texture, 0 + 128,    384, 63, 31, Margin( 8, 8, 8, 8 ) );
                Textures.Tab.Left.Active.Init( &m_texture, 64,      384, 31, 63, Margin( 8, 8, 8, 8 ) );
                Textures.Tab.Left.Inactive.Init( &m_texture, 64 + 128,  384, 31, 63, Margin( 8, 8, 8, 8 ) );
                Textures.Tab.Right.Active.Init( &m_texture, 96,     384, 31, 63, Margin( 8, 8, 8, 8 ) );
                Textures.Tab.Right.Inactive.Init( &m_texture, 96 + 128, 384, 31, 63, Margin( 8, 8, 8, 8 ) );
                Textures.Tab.HeaderBar.Init( &m_texture, 128, 352, 127, 31, Margin( 4, 4, 4, 4 ) );
                Textures.Window.Close.Init( &m_texture, 32, 448, 31, 31 );
                Textures.Window.Close_Hover.Init( &m_texture, 64, 448, 31, 31 );
                Textures.Window.Close_Down.Init( &m_texture, 96, 448, 31, 31 );
                Textures.Window.Maxi.Init( &m_texture, 32 + 96 * 2, 448, 31, 31 );
                Textures.Window.Maxi_Hover.Init( &m_texture, 64 + 96 * 2, 448, 31, 31 );
                Textures.Window.Maxi_Down.Init( &m_texture, 96 + 96 * 2, 448, 31, 31 );
                Textures.Window.Restore.Init( &m_texture, 32 + 96 * 2, 448 + 32, 31, 31 );
                Textures.Window.Restore_Hover.Init( &m_texture, 64 + 96 * 2, 448 + 32, 31, 31 );
                Textures.Window.Restore_Down.Init( &m_texture, 96 + 96 * 2, 448 + 32, 31, 31 );
                Textures.Window.Mini.Init( &m_texture, 32 + 96, 448, 31, 31 );
                Textures.Window.Mini_Hover.Init( &m_texture, 64 + 96, 448, 31, 31 );
                Textures.Window.Mini_Down.Init( &m_texture, 96 + 96, 448, 31, 31 );
                Textures.Tree.Background.Init( &m_texture, 256, 128, 127,   127,    Margin( 16, 16, 16, 16 ) );
                Textures.Tree.Plus.Init( &m_texture, 448, 96, 15, 15 );
                Textures.Tree.Minus.Init( &m_texture, 464, 96, 15, 15 );
                Textures.Input.Button.Normal.Init( &m_texture, 480, 0,  31,     31,     Margin( 8, 8, 8, 8 ) );
                Textures.Input.Button.Hovered.Init( &m_texture, 480, 32,    31,     31,     Margin( 8, 8, 8, 8 ) );
                Textures.Input.Button.Disabled.Init( &m_texture, 480, 64,   31,     31,     Margin( 8, 8, 8, 8 ) );
                Textures.Input.Button.Pressed.Init( &m_texture, 480, 96,    31,     31,     Margin( 8, 8, 8, 8 ) );

                for ( int i = 0; i < 4; i++ )
                {
                    Textures.Scroller.Button.Normal[i].Init( &m_texture, 464 + 0, 208 + i * 16, 15, 15, Margin( 2, 2, 2, 2 ) );
                    Textures.Scroller.Button.Hover[i].Init( &m_texture, 480, 208 + i * 16, 15, 15, Margin( 2, 2, 2, 2 ) );
                    Textures.Scroller.Button.Down[i].Init( &m_texture, 464, 272 + i * 16, 15, 15, Margin( 2, 2, 2, 2 ) );
                    Textures.Scroller.Button.Disabled[i].Init( &m_texture, 480 + 48, 272 + i * 16, 15, 15, Margin( 2, 2, 2, 2 ) );
                }

                Textures.Scroller.TrackV.Init( &m_texture, 384,         208, 15, 127, Margin( 4, 4, 4, 4 ) );
                Textures.Scroller.ButtonV_Normal.Init( &m_texture, 384 + 16,        208, 15, 127, Margin( 4, 4, 4, 4 ) );
                Textures.Scroller.ButtonV_Hover.Init( &m_texture, 384 + 32,     208, 15, 127, Margin( 4, 4, 4, 4 ) );
                Textures.Scroller.ButtonV_Down.Init( &m_texture, 384 + 48,      208, 15, 127, Margin( 4, 4, 4, 4 ) );
                Textures.Scroller.ButtonV_Disabled.Init( &m_texture, 384 + 64,      208, 15, 127, Margin( 4, 4, 4, 4 ) );
                Textures.Scroller.TrackH.Init( &m_texture, 384, 128,        127, 15, Margin( 4, 4, 4, 4 ) );
                Textures.Scroller.ButtonH_Normal.Init( &m_texture, 384, 128 + 16,   127, 15, Margin( 4, 4, 4, 4 ) );
                Textures.Scroller.ButtonH_Hover.Init( &m_texture, 384,  128 + 32,   127, 15, Margin( 4, 4, 4, 4 ) );
                Textures.Scroller.ButtonH_Down.Init( &m_texture, 384,   128 + 48,   127, 15, Margin( 4, 4, 4, 4 ) );
                Textures.Scroller.ButtonH_Disabled.Init( &m_texture, 384,   128 + 64,   127, 15, Margin( 4, 4, 4, 4 ) );
                Textures.Input.ListBox.Background.Init( &m_texture, 256,    256, 63, 127, Margin( 8, 8, 8, 8 ) );
                Textures.Input.ListBox.Hovered.Init( &m_texture, 320,   320, 31, 31, Margin( 8, 8, 8, 8 ) );
                Textures.Input.ListBox.EvenLine.Init( &m_texture, 352,  256, 31, 31, Margin( 8, 8, 8, 8 ) );
                Textures.Input.ListBox.OddLine.Init( &m_texture, 352,  288, 31, 31, Margin( 8, 8, 8, 8 ) );
                Textures.Input.ListBox.EvenLineSelected.Init( &m_texture, 320,  256, 31, 31, Margin( 8, 8, 8, 8 ) );
                Textures.Input.ListBox.OddLineSelected.Init( &m_texture, 320,   288, 31, 31, Margin( 8, 8, 8, 8 ) );
                Textures.Input.ComboBox.Normal.Init( &m_texture, 384,   336,    127, 31, Margin( 8, 8, 32, 8 ) );
                Textures.Input.ComboBox.Hover.Init( &m_texture, 384,    336 + 32, 127, 31, Margin( 8, 8, 32, 8 ) );
                Textures.Input.ComboBox.Down.Init( &m_texture, 384, 336 + 64, 127, 31, Margin( 8, 8, 32, 8 ) );
                Textures.Input.ComboBox.Disabled.Init( &m_texture, 384, 336 + 96, 127, 31, Margin( 8, 8, 32, 8 ) );
                Textures.Input.ComboBox.Button.Normal.Init( &m_texture, 496,    272,    15, 15 );
                Textures.Input.ComboBox.Button.Hover.Init( &m_texture, 496, 272 + 16, 15, 15 );
                Textures.Input.ComboBox.Button.Down.Init( &m_texture, 496,  272 + 32, 15, 15 );
                Textures.Input.ComboBox.Button.Disabled.Init( &m_texture, 496,  272 + 48, 15, 15 );
                Textures.Input.UpDown.Up.Normal.Init( &m_texture, 384,      112,    7, 7 );
                Textures.Input.UpDown.Up.Hover.Init( &m_texture, 384 + 8,   112,    7, 7 );
                Textures.Input.UpDown.Up.Down.Init( &m_texture, 384 + 16,   112,    7, 7 );
                Textures.Input.UpDown.Up.Disabled.Init( &m_texture, 384 + 24,   112,    7, 7 );
                Textures.Input.UpDown.Down.Normal.Init( &m_texture, 384,        120,    7, 7 );
                Textures.Input.UpDown.Down.Hover.Init( &m_texture, 384 + 8, 120,    7, 7 );
                Textures.Input.UpDown.Down.Down.Init( &m_texture, 384 + 16, 120,    7, 7 );
                Textures.Input.UpDown.Down.Disabled.Init( &m_texture, 384 + 24, 120,    7, 7 );
                Textures.ProgressBar.Back.Init( &m_texture, 384,    0, 31, 31, Margin( 8, 8, 8, 8 ) );
                Textures.ProgressBar.Front.Init( &m_texture, 384 + 32,  0, 31, 31, Margin( 8, 8, 8, 8 ) );
                Textures.Input.Slider.H.Normal.Init( &m_texture, 416,   32, 15, 15 );
                Textures.Input.Slider.H.Hover.Init( &m_texture, 416,    32 + 16, 15, 15 );
                Textures.Input.Slider.H.Down.Init( &m_texture, 416, 32 + 32, 15, 15 );
                Textures.Input.Slider.H.Disabled.Init( &m_texture, 416, 32 + 48, 15, 15 );
                Textures.Input.Slider.V.Normal.Init( &m_texture, 416 + 16,  32, 15, 15 );
                Textures.Input.Slider.V.Hover.Init( &m_texture, 416 + 16,   32 + 16, 15, 15 );
                Textures.Input.Slider.V.Down.Init( &m_texture, 416 + 16,    32 + 32, 15, 15 );
                Textures.Input.Slider.V.Disabled.Init( &m_texture, 416 + 16,    32 + 48, 15, 15 );
                Textures.CategoryList.Outer.Init( &m_texture, 256,          384, 63, 63, Margin( 8, 8, 8, 8 ) );
                Textures.CategoryList.Inner.Init( &m_texture, 256 + 64, 384, 63, 63, Margin( 8, 21, 8, 8 ) );
                Textures.CategoryList.Header.Init( &m_texture, 320,         352, 63, 31, Margin( 8, 8, 8, 8 ) );
                Textures.GroupBox.Init( &m_texture, 0,          448, 31, 31, Margin( 8, 8, 8, 8 ) );
            }

            void DrawButton( Gwk::Controls::Base* control, bool bDepressed, bool bHovered, bool bDisabled ) override
            {
                if ( bDisabled )    { return Textures.Input.Button.Disabled.Draw( GetRender(), control->GetRenderBounds() ); }

                if ( bDepressed )   { return Textures.Input.Button.Pressed.Draw( GetRender(), control->GetRenderBounds() ); }

                if ( bHovered )     { return Textures.Input.Button.Hovered.Draw( GetRender(), control->GetRenderBounds() ); }

                Textures.Input.Button.Normal.Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawMenuItem( Gwk::Controls::Base* control, bool bSubmenuOpen, bool bChecked ) override
            {
                const Gwk::Rect & rect = control->GetRenderBounds();

                if ( bSubmenuOpen || control->IsHovered() ) { Textures.Menu.Hover.Draw( GetRender(), rect ); }

                if ( bChecked ) { Textures.Menu.Check.Draw( GetRender(), Gwk::Rect( rect.x + 4, rect.y + 3, 15, 15 ) ); }
            }

            void DrawMenuStrip( Gwk::Controls::Base* control ) override
            {
                Textures.Menu.Strip.Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawMenu( Gwk::Controls::Base* control, bool bPaddingDisabled ) override
            {
                if ( !bPaddingDisabled )
                {
                    return Textures.Menu.BackgroundWithMargin.Draw( GetRender(), control->GetRenderBounds() );
                }

                Textures.Menu.Background.Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawMenuRightArrow( Controls::Base* control ) override
            {
                Textures.Menu.RightArrow.Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawShadow( Gwk::Controls::Base* control ) override
            {
                Gwk::Rect r = control->GetRenderBounds();
                r.x -= 4;
                r.y -= 4;
                r.w += 10;
                r.h += 10;
                Textures.Shadow.Draw( GetRender(), r );
            }

            void DrawRadioButton( Gwk::Controls::Base* control, bool bSelected, bool bDepressed ) override
            {
                if ( bSelected )
                {
                    if ( control->IsDisabled() )
                    { Textures.RadioButton.Disabled.Checked.Draw( GetRender(), control->GetRenderBounds() ); }
                    else
                    { Textures.RadioButton.Active.Checked.Draw( GetRender(), control->GetRenderBounds() ); }
                }
                else
                {
                    if ( control->IsDisabled() )
                    { Textures.RadioButton.Disabled.Normal.Draw( GetRender(), control->GetRenderBounds() ); }
                    else
                    { Textures.RadioButton.Active.Normal.Draw( GetRender(), control->GetRenderBounds() ); }
                }
            }

            void DrawCheckBox( Gwk::Controls::Base* control, bool bSelected, bool bDepressed ) override
            {
                if ( bSelected )
                {
                    if ( control->IsDisabled() )
                    { Textures.Checkbox.Disabled.Checked.Draw( GetRender(), control->GetRenderBounds() ); }
                    else
                    { Textures.Checkbox.Active.Checked.Draw( GetRender(), control->GetRenderBounds() ); }
                }
                else
                {
                    if ( control->IsDisabled() )
                    { Textures.Checkbox.Disabled.Normal.Draw( GetRender(), control->GetRenderBounds() ); }
                    else
                    { Textures.Checkbox.Active.Normal.Draw( GetRender(), control->GetRenderBounds() ); }
                }
            }

            void DrawGroupBox( Gwk::Controls::Base* control, int textStart, int textHeight, int textWidth ) override
            {
                Gwk::Rect rect = control->GetRenderBounds();
                rect.y += textHeight/2;
                rect.h -= textHeight/2;
                Textures.GroupBox.Draw( GetRender(), rect, Gwk::Colors::White, ~(1<<1) );
                rect.x += textStart + textWidth - 4;
                rect.w -= textStart + textWidth - 4;
                Textures.GroupBox.Draw( GetRender(), rect, Gwk::Colors::White, (1<<1) );
            }

            void DrawTextBox( Gwk::Controls::Base* control ) override
            {
                if ( control->IsDisabled() )
                { return Textures.TextBox.Disabled.Draw( GetRender(), control->GetRenderBounds() ); }

                if ( control->IsFocussed() )
                { Textures.TextBox.Focus.Draw( GetRender(), control->GetRenderBounds() ); }
                else
                { Textures.TextBox.Normal.Draw( GetRender(), control->GetRenderBounds() ); }
            }

            virtual void DrawActiveTabButton( Gwk::Controls::Base* control, Position dir )
            {
                if ( dir == Position::Bottom )
                    Textures.Tab.Bottom.Active.Draw( GetRender(), control->GetRenderBounds() + Rect( 0, -8, 0, 8 ) );
                else if ( dir == Position::Top )
                    Textures.Tab.Top.Active.Draw( GetRender(), control->GetRenderBounds() + Rect( 0, 0, 0, 8 ) );
                else if ( dir == Position::Left )
                    Textures.Tab.Left.Active.Draw( GetRender(), control->GetRenderBounds() + Rect( 0, 0, 8, 0 ) );
                else if ( dir == Position::Right )
                    Textures.Tab.Right.Active.Draw( GetRender(), control->GetRenderBounds() + Rect( -8, 0, 8, 0 ) );
            }

            void DrawTabButton( Gwk::Controls::Base* control, bool bActive, Position dir ) override
            {
                if ( bActive )
                {
                    DrawActiveTabButton( control, dir );
                    return;
                }

                if ( dir == Position::Bottom )
                    Textures.Tab.Bottom.Inactive.Draw( GetRender(), control->GetRenderBounds() );
                else if ( dir == Position::Top )
                    Textures.Tab.Top.Inactive.Draw( GetRender(), control->GetRenderBounds() );
                else if ( dir == Position::Left )
                    Textures.Tab.Left.Inactive.Draw( GetRender(), control->GetRenderBounds() );
                else if ( dir == Position::Right )
                    Textures.Tab.Right.Inactive.Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawTabControl( Gwk::Controls::Base* control ) override
            {
                Textures.Tab.Control.Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawTabTitleBar( Gwk::Controls::Base* control ) override
            {
                Textures.Tab.HeaderBar.Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawGenericPanel( Controls::Base* control ) override
            {
                Textures.Panel.Normal.Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawWindow( Gwk::Controls::Base* control, int topHeight, bool inFocus ) override
            {
                if ( inFocus ) { Textures.Window.Normal.Draw( GetRender(), control->GetRenderBounds() ); }
                else { Textures.Window.Inactive.Draw( GetRender(), control->GetRenderBounds() ); }
            }

            void DrawHighlight( Gwk::Controls::Base* control ) override
            {
                Gwk::Rect rect = control->GetRenderBounds();
                GetRender()->SetDrawColor( Gwk::Color( 255, 100, 255, 255 ) );
                GetRender()->DrawFilledRect( rect );
            }

            void DrawScrollBar( Gwk::Controls::Base* control, bool isHorizontal, bool bDepressed ) override
            {
                if ( isHorizontal )
                { Textures.Scroller.TrackH.Draw( GetRender(), control->GetRenderBounds() ); }
                else
                { Textures.Scroller.TrackV.Draw( GetRender(), control->GetRenderBounds() ); }
            }

            void DrawScrollBarBar( Controls::Base* control, bool bDepressed, bool isHovered, bool isHorizontal ) override
            {
                if ( !isHorizontal )
                {
                    if ( control->IsDisabled() )
                    { return Textures.Scroller.ButtonV_Disabled.Draw( GetRender(), control->GetRenderBounds() ); }

                    if ( bDepressed )
                    { return Textures.Scroller.ButtonV_Down.Draw( GetRender(), control->GetRenderBounds() ); }

                    if ( isHovered )
                    { return Textures.Scroller.ButtonV_Hover.Draw( GetRender(), control->GetRenderBounds() ); }

                    return Textures.Scroller.ButtonV_Normal.Draw( GetRender(), control->GetRenderBounds() );
                }

                if ( control->IsDisabled() )
                { return Textures.Scroller.ButtonH_Disabled.Draw( GetRender(), control->GetRenderBounds() ); }

                if ( bDepressed )
                { return Textures.Scroller.ButtonH_Down.Draw( GetRender(), control->GetRenderBounds() ); }

                if ( isHovered )
                { return Textures.Scroller.ButtonH_Hover.Draw( GetRender(), control->GetRenderBounds() ); }

                return Textures.Scroller.ButtonH_Normal.Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawProgressBar( Gwk::Controls::Base* control, bool isHorizontal, float progress ) override
            {
                Gwk::Rect rect = control->GetRenderBounds();
                Gwk::Color FillColour( 0, 211, 40, 255 );

                if ( isHorizontal )
                {
                    Textures.ProgressBar.Back.Draw( GetRender(), rect );
                    rect.w *= progress;

                    if ( rect.w > 0 )
                    { Textures.ProgressBar.Front.Draw( GetRender(), rect ); }
                }
                else
                {
                    Textures.ProgressBar.Back.Draw( GetRender(), rect );
                    int inv_progress = rect.h * ( 1 - progress );
                    rect.y += inv_progress;
                    rect.h -= inv_progress;
                    Textures.ProgressBar.Front.Draw( GetRender(), rect );
                }
            }

            void DrawListBox( Gwk::Controls::Base* control ) override
            {
                return Textures.Input.ListBox.Background.Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawListBoxLine( Gwk::Controls::Base* control, bool bSelected, bool bEven ) override
            {
                if ( bSelected )
                {
                    if ( bEven )
                    { return Textures.Input.ListBox.EvenLineSelected.Draw( GetRender(), control->GetRenderBounds() ); }
                    else
                    { return Textures.Input.ListBox.OddLineSelected.Draw( GetRender(), control->GetRenderBounds() ); }
                }

                if ( control->IsHovered() )
                { return Textures.Input.ListBox.Hovered.Draw( GetRender(), control->GetRenderBounds() ); }

                if ( bEven )
                { return Textures.Input.ListBox.EvenLine.Draw( GetRender(), control->GetRenderBounds() ); }

                return Textures.Input.ListBox.OddLine.Draw( GetRender(), control->GetRenderBounds() );
            }

            virtual void DrawSliderNotchesH( Gwk::Rect rect, int numNotches, int dist )
            {
                if ( numNotches == 0 ) { return; }

                float iSpacing = ( float ) rect.w / ( float ) numNotches;

                for ( int i = 0; i < numNotches + 1; i++ )
                {
                    GetRender()->DrawFilledRect( Gwk::Rect( rect.x + iSpacing * i, rect.y + dist - 2, 1, 5 ) );
                }
            }

            virtual void DrawSliderNotchesV( Gwk::Rect rect, int numNotches, int dist )
            {
                if ( numNotches == 0 ) { return; }

                float iSpacing = ( float ) rect.h / ( float ) numNotches;

                for ( int i = 0; i < numNotches + 1; i++ )
                {
                    GetRender()->DrawFilledRect( Gwk::Rect( rect.x + dist - 1, rect.y + iSpacing * i, 5, 1 ) );
                }
            }

            void DrawSlider( Gwk::Controls::Base* control, bool bIsHorizontal, int numNotches, int barSize ) override
            {
                if ( bIsHorizontal )
                {
                    Gwk::Rect rect = control->GetRenderBounds();
                    rect.x += barSize/2;
                    rect.w -= barSize;
                    rect.y += rect.h/2 - 1;
                    rect.h = 1;
                    GetRender()->SetDrawColor( Gwk::Color( 0, 0, 0, 100 ) );
                    DrawSliderNotchesH( rect, numNotches, barSize/2 );
                    return GetRender()->DrawFilledRect( rect );
                }

                Gwk::Rect rect = control->GetRenderBounds();
                rect.y += barSize/2;
                rect.h -= barSize;
                rect.x += rect.w/2 - 1;
                rect.w = 1;
                GetRender()->SetDrawColor( Gwk::Color( 0, 0, 0, 100 ) );
                DrawSliderNotchesV( rect, numNotches, barSize * 0.4 );
                return GetRender()->DrawFilledRect( rect );
            }

            void DrawComboBox( Gwk::Controls::Base* control, bool bDown, bool bMenuOpen ) override
            {
                if (control->IsDisabled())
                    return Textures.Input.ComboBox.Disabled.Draw( GetRender(), control->GetRenderBounds() );

                if (bDown || bMenuOpen)
                    return Textures.Input.ComboBox.Down.Draw( GetRender(), control->GetRenderBounds() );

                if (control->IsHovered())
                    return Textures.Input.ComboBox.Hover.Draw( GetRender(), control->GetRenderBounds() );

                Textures.Input.ComboBox.Normal.Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawKeyboardHighlight( Gwk::Controls::Base* control, const Gwk::Rect & r, int iOffset ) override
            {
                Gwk::Rect rect = r;
                rect.x += iOffset;
                rect.y += iOffset;
                rect.w -= iOffset * 2;
                rect.h -= iOffset * 2;
                //draw the top and bottom
                bool skip = true;

                for ( int i = 0; i < rect.w/2; i++ )
                {
                    m_render->SetDrawColor( Gwk::Color( 0, 0, 0, 255 ) );

                    if ( !skip )
                    {
                        GetRender()->DrawPixel( rect.x + ( i * 2 ), rect.y );
                        GetRender()->DrawPixel( rect.x + ( i * 2 ), rect.y + rect.h - 1 );
                    }
                    else
                    { skip = !skip; }
                }

                skip = false;

                for ( int i = 0; i < rect.h/2; i++ )
                {
                    GetRender()->SetDrawColor( Gwk::Color( 0, 0, 0, 255 ) );

                    if ( !skip )
                    {
                        GetRender()->DrawPixel( rect.x , rect.y + i * 2 );
                        GetRender()->DrawPixel( rect.x + rect.w - 1, rect.y + i * 2 );
                    }
                    else
                    { skip = !skip; }
                }
            }

            void DrawTooltip( Gwk::Controls::Base* control ) override
            {
                return Textures.Tooltip.Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawScrollButton( Gwk::Controls::Base* control, Position direction, bool bDepressed, bool bHovered, bool bDisabled ) override
            {
                int i = 0;

                if ( direction == Position::Top ) { i = 1; }
                else if ( direction == Position::Right ) { i = 2; }
                else if ( direction == Position::Bottom ) { i = 3; }

                if ( bDisabled )
                    return Textures.Scroller.Button.Disabled[i].Draw( GetRender(), control->GetRenderBounds() );

                if ( bDepressed )
                    return Textures.Scroller.Button.Down[i].Draw( GetRender(), control->GetRenderBounds() );

                if ( bHovered )
                    return Textures.Scroller.Button.Hover[i].Draw( GetRender(), control->GetRenderBounds() );

                return Textures.Scroller.Button.Normal[i].Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawComboDownArrow( Gwk::Controls::Base* control, bool bHovered, bool bDown, bool bMenuOpen, bool bDisabled ) override
            {
                if ( bDisabled )
                { return Textures.Input.ComboBox.Button.Disabled.Draw( GetRender(), control->GetRenderBounds() ); }

                if ( bDown || bMenuOpen )
                { return Textures.Input.ComboBox.Button.Down.Draw( GetRender(), control->GetRenderBounds() ); }

                if ( bHovered )
                { return Textures.Input.ComboBox.Button.Hover.Draw( GetRender(), control->GetRenderBounds() ); }

                Textures.Input.ComboBox.Button.Normal.Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawNumericUpDownButton( Gwk::Controls::Base* control, bool bDepressed, bool bUp ) override
            {
                if ( bUp )
                {
                    if ( control->IsDisabled() )    { return Textures.Input.UpDown.Up.Disabled.DrawCenter( GetRender(), control->GetRenderBounds() ); }

                    if ( bDepressed )               { return Textures.Input.UpDown.Up.Down.DrawCenter( GetRender(), control->GetRenderBounds() ); }

                    if ( control->IsHovered() )     { return Textures.Input.UpDown.Up.Hover.DrawCenter( GetRender(), control->GetRenderBounds() ); }

                    return Textures.Input.UpDown.Up.Normal.DrawCenter( GetRender(), control->GetRenderBounds() );
                }

                if ( control->IsDisabled() )    { return Textures.Input.UpDown.Down.Disabled.DrawCenter( GetRender(), control->GetRenderBounds() ); }

                if ( bDepressed )               { return Textures.Input.UpDown.Down.Down.DrawCenter( GetRender(), control->GetRenderBounds() ); }

                if ( control->IsHovered() )     { return Textures.Input.UpDown.Down.Hover.DrawCenter( GetRender(), control->GetRenderBounds() ); }

                return Textures.Input.UpDown.Down.Normal.DrawCenter( GetRender(), control->GetRenderBounds() );
            }

            void DrawStatusBar( Controls::Base* control ) override
            {
                Textures.StatusBar.Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawTreeButton( Controls::Base* control, bool bOpen ) override
            {
                Gwk::Rect rect = control->GetRenderBounds();

                if ( bOpen )
                { Textures.Tree.Minus.Draw( GetRender(), rect ); }
                else
                { Textures.Tree.Plus.Draw( GetRender(), rect ); }
            }

            void DrawColorDisplay( Controls::Base* control, Gwk::Color color ) override
            {
                Gwk::Rect rect = control->GetRenderBounds();

                if ( color.a != 255 )
                {
                    GetRender()->SetDrawColor( Gwk::Color( 255, 255, 255, 255 ) );
                    GetRender()->DrawFilledRect( rect );
                    GetRender()->SetDrawColor( Gwk::Color( 128, 128, 128, 128 ) );
                    GetRender()->DrawFilledRect( Gwk::Rect( 0, 0, rect.w/2, rect.h/2 ) );
                    GetRender()->DrawFilledRect( Gwk::Rect( rect.w/2, rect.h/2, rect.w/2, rect.h/2 ) );
                }

                GetRender()->SetDrawColor( color );
                GetRender()->DrawFilledRect( rect );
                GetRender()->SetDrawColor( Gwk::Color( 0, 0, 0, 255 ) );
                GetRender()->DrawLinedRect( rect );
            }

            void DrawModalControl( Controls::Base* control ) override
            {
                if ( !control->ShouldDrawBackground() ) { return; }

                Gwk::Rect rect = control->GetRenderBounds();
                GetRender()->SetDrawColor( Colors.ModalBackground );
                GetRender()->DrawFilledRect( rect );
            }

            void DrawMenuDivider( Controls::Base* control ) override
            {
                Gwk::Rect rect = control->GetRenderBounds();
                GetRender()->SetDrawColor( Gwk::Color( 0, 0, 0, 100 ) );
                GetRender()->DrawFilledRect( rect );
            }

            void DrawTreeControl( Controls::Base* control ) override
            {
                Textures.Tree.Background.Draw( GetRender(), control->GetRenderBounds() );
            }

            void DrawWindowCloseButton( Gwk::Controls::Base* control, bool bDepressed, bool bHovered, bool bDisabled ) override
            {
                Gwk::Rect r = Gwk::Rect( control->GetRenderBounds().x, control->GetRenderBounds().y, 31, 31 );

                if ( bDisabled )    { return Textures.Window.Close.Draw( GetRender(), r, Gwk::Color( 255, 255, 255, 50 ) ); }

                if ( bDepressed )   { return Textures.Window.Close_Down.Draw( GetRender(), r ); }

                if ( bHovered )     { return Textures.Window.Close_Hover.Draw( GetRender(), r ); }

                Textures.Window.Close.Draw( GetRender(), r );
            }

            void DrawWindowMaximizeButton( Gwk::Controls::Base* control, bool bDepressed, bool bHovered, bool bDisabled, bool bMaximized ) override
            {
                Gwk::Rect r = Gwk::Rect( control->GetRenderBounds().x, control->GetRenderBounds().y, 31, 31 );

                if ( !bMaximized )
                {
                    if ( bDisabled )    { return Textures.Window.Maxi.Draw( GetRender(), r, Gwk::Color( 255, 255, 255, 50 ) ); }

                    if ( bDepressed )   { return Textures.Window.Maxi_Down.Draw( GetRender(), r ); }

                    if ( bHovered )     { return Textures.Window.Maxi_Hover.Draw( GetRender(), r ); }

                    return Textures.Window.Maxi.Draw( GetRender(), r );
                }

                if ( bDisabled )    { return Textures.Window.Restore.Draw( GetRender(), r, Gwk::Color( 255, 255, 255, 50 ) ); }

                if ( bDepressed )   { return Textures.Window.Restore_Down.Draw( GetRender(), r ); }

                if ( bHovered )     { return Textures.Window.Restore_Hover.Draw( GetRender(), r ); }

                return Textures.Window.Restore.Draw( GetRender(), r );
            }

            void DrawWindowMinimizeButton( Gwk::Controls::Base* control, bool bDepressed, bool bHovered, bool bDisabled ) override
            {
                Gwk::Rect r = Gwk::Rect( control->GetRenderBounds().x, control->GetRenderBounds().y, 31, 31 );

                if ( bDisabled )    { return Textures.Window.Mini.Draw( GetRender(), r, Gwk::Color( 255, 255, 255, 100 ) ); }

                if ( bDepressed )   { return Textures.Window.Mini_Down.Draw( GetRender(), r ); }

                if ( bHovered )     { return Textures.Window.Mini_Hover.Draw( GetRender(), r ); }

                Textures.Window.Mini.Draw( GetRender(), r );
            }


            void DrawSlideButton( Gwk::Controls::Base* control, bool bDepressed, bool bHorizontal ) override
            {
                if ( !bHorizontal )
                {
                    if ( control->IsDisabled() )    { return Textures.Input.Slider.V.Disabled.DrawCenter( GetRender(), control->GetRenderBounds() ); }

                    if ( bDepressed )               { return Textures.Input.Slider.V.Down.DrawCenter( GetRender(), control->GetRenderBounds() ); }

                    if ( control->IsHovered() )     { return Textures.Input.Slider.V.Hover.DrawCenter( GetRender(), control->GetRenderBounds() ); }

                    return Textures.Input.Slider.V.Normal.DrawCenter( GetRender(), control->GetRenderBounds() );
                }

                if ( control->IsDisabled() )    { return Textures.Input.Slider.H.Disabled.DrawCenter( GetRender(), control->GetRenderBounds() ); }

                if ( bDepressed )               { return Textures.Input.Slider.H.Down.DrawCenter( GetRender(), control->GetRenderBounds() ); }

                if ( control->IsHovered() )     { return Textures.Input.Slider.H.Hover.DrawCenter( GetRender(), control->GetRenderBounds() ); }

                Textures.Input.Slider.H.Normal.DrawCenter( GetRender(), control->GetRenderBounds() );
            }

            void DrawTreeNode( Controls::Base* ctrl, bool bOpen, bool bSelected, int iLabelHeight, int iLabelWidth, int iHalfWay, int iLastBranch, bool bIsRoot ) override
            {
                if ( bSelected )
                {
                    Textures.Selection.Draw( GetRender(), Gwk::Rect( 17, 0, iLabelWidth + 2, iLabelHeight - 1 ) );
                }

                Base::DrawTreeNode( ctrl, bOpen, bSelected, iLabelHeight, iLabelWidth, iHalfWay, iLastBranch, bIsRoot );
            }

            void DrawCategoryHolder( Controls::Base* ctrl ) override
            {
                Textures.CategoryList.Outer.Draw( GetRender(), ctrl->GetRenderBounds() );
            }

            void DrawCategoryInner( Controls::Base* ctrl, bool bCollapsed ) override
            {
                if ( bCollapsed )
                    return Textures.CategoryList.Header.Draw( GetRender(), ctrl->GetRenderBounds() );

                Textures.CategoryList.Inner.Draw( GetRender(), ctrl->GetRenderBounds() );
            }
        };
    }
}

#endif
