/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_PROPERTY_FILE_H
#define GWK_CONTROLS_PROPERTY_FILE_H

#include <Gwork/Controls/Properties.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Controls/Dialogs/FolderOpen.h>

namespace Gwk
{
    namespace Controls
    {
        namespace Property
        {
            class File : public Property::Text
            {
                GWK_CONTROL_INLINE(File, Property::Text)
                {
                    Controls::Button* button = new Controls::Button(this);
                    button->Dock(Position::Right);
                    button->SetText("...");
                    button->SetWidth(20);
                    button->onPress.Add(this, &ThisClass::OnButtonPress);
                    button->SetMargin(Margin(1, 1, 1, 2));
                    m_strDialogName = "Find File";
                    m_strFileExtension = "*.*";
                }

                File* SetExtension(const Gwk::String& string)
                {
                    m_strFileExtension = string;
                    return this;
                }

                void OnButtonPress(Event::Info)
                {
                    String fileChosen;
                    if (Dialogs::FileOpen(true, m_strDialogName,
                                          m_textBox->GetText(), m_strFileExtension, fileChosen))
                    {
                        m_textBox->SetText(fileChosen);
                    }
                }

                String m_strDialogName;
                String m_strFileExtension;
            };


        }
    }
}
#endif // ifndef GWK_CONTROLS_PROPERTY_FILE_H
