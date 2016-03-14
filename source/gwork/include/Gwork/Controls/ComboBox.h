/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
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

            virtual void Render(Skin::Base* skin) override;
            virtual void Layout(Skin::Base* skin) override;
            virtual void UpdateColours() override;

            virtual void SelectItem(MenuItem* item, bool bFireChangeEvents =
                                        true);
            virtual void SelectItemByName(const Gwk::String& name,
                                          bool bFireChangeEvents = true);
            virtual Gwk::Controls::Label* GetSelectedItem();

            virtual void OpenList();
            virtual void CloseList();

            virtual void ClearItems();

            virtual MenuItem* AddItem(const String& strLabel, const String& strName = "");
            virtual bool      OnKeyUp(bool bDown) override;
            virtual bool      OnKeyDown(bool bDown) override;

            virtual void RenderFocus(Gwk::Skin::Base* skin) override;
            virtual void OnLostKeyboardFocus() override;
            virtual void OnKeyboardFocus() override;

            virtual bool IsMenuOpen();

            virtual bool IsMenuComponent() override
            {
                return true;
            }

            Gwk::Event::Caller onSelection;

        protected:

            virtual void OnPress() override;
            virtual void OnItemSelected(Event::Info info);

            Menu* m_menu;
            MenuItem* m_selectedItem;

            Controls::Base* m_button;

        };


    }
}
#endif // ifndef GWK_CONTROLS_COMBOBOX_H
