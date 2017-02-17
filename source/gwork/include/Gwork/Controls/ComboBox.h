/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_COMBOBOX_H
#define GWK_CONTROLS_COMBOBOX_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/TextBox.h>
#include <Gwork/Controls/Menu.h>


namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT ComboBox : public Button
        {
        public:

            GWK_CONTROL(ComboBox, Button);

            void Render(Skin::Base* skin) override;
            void Layout(Skin::Base* skin) override;
            void UpdateColors() override;

            virtual void SelectItem(MenuItem* item, bool bFireChangeEvents =
                                        true);
            virtual void SelectItemByName(const Gwk::String& name,
                                          bool bFireChangeEvents = true);
            virtual Gwk::Controls::Label* GetSelectedItem();

            void OnPress(Event::Info info) override;
            virtual void OnItemSelected(Event::Info info);
            virtual void OpenList();
            virtual void CloseList();

            virtual void ClearItems();

            virtual MenuItem* AddItem(const String& strLabel, const String& strName = "");
            bool OnKeyUp(bool bDown) override;
            bool OnKeyDown(bool bDown) override;

            void RenderFocus(Gwk::Skin::Base* skin) override;
            void OnLostKeyboardFocus() override;
            void OnKeyboardFocus() override;

            virtual bool IsMenuOpen();

            bool IsMenuComponent() override
            {
                return true;
            }

            Gwk::Event::Listener onSelection;

        protected:

            Menu* m_menu;
            MenuItem* m_selectedItem;

            Controls::Base* m_button;

        };


    }
}
#endif // ifndef GWK_CONTROLS_COMBOBOX_H
