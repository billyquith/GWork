# Gwork library

include_directories(
    ${CMAKE_SOURCE_DIR}/source/platform/include
    ${CMAKE_SOURCE_DIR}/source/gwork/include
    ${RENDERER_INC}
    ${CMAKE_SOURCE_DIR}/source/test/include
)

set(GWK_TEST_SOURCES
    include/Gwork/Test/Test.h
    source/Button.cpp
    source/Checkbox.cpp
    source/CollapsibleList.cpp
    source/ColorPicker.cpp
    source/ComboBox.cpp
    source/CrossSplitter.cpp
    source/GroupBox.cpp
    source/ImagePanel.cpp
    source/Label.cpp
    source/LabelMultiline.cpp
    source/ListBox.cpp
    source/MenuStrip.cpp
    source/Numeric.cpp
    source/PageControl.cpp
    source/ProgressBar.cpp
    source/Properties.cpp
    source/RadioButton.cpp
    source/ScrollControl.cpp
    source/Slider.cpp
    source/StatusBar.cpp
    source/TabControl.cpp
    source/TextBox.cpp
    source/Test.cpp
    source/TreeControl.cpp
    source/Window.cpp
)

set(GWK_HEADER_FOLDER "Headers")
set(GWK_SOURCE_FOLDER "Source")

# header folder groups
source_group("${GWK_HEADER_FOLDER}"
             REGULAR_EXPRESSION "include/Gwork/Test/.*\\.h")
source_group("${GWK_SOURCE_FOLDER}"
             REGULAR_EXPRESSION "source/.*\\.cpp")

# Test library
if(BUILD_TEST)
    add_library(GworkTest STATIC ${GWK_TEST_SOURCES})
    target_compile_features(GworkTest PUBLIC cxx_override)
    target_link_libraries(GworkTest Gwork Gwork${RENDERER_NAME})

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/Gwork
            DESTINATION include)

    install(TARGETS GworkTest
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
endif(BUILD_TEST)
