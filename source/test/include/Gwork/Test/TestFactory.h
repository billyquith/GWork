/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#pragma once
#ifndef GWK_TEST_TESTFACTORY_H
#define GWK_TEST_TESTFACTORY_H

#include <Gwork/Test/Test.h>


class TestFactory : public TestCategory
{
public:
    GWK_CONTROL(TestFactory, TestCategory);
    
private:

    void OnCategorySelect(Gwk::Event::Info info);

    Gwk::Controls::Base *m_lastControl;
    
};


#endif // ifndef GWK_TEST_TESTFACTORY_H
