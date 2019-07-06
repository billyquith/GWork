/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-17 Billy Quith
 *  See license in Gwork.h
 */


#pragma once
#ifndef GWK_TEST_TESTREFLECT_H
#define GWK_TEST_TESTREFLECT_H

#include <Gwork/Test/Test.h>

namespace Gwk {
    namespace Test {

        //
        /// Here we test Ponder reflection
        ///  - We use the runtime API of Ponder to create controls.
        //
        class TestReflect : public TestCategory
        {
        public:
            GWK_CONTROL(TestReflect, TestCategory);
    
        private:

            void OnCategorySelect(Gwk::Event::Info info);

            Gwk::Controls::Base *m_lastControl;    
        };

    }
}

#endif // ifndef GWK_TEST_TESTREFLECT_H
