/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Gwork.h>
#include <Gwork/Controls/SplitterBar.h>

using namespace Gwk;
using namespace Controls;

GWK_CONTROL_CONSTRUCTOR(SplitterBar)
{
    SetTarget(this);
    RestrictToParent(true);
}

void SplitterBar::Layout(Skin::Base* /*skin*/)
{
    MoveTo(X(), Y());
}
