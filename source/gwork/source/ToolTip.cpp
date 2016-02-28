/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include "Gwork/ToolTip.h"
#include "Gwork/Utility.h"

namespace Gwk {
    namespace ToolTip
    {
        Controls::Base* g_ToolTip = NULL;

        GWK_EXPORT bool TooltipActive()
        {
            return g_ToolTip != NULL;
        }

        void Enable(Controls::Base* pControl)
        {
            if (!pControl->GetToolTip())
                return;

            g_ToolTip = pControl;
        }

        void Disable(Controls::Base* pControl)
        {
            if (g_ToolTip == pControl)
                g_ToolTip = NULL;
        }

        void RenderToolTip(Skin::Base* skin)
        {
            if (!g_ToolTip)
                return;

            Gwk::Renderer::Base* render = skin->GetRender();
            Gwk::Point pOldRenderOffset = render->GetRenderOffset();
            Gwk::Point MousePos = Input::GetMousePosition();
            Gwk::Rect Bounds = g_ToolTip->GetToolTip()->GetBounds();
            Gwk::Rect rOffset = Gwk::Rect(MousePos.x-Bounds.w*0.5f, MousePos.y-Bounds.h-10,
                                            Bounds.w,
                                            Bounds.h);
            rOffset = Utility::ClampRectToRect(rOffset, g_ToolTip->GetCanvas()->GetBounds());
            // Calculate offset on screen bounds
            render->AddRenderOffset(rOffset);
            render->EndClip();
            skin->DrawToolTip(g_ToolTip->GetToolTip());
            g_ToolTip->GetToolTip()->DoRender(skin);
            render->SetRenderOffset(pOldRenderOffset);
        }

        void ControlDeleted(Controls::Base* pControl)
        {
            Disable(pControl);
        }

    }
}
