/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_PROPERTY_FOLDER_H
#define GWK_CONTROLS_PROPERTY_FOLDER_H

#include <Gwork/Controls/Properties.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Controls/Dialogs/FolderOpen.h>

namespace Gwk
{
    namespace Controls
    {
        namespace Property
        {
            class Folder : public Property::Text
            {
                GWK_CONTROL_INLINE(Folder, Property::Text)
                {
                    Controls::Button* button = new Controls::Button(this);
                    button->Dock(Position::Right);
                    button->SetText("...");
                    button->SetWidth(20);
                    button->onPress.Add(this, &ThisClass::OnButtonPress);
                    button->SetMargin(Margin(1, 1, 1, 2));
                    m_strDialogName = "Find Folder";
                }

                void OnButtonPress(Event::Info)
                {
                    String folder;
                    if (Dialogs::FolderOpen(true, m_strDialogName,
                                            m_textBox->GetText(), folder))
                    {
                        m_textBox->SetText(folder);
                    }
                }

                String m_strDialogName;
            };


        }
    }
}
#endif // ifndef GWK_CONTROLS_PROPERTY_FOLDER_H
