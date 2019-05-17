/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_LAYOUT_POSITION_H
#define GWK_CONTROLS_LAYOUT_POSITION_H

#include <Gwork/Controls/Label.h>
#include <Gwork/Utility.h>


namespace Gwk
{
    namespace Controls
    {
        namespace Layout
        {
            class GWK_EXPORT CalcPosition : public ControlClass<CalcPosition, Base>
            {
            public:

                CalcPosition(Gwk::Controls::Base *parent, const Gwk::String &name=""):BaseClass(parent, name)
                {
                    SetPosition(Position::Left | Position::Top);
                }

                void PostLayout(Skin::Base* skin) override
                {
                    for (auto&& child : Children)
                    {
                        if (child->GetDock() != Position::None)
                            continue;

                        child->SetPosition(m_position);
                    }
                }

                void SetPosition(Position p)
                {
                    if (m_position != p)
                    {
                        m_position = p;
                        Invalidate();
                    }
                }

            private:

                Position m_position;
            };


            class GWK_EXPORT Center : public ControlClass<Center, CalcPosition>
            {
            public:
                Center(Gwk::Controls::Base *parent, const Gwk::String &name=""):BaseClass(parent, name)
                {
                    SetPosition(Position::Center);
                }
            };


        }
    }
}
#endif // ifndef GWK_CONTROLS_LAYOUT_POSITION_H
