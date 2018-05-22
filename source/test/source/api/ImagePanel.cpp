/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
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
            img->SetImage("logo.png");
            img->SetBounds(10, 30, 159, 69);
        }
        // Missing
        {
            Controls::ImagePanel* img = new Controls::ImagePanel(this);
            img->SetImage("missingimage.png");
            img->SetBounds(200, 10, 100, 100);
        }
    }

};


DECLARE_TEST(ImagePanel);
