/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_LAYOUT_POSITION_H
#define GWK_CONTROLS_LAYOUT_POSITION_H

#include "Gwork/Controls/Label.h"
#include "Gwork/Utility.h"


namespace Gwk
{
    namespace Controls
    {
        namespace Layout
        {
            class GWK_EXPORT Position : public Base
            {
            public:

                GWK_CONTROL_INLINE(Position, Base)
                {
                    SetPosition(Docking::Left|Docking::Top);
                }

                void PostLayout(Skin::Base* skin) override
                {
                    for (Base::List::iterator it = Children.begin(); it != Children.end(); ++it)
                    {
                        Base* pChild = *it;

                        if (pChild->GetDock() != Docking::None)
                            continue;

                        pChild->Position(m_iPosition);
                    }
                }

                void SetPosition(int p)
                {
                    if (m_iPosition == p)
                        return;

                    m_iPosition = p;
                    Invalidate();
                }

            private:

                int m_iPosition;
            };


            class GWK_EXPORT Center : public Position
            {
                GWK_CONTROL_INLINE(Center, Position)
                {
                    SetPosition(Docking::Center);
                }
            };


        }
    }
}
#endif // ifndef GWK_CONTROLS_LAYOUT_POSITION_H
