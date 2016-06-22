/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_PROPERTY_BASEPROPERTY_H
#define GWK_CONTROLS_PROPERTY_BASEPROPERTY_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Utility.h>


namespace Gwk
{
    namespace Controls
    {
        namespace Property
        {
            class GWK_EXPORT Base : public Gwk::Controls::Base
            {
            public:

                GWK_CONTROL_INLINE(Base, Gwk::Controls::Base)
                {
                    SetHeight(17);
                }

                virtual String GetPropertyValue() = 0;

                virtual void SetPropertyValue(const String& v,
                                              bool bFireChangeEvents = false) = 0;

                virtual bool IsEditing() = 0;

                virtual void DoChanged()
                {
                    Event::Information info(this);
                    info.String = GetPropertyValue();
                    onChange.Call(this, info);
                }

                void OnPropertyValueChanged(Event::Info)
                {
                    DoChanged();
                }

                Event::Listener onChange;
            };


        }
    }
}
#endif // ifndef GWK_CONTROLS_PROPERTY_BASEPROPERTY_H
