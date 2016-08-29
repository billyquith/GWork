# Gwork docs
# Copyright (c) 2015-2016 Billy Quith
#
# Portions of this file come from Ponder:
# 
## The MIT License (MIT)
## 
## Copyright (c) 2009-2014 TEGESO/TEGESOFT
## Copyright (c) 2015-2016 Billy Quith
## 
## Permission is hereby granted, free of charge, to any person obtaining a copy
## of this software and associated documentation files (the "Software"), to deal
## in the Software without restriction, including without limitation the rights
## to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
## copies of the Software, and to permit persons to whom the Software is
## furnished to do so, subject to the following conditions:
## 
## The above copyright notice and this permission notice shall be included in
## all copies or substantial portions of the Software.
## 
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
## AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
## OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
## THE SOFTWARE.


find_package(Doxygen)

if(DOXYGEN_FOUND)
    
    set(DOXYGEN_INPUT_DIR ${GWK_SOURCE_DIR}/source)
       
    set(DOXYGEN_OUTPUT_TYPE html)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/doc/)
    
    set(DOXYGEN_CONFIG_FILENAME "Gwork.doxcfg")
    set(DOXYGEN_CONFIG_FILE ${GWK_SOURCE_DIR}/docs/${DOXYGEN_CONFIG_FILENAME})

    if(WIN32)
        set(DOXYGEN_GENERATE_MAN NO)
        find_program(HHC_PROGRAM 
            NAMES hhc.exe 
            PATHS "c:/Program Files/HTML Help Workshop"
            DOC "HTML Help Compiler program")
        if(HHC_PROGRAM)
            set(DOXYGEN_GENERATE_HTMLHELP YES)
        else()
            set(DOXYGEN_GENERATE_HTMLHELP NO)
        endif()
    else()
        set(DOXYGEN_GENERATE_MAN NO)
        set(DOXYGEN_GENERATE_HTMLHELP NO)
        set(HHC_PROGRAM)
    endif()

    # project code to document
    set(DOXYGEN_CODE_DIRS
        ${DOXYGEN_INPUT_DIR}/platform/include
        ${DOXYGEN_INPUT_DIR}/gwork/include
        ${DOXYGEN_INPUT_DIR}/util/include
        ${DOXYGEN_INPUT_DIR}/test/include
    )
    
    # directories to scan for documentation
    set(DOXYGEN_INPUT_DIRS ${DOXYGEN_CODE_DIRS} pages)

    # config substitute variables
    set(DOC_PROJECT_NAME "Gwork")
    set(DOC_PROJECT_BRIEF "GUI")
    set(DOC_LOGO "logo2.png")
    set(DOC_FILE_PATTERNS "*.c *.cpp *.h *.hpp *.dox")
    STRING(REPLACE ";" " " DOC_INPUT "${DOXYGEN_INPUT_DIRS}") # want space separated
    STRING(REPLACE ";" " " DOC_MACRO_DIRS "${DOXYGEN_CODE_DIRS}") # want space separated

    # subsitutes to make to patch up docs
    set(PREDEFINES 
        "GWK_CONTROL(CLS,BASE)=/** Constructor. */ CLS(Gwk::Control::Base *parent)"
        "GWK_CONTROL_INLINE(CLS,BASE)=/** Constructor. */ CLS(Gwk::Control::Base *parent)"
        "__cplusplus"
    )

    # format defines as required
    set(DOC_PREDEFINES "")
    foreach(_DEF ${PREDEFINES})
        set(DOC_PREDEFINES "${DOC_PREDEFINES} \\\n    \"${_DEF}\"")
    endforeach()

    # configure the source Doxyfile by copying it and replacing all @variables@
    configure_file(Doxyfile.in ${DOXYGEN_CONFIG_FILE} @ONLY)

    # copy necessary HTML doc files
    file(GLOB image_files images/*)
    foreach(file ${image_files})
        get_filename_component(filename ${file} NAME)
        configure_file(${file} ${DOXYGEN_OUTPUT_DIR}/${DOXYGEN_OUTPUT_TYPE}/${filename} COPYONLY)
    endforeach()

    # target setup
    add_custom_target(doc
        COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG_FILE}
        COMMAND ${CMAKE_COMMAND} -E echo "Done."
    )
    
    # install(DIRECTORY ${DOXYGEN_OUTPUT_DIR}
    #     DESTINATION ${INSTALL_MISC_DIR}/doc
    #     COMPONENT apidoc
    # )
endif()

