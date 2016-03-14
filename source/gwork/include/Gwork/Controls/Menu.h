/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_MENU_H
#define GWK_CONTROLS_MENU_H

#include <Gwork/BaseRender.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/MenuItem.h>
#include <Gwork/Controls/ScrollControl.h>

namespace Gwk
{
    namespace Controls
    {
        class MenuItem;

        class GWK_EXPORT Menu : public ScrollControl
        {
        public:

            GWK_CONTROL(Menu, ScrollControl);

            virtual void Render(Skin::Base* skin) override;
            virtual void RenderUnder(Skin::Base* skin) override;

            virtual void Layout(Skin::Base* skin) override;

            virtual MenuItem* AddItem(const String& strName,
                                      const String& strIconName = "",
                                      const String& strAccelerator = "");

            virtual void AddDivider();

            void OnHoverItem(Event::Info info);
            void CloseAll();
            bool IsMenuOpen();
            void ClearItems();

            virtual void Open(Position pos);
            virtual void Close();

            virtual bool IsMenuComponent() override
            {
                return true;
            }

            virtual void CloseMenus() override;

            bool IconMarginDisabled()
            {
                return m_bDisableIconMargin;
            }

            void SetDisableIconMargin(bool bDisable)
            {
                m_bDisableIconMargin = bDisable;
            }

            bool DeleteOnClose()
            {
                return m_bDeleteOnClose;
            }

            void SetDeleteOnClose(bool b)
            {
                m_bDeleteOnClose = b;
            }

        protected:

            virtual bool ShouldHoverOpenMenu()
            {
                return true;
            }

            virtual void OnAddItem(MenuItem* item);

            bool m_bDisableIconMargin;
            bool m_bDeleteOnClose;
        };


        class GWK_EXPORT MenuDivider : public Base
        {
        public:

            GWK_CONTROL_INLINE(MenuDivider, Base)
            {
                SetHeight(1);
            }

            void Render(Gwk::Skin::Base* skin) override;
        };


    }

}
#endif // ifndef GWK_CONTROLS_MENU_H
