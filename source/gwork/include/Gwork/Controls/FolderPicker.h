/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2015-2016 Billy Quith
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
                m_Button = new Controls::Button(this);
                m_Button->Dock(Docking::Right);
                m_Button->SetMargin(Margin(2, 0, 0, 0));
                m_Button->SetText("..");
                m_Button->SetSize(20, 20);
                m_Button->onPress.Add(this, &FolderPicker::OnBrowse);
                m_TextBox = new Controls::TextBox(this);
                m_TextBox->Dock(Docking::Fill);
                this->SetSize(100, 20);
                m_BrowseName = "Find Folder";
            }


            void SetFolder(const String& strValue)
            {
                m_TextBox->SetText(strValue);
                m_TextBox->MoveCaretToEnd();
                onFolderChanged.Call(this);
            }

            const Gwk::String& GetFolder()
            {
                return m_TextBox->GetText();
            }

            void OnBrowse()
            {
                String folder;
                if (Dialogs::FolderOpen(true, "Name", GetFolder(), folder))
                {
                    SetFolder(folder);
                }
            }

            virtual String GetValue() override
            {
                return GetFolder();
            }

            virtual void SetValue(const String& strValue) override
            {
                return SetFolder(strValue);
            }

            Event::Caller onFolderChanged;

        private:

            Controls::TextBox*  m_TextBox;
            Controls::Button*   m_Button;
            Gwk::String         m_BrowseName;
        };


    }

}
#endif // ifndef GWK_CONTROLS_FOLDERPICKER_H
