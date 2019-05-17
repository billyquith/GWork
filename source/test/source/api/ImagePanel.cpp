/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/ImagePanel.h>

namespace Gwk {
namespace Test {

class ImagePanel:public ControlClass<ImagePanel, TestUnit>
{
public:

    ImagePanel(Gwk::Controls::Base *parent, const Gwk::String &name=""):BaseClass(parent, name)
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

GWK_REGISTER_TEST(ImagePanel);

}}

