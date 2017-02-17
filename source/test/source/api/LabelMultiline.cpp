/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Controls/CrossSplitter.h>

using namespace Gwk;

class LabelMultiline : public TestUnit
{
public:

    GWK_CONTROL_INLINE(LabelMultiline, TestUnit)
    {
        Dock(Position::Fill);
        Gwk::Controls::CrossSplitter* splitter = new Gwk::Controls::CrossSplitter(this);
        splitter->Dock(Position::Fill);
        splitter->SetMargin(Margin(10, 10, 10, 10));
        {
            Gwk::Controls::Label* label = new Gwk::Controls::Label(splitter);
            label->SetText(
                "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ultrices pharetra scelerisque. Ut posuere velit ligula suscipit ut lobortis ligula congue. Aliquam lacinia cursus est, quis aliquam nisl scelerisque vitae. Nunc porta eros sem, nec rhoncus eros. Integer elementum, quam vitae egestas dictum, mi quam gravida augue, non fringilla lacus nisi sit amet nunc. Maecenas dolor tellus, consequat sed sodales ut, aliquam ac enim. Nulla facilisi. Maecenas eleifend, velit a lobortis vehicula, nunc lacus egestas leo, volutpat egestas augue.");
            label->SetWrap(true);
            splitter->SetPanel(0, label);
        }
        {
            Gwk::Controls::Label* label = new Gwk::Controls::Label(splitter);
            label->SetText(
                "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed ultrices pharetra scelerisque. Ut posuere velit a ligula suscipit ut lobortis ligula congue. Aliquam lacinia cursus est, quis aliquam nisl scelerisque vitae. Nunc porta eros sem, nec rhoncus eros. Integer elementum, quam vitae egestas dictum, mi quam gravida augue, non fringilla lacus nisi sit amet nunc. Maecenas dolor tellus, consequat sed sodales ut, aliquam ac enim. Nulla facilisi. Maecenas eleifend, velit a lobortis vehicula, nunc lacus egestas leo, volutpat egestas augue nulla nec turpis. Aenean convallis diam magna. Duis ac lacinia massa. In id dui vel dui laoreet congue. Aliquam suscipit quam et augue sagittis egestas. Integer posuere arcu sit amet lacus porttitor et malesuada enim mollis. Duis luctus est in purus consectetur sit amet volutpat tortor euismod. Nulla facilisi.");
            label->SetWrap(true);
            splitter->SetPanel(1, label);
        }
        {
            Gwk::Controls::Label* label = new Gwk::Controls::Label(splitter);
            label->SetText(
                "Integer eget rutrum nisi. Ut eget dui et turpis volutpat pulvinar non sed lacus. Proin vestibulum felis nec elit tristique non imperdiet eros pretium. Nullam pulvinar sem eleifend turpis eleifend vel dapibus dui sodales. Curabitur euismod hendrerit felis nec vestibulum. Suspendisse tempus gravida ligula a vestibulum. Phasellus et eros at justo hendrerit cursus. Phasellus rutrum porta diam, in mollis ante aliquam at. Vestibulum interdum ligula at massa auctor scelerisque. Sed at tincidunt risus. Donec ut est dui. Vestibulum blandit urna eu metus malesuada blandit");
            label->SetWrap(true);
            splitter->SetPanel(2, label);
        }
        {
            Gwk::Controls::Label* label = new Gwk::Controls::Label(splitter);
            label->SetText(
                "Nullam vel risus eget lacus consectetur rutrum. Curabitur eros libero, porta sed commodo vel, euismod non quam. Fusce bibendum posuere metus, nec mollis odio rutrum ac. Cras nec sapien et mauris dapibus pretium id quis dolor. Sed a velit vel tellus viverra sodales. Praesent tempor purus et elit ultrices tristique. Sed in enim nec elit molestie fermentum et quis enim. Nullam varius placerat lacus nec ultrices. Aliquam erat volutpat. Suspendisse potenti. Nullam euismod pulvinar luctus. Vestibulum ut dui nisi, eget tempus est. Vivamus molestie arcu non enim pulvinar sollicitudin. Pellentesque dapibus risus sit amet diam tempor faucibus accumsan ante porta. Phasellus quis facilisis quam. Fusce eget adipiscing magna.");
            label->SetWrap(true);
            splitter->SetPanel(3, label);
        }
    }

    Gwk::Font m_font;
};


DECLARE_TEST(LabelMultiline);
