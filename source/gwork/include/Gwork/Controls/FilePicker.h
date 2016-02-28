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
                m_Button = new Controls::Button(this);
                m_Button->Dock(Docking::Right);
                m_Button->SetMargin(Margin(2, 0, 0, 0));
                m_Button->SetText("..");
                m_Button->SetSize(20, 20);
                m_Button->onPress.Add(this, &FilePicker::OnBrowse);
                m_TextBox = new Controls::TextBox(this);
                m_TextBox->Dock(Docking::Fill);
                this->SetSize(100, 20);
                SetFileType("Any Type | *.*");
            }

            void SetFileType(const Gwk::String& string)
            {
                m_FileType = string;
            }

            Gwk::String GetFileType()
            {
                return m_FileType;
            }

            void SetFileName(const String& strValue)
            {
                m_TextBox->SetText(strValue);
                m_TextBox->MoveCaretToEnd();
                onFileChanged.Call(this);
            }

            const Gwk::String& GetFileName()
            {
                return m_TextBox->GetText();
            }

            void OnBrowse()
            {
                String fileChosen;
                if (Dialogs::FileOpen(true, "Name", "Start Path", m_FileType, fileChosen))
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

            Event::Caller onFileChanged;

        private:

            Controls::TextBox*  m_TextBox;
            Controls::Button*   m_Button;

            String m_FileType;
        };


    }

}
#endif // ifndef GWK_CONTROLS_FILEPICKER_H
