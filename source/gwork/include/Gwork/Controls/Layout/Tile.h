/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2015-2016 Billy Quith
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
                    Dock(Docking::Fill);
                    SetTileSize(22, 22);
                }

                void PostLayout(Skin::Base* skin) override
                {
                    Gwk::Rect bounds = GetInnerBounds();
                    Gwk::Point pos = Point(bounds.x, bounds.y);

                    for (Base::List::iterator it = Children.begin(); it != Children.end(); ++it)
                    {
                        Base* pChild = *it;

                        if (pChild->GetDock() != Docking::None)
                            continue;

                        pChild->SetPos(pos.x+m_TileSize.x/2 - pChild->Width()/2,
                                       pos.y+m_TileSize.y/2 - pChild->Height()/2);
                        pos.x = pos.x+m_TileSize.x;

                        if (pos.x+m_TileSize.x > bounds.x+bounds.w)
                        {
                            pos.x = bounds.x;
                            pos.y += m_TileSize.y;
                        }
                    }
                }

                void SetTileSize(int x, int y)
                {
                    m_TileSize = Point(x, y);
                }

            private:

                Point m_TileSize;
            };


        }
    }
}

#endif // ifndef GWK_CONTROLS_LAYOUT_TILE_H
