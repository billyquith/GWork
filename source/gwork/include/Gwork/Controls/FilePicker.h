/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2015-2016 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_FILEPICKER_H
#define GWK_CONTROLS_FILEPICKER_H

#include <Gwork/BaseRender.h>
#include <Gwork/Controls/Dialogs/FileOpen.h>
#include <Gwork/Controls/TextBox.h>


namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT FilePicker : public Controls::Base
        {
        public:

            GWK_CONTROL_INLINE(FilePicker, Controls::Base)
            {
                m_button = new Controls::Button(this);
                m_button->Dock(Position::Right);
                m_button->SetMargin(Margin(2, 0, 0, 0));
                m_button->SetText("..");
                m_button->SetSize(20, 20);
                m_button->onPress.Add(this, &FilePicker::OnBrowse);
                m_textBox = new Controls::TextBox(this);
                m_textBox->Dock(Position::Fill);
                this->SetSize(100, 20);
                SetFileType("Any Type | *.*");
            }

            void SetFileType(const Gwk::String& string)
            {
                m_fileType = string;
            }

            Gwk::String GetFileType()
            {
                return m_fileType;
            }

            void SetFileName(const String& strValue)
            {
                m_textBox->SetText(strValue);
                m_textBox->MoveCaretToEnd();
                onFileChanged.Call(this);
            }

            const Gwk::String& GetFileName()
            {
                return m_textBox->GetText();
            }

            void OnBrowse(Event::Info)
            {
                String fileChosen;
                if (Dialogs::FileOpen(true, "Name", "Start Path", m_fileType, fileChosen))
                {
                    SetFileName(fileChosen);
                }
            }

            virtual String GetValue() override
            {
                return GetFileName();
            }

            virtual void SetValue(const String& strValue) override
            {
                return SetFileName(strValue);
            }

            Event::Listener onFileChanged;

        private:

            Controls::TextBox*  m_textBox;
            Controls::Button*   m_button;

            String m_fileType;
        };


    }

}
#endif // ifndef GWK_CONTROLS_FILEPICKER_H
