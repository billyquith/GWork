/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/ImagePanel.h>

using namespace Gwk;

class ImagePanel : public TestUnit
{
public:

    GWK_CONTROL_INLINE(ImagePanel, TestUnit)
    {
        // Normal
        {
            Controls::ImagePanel* img = new Controls::ImagePanel(this);
            img->SetImage("gwen.png");
            img->SetBounds(10, 10, 100, 100);
        }
        // Missing
        {
            Controls::ImagePanel* img = new Controls::ImagePanel(this);
            img->SetImage("missingimage.png");
            img->SetBounds(120, 10, 100, 100);
        }
    }

};


DECLARE_TEST(ImagePanel);
