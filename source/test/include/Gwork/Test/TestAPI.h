/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#pragma once
#ifndef GWK_TEST_TESTAPI_H
#define GWK_TEST_TESTAPI_H

#include <Gwork/Test/Test.h>

//
// Here we test the Gwork C++ API.
//  - All controls created with C++.
//
class TestAPI : public TestCategory
{
public:
    GWK_CONTROL(TestAPI, TestCategory);
    
private:

    void OnCategorySelect(Gwk::Event::Info info);

    Gwk::Controls::Base *m_lastControl;
    
};


#endif // ifndef GWK_TEST_TESTAPI_H
