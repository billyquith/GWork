/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_GROUPBOX_H
#define GWK_CONTROLS_GROUPBOX_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>


namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT GroupBox : public Label
        {
        public:

            GWK_CONTROL(GroupBox, Label);

            void Render(Skin::Base* skin) override;
            void Layout(Skin::Base* skin) override;

            virtual void SetInnerMargin(int i)
            {
                m_innerMargin = i;
            }

        protected:

            int m_innerMargin;

        };


    }
}
#endif // ifndef GWK_CONTROLS_GROUPBOX_H
