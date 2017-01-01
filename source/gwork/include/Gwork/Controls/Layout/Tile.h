/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_LAYOUT_TILE_H
#define GWK_CONTROLS_LAYOUT_TILE_H

#include <Gwork/Controls/Base.h>

namespace Gwk
{
    namespace Controls
    {
        namespace Layout
        {
            class GWK_EXPORT Tile : public Base
            {
            public:

                GWK_CONTROL_INLINE(Tile, Base)
                {
                    Dock(Position::Fill);
                    SetTileSize(22, 22);
                }

                void PostLayout(Skin::Base* skin) override
                {
                    Gwk::Rect bounds = GetInnerBounds();
                    Gwk::Point pos = Point(bounds.x, bounds.y);

                    for(auto&& child : Children)
                    {
                        if (child->GetDock() != Position::None)
                            continue;

                        child->SetPos(pos.x+m_tileSize.x/2 - child->Width()/2,
                                       pos.y+m_tileSize.y/2 - child->Height()/2);
                        pos.x = pos.x+m_tileSize.x;

                        if (pos.x+m_tileSize.x > bounds.x+bounds.w)
                        {
                            pos.x = bounds.x;
                            pos.y += m_tileSize.y;
                        }
                    }
                }

                void SetTileSize(int x, int y)
                {
                    m_tileSize = Point(x, y);
                }

            private:

                Point m_tileSize;
            };


        }
    }
}

#endif // ifndef GWK_CONTROLS_LAYOUT_TILE_H
