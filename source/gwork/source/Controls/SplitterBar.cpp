/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Gwork.h>
#include <Gwork/Controls/SplitterBar.h>

using namespace Gwk;
using namespace Controls;

SplitterBar::SplitterBar(Gwk::Controls::Base *parent, const Gwk::String &name):BaseClass(parent, name)
{
    SetTarget(this);
    RestrictToParent(true);
}

void SplitterBar::Layout(Skin::Base* /*skin*/)
{
    MoveTo(X(), Y());
}
