/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_MENUITEM_H
#define GWK_CONTROLS_MENUITEM_H

#include <Gwork/BaseRender.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Controls/Menu.h>

namespace Gwk
{
    namespace Controls
    {
        class Menu;

        class GWK_EXPORT MenuItem : public Button
        {
        public:

            GWK_CONTROL(MenuItem, Button);

            virtual ~MenuItem();

            void Render(Skin::Base* skin) override;
            void Layout(Skin::Base* skin) override;

            void SizeToContents() override;

            virtual void OnPress(Event::Info) override;

            Menu* GetMenu();

            bool IsMenuOpen();
            void OpenMenu();
            void CloseMenu();
            void ToggleMenu();

            void SetOnStrip(bool b)
            {
                m_bOnStrip = b;
            }

            bool OnStrip()
            {
                return m_bOnStrip;
            }

            virtual void SetCheckable(bool bCheck)
            {
                m_bCheckable = bCheck;
            }

            virtual void SetChecked(bool bCheck);
            virtual bool GetChecked()
            {
                return m_bChecked;
            }

            template <typename T>
            MenuItem* SetAction(Gwk::Event::Handler* handler, T fn)
            {
                if (m_accelerator)
                    AddAccelerator(m_accelerator->GetText(), fn, handler);

                onMenuItemSelected.Add(handler, fn);
                return this;
            }

            void SetAccelerator(const String& strAccelerator);

            Gwk::Event::Listener onMenuItemSelected;
            Gwk::Event::Listener onChecked;
            Gwk::Event::Listener onUnChecked;
            Gwk::Event::Listener onCheckChange;

        private:

            Menu*   m_menu;
            bool m_bOnStrip;
            bool m_bCheckable;
            bool m_bChecked;

            Label*  m_accelerator;


            Controls::Base*     m_submenuArrow;
        };


    }

}
#endif // ifndef GWK_CONTROLS_MENUITEM_H
