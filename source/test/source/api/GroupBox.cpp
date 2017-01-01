/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/GroupBox.h>

using namespace Gwk;


static const char *c_text =
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam et dolor sit amet quam euismod \
lobortis. Morbi accumsan, odio accumsan dignissim dignissim, leo magna tempor \
velit, at interdum magna nibh eu leo. Quisque non scelerisque odio. Vestibulum \
condimentum tincidunt libero, a semper est dapibus non. Proin rutrum lorem nec \
ante sodales ultrices. Quisque ut risus tortor, vitae laoreet sapien. Vestibulum tortor \
velit, semper eu egestas at, sagittis sit amet massa. Duis erat ipsum, feugiat at pulvinar \
vel, sodales id velit. Nam eget est eget lacus sodales fermentum. Ut semper, nulla sed \
adipiscing scelerisque, augue lacus vehicula turpis, ut ullamcorper mauris diam id neque. \
Vestibulum nec massa eu nunc venenatis suscipit vitae sed sem. \n\nPellentesque sed diam \
at sapien malesuada eleifend. Sed quis felis metus, cursus pharetra risus. Phasellus in \
nisl neque, a rutrum nibh. Vestibulum sem massa, condimentum vel pellentesque at, mattis \
id nibh. Fusce rutrum auctor accumsan. Ut nulla diam, hendrerit eu semper eu, vulputate eget \
sem. Morbi hendrerit metus eu mi consequat eu ultrices mi fermentum. Cras ac lectus turpis. \
Proin non aliquet purus. Ut lobortis turpis non tortor auctor eleifend. Class aptent taciti \
sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Sed iaculis gravida \
ipsum et cursus. Sed hendrerit libero a massa rutrum eu gravida nunc egestas. \
Maecenas venenatis dui sit amet dolor luctus quis scelerisque mi tincidunt. Cum sociis \
natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.";


class GroupBox : public TestUnit
{
public:

    GWK_CONTROL_INLINE(GroupBox, TestUnit)
    {
        Gwk::Controls::GroupBox* group = new Gwk::Controls::GroupBox(this);
        group->SetText("Group Box");
        group->SetSize(300, 200);

		Gwk::Controls::Label* label = new Gwk::Controls::Label( group );
		label->SetText(c_text);
		label->SetWrap( true );
		label->Dock(Position::Fill);
    }
};


DECLARE_TEST(GroupBox);
