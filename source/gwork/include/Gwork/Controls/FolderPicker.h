/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_FOLDERPICKER_H
#define GWK_CONTROLS_FOLDERPICKER_H

#include <Gwork/BaseRender.h>
#include <Gwork/Controls/Dialogs/FolderOpen.h>
#include <Gwork/Controls/TextBox.h>


namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT FolderPicker : public Controls::Base
        {
        public:

            GWK_CONTROL_INLINE(FolderPicker, Controls::Base)
            {
                m_button = new Controls::Button(this);
                m_button->Dock(Position::Right);
                m_button->SetMargin(Margin(2, 0, 0, 0));
                m_button->SetText("..");
                m_button->SetSize(20, 20);
                m_button->onPress.Add(this, &FolderPicker::OnBrowse);
                m_textBox = new Controls::TextBox(this);
                m_textBox->Dock(Position::Fill);
                this->SetSize(100, 20);
                m_browseName = "Find Folder";
            }


            void SetFolder(const String& strValue)
            {
                m_textBox->SetText(strValue);
                m_textBox->MoveCaretToEnd();
                onFolderChanged.Call(this);
            }

            const Gwk::String& GetFolder()
            {
                return m_textBox->GetText();
            }

            void OnBrowse(Event::Info)
            {
                String folder;
                if (Dialogs::FolderOpen(true, "Name", GetFolder(), folder))
                {
                    SetFolder(folder);
                }
            }

            String GetValue() override
            {
                return GetFolder();
            }

            void SetValue(const String& strValue) override
            {
                return SetFolder(strValue);
            }

            Event::Listener onFolderChanged;

        private:

            Controls::TextBox*  m_textBox;
            Controls::Button*   m_button;
            Gwk::String         m_browseName;
        };


    }

}
#endif // ifndef GWK_CONTROLS_FOLDERPICKER_H
