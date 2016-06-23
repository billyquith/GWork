/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2015-2016 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Utility.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/PageControl.h>
#include <Gwork/Anim.h>

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(PageControl)
{
    m_pageCount = 0;
    m_currentPage = 0;
    SetUseFinishButton(true);

    for (int i = 0; i < MaxPages; i++)
    {
        m_pages[i] = nullptr;
    }

    Base* controls = new Base(this);
    controls->Dock(Position::Bottom);
    controls->SetSize(24, 24);
    controls->SetMargin(Margin(10, 10, 10, 10));
    m_finish = new Controls::Button(controls);
    m_finish->SetText("Finish");
    m_finish->Dock(Position::Right);
    m_finish->onPress.Add(this, &ThisClass::Finish);
    m_finish->SetSize(70);
    m_finish->SetMargin(Margin(4, 0, 0, 0));
    m_finish->Hide();
    m_next = new Controls::Button(controls);
    m_next->SetText("Next >");
    m_next->Dock(Position::Right);
    m_next->onPress.Add(this, &ThisClass::NextPage);
    m_next->SetSize(70);
    m_next->SetMargin(Margin(4, 0, 0, 0));
    m_back = new Controls::Button(controls);
    m_back->SetText("< Back");
    m_back->Dock(Position::Right);
    m_back->onPress.Add(this, &ThisClass::PreviousPage);
    m_back->SetSize(70);
    m_label = new Controls::Label(controls);
    m_label->Dock(Position::Fill);
    m_label->SetAlignment(Position::Left|Position::CenterV);
    m_label->SetText("Page 1 or 2");
}

void PageControl::SetPageCount(unsigned int iNum)
{
    if (iNum >= MaxPages)
        iNum = MaxPages;

    for (unsigned int i = 0; i < iNum; i++)
    {
        if (!m_pages[i])
        {
            m_pages[i] = new Controls::Base(this);
            m_pages[i]->Dock(Position::Fill);
        }
    }

    m_pageCount = iNum;
    // Setting to -1 to force the page to change
    m_currentPage = -1;
    HideAll();
    ShowPage(0);
}

void PageControl::HideAll()
{
    for (int i = 0; i < MaxPages; i++)
    {
        if (!m_pages[i])
            continue;

        m_pages[i]->Hide();
    }
}

void PageControl::ShowPage(unsigned int i)
{
    if (m_currentPage == i)
        return;

    if (m_pages[i])
    {
        m_pages[i]->Show();
        m_pages[i]->Dock(Position::Fill);
    }

    m_currentPage = i;
    m_back->SetDisabled(m_currentPage == 0);
    m_next->SetDisabled(m_currentPage >= m_pageCount);
    m_label->SetText(Utility::Format("Page %i of %i", m_currentPage+1, m_pages));

    if (GetUseFinishButton())
    {
        const bool bFinished = m_currentPage >= m_pageCount-1;
        m_next->SetHidden(bFinished);
        m_finish->SetHidden(!bFinished);
    }

    {
        Event::Info info(this);
        info.Integer = i;
        info.Control = m_pages[i];
        onPageChanged.Call(this, info);
    }
}

Controls::Base* PageControl::GetPage(unsigned int i)
{
    return m_pages[i];
}

Controls::Base* PageControl::GetCurrentPage()
{
    return GetPage(GetPageNumber());
}

void PageControl::NextPage(Event::Info)
{
    if (m_currentPage >= m_pageCount-1)
        return;

    if (m_pages[m_currentPage])
    {
        m_pages[m_currentPage]->Dock(Position::None);
        Anim::Add(m_pages[m_currentPage],
                  new Anim::Pos::X(m_pages[m_currentPage]->X(), Width()* -1, 0.2f, true, 0.0f,
                                   -1));
    }

    ShowPage(m_currentPage+1);

    if (m_pages[m_currentPage])
    {
        m_pages[m_currentPage]->Dock(Position::None);
        Anim::Add(m_pages[m_currentPage], new Anim::Pos::X(Width(), 0, 0.2f, false, 0.0f, -1));
    }
}

void PageControl::PreviousPage(Event::Info)
{
    if (m_currentPage == 0)
        return;

    if (m_pages[m_currentPage])
    {
        m_pages[m_currentPage]->Dock(Position::None);
        Anim::Add(m_pages[m_currentPage],
                  new Anim::Pos::X(m_pages[m_currentPage]->X(), Width(), 0.3f, true, 0.0f, -1));
    }

    ShowPage(m_currentPage-1);

    if (m_pages[m_currentPage])
    {
        m_pages[m_currentPage]->Dock(Position::None);
        Anim::Add(m_pages[m_currentPage],
                  new Anim::Pos::X(Width()* -1, 0, 0.3f, false, 0.0f, -1));
    }
}

void PageControl::Finish(Event::Info)
{
    onFinish.Call(this);
}
