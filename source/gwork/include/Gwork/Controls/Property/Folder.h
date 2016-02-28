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
                    Controls::Button* pButton = new Controls::Button(this);
                    pButton->Dock(Docking::Right);
                    pButton->SetText("...");
                    pButton->SetWidth(20);
                    pButton->onPress.Add(this, &ThisClass::OnButtonPress);
                    pButton->SetMargin(Margin(1, 1, 1, 2));
                    m_strDialogName = "Find Folder";
                }

                void OnButtonPress(Controls::Base* control)
                {
                    String folder;
                    if (Dialogs::FolderOpen(true, m_strDialogName,
                                            m_TextBox->GetText(), folder))
                    {
                        m_TextBox->SetText(folder);
                    }
                }

                String m_strDialogName;
            };


        }
    }
}
#endif // ifndef GWK_CONTROLS_PROPERTY_FOLDER_H
