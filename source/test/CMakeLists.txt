 # Gwork library

include_directories(
    ${CMAKE_SOURCE_DIR}/source/platform/include
    ${CMAKE_SOURCE_DIR}/source/gwork/include
    ${GWK_RENDER_INCLUDES}
    ${CMAKE_SOURCE_DIR}/source/util/include
    ${CMAKE_SOURCE_DIR}/source/test/include
)

set(GWK_TEST_SOURCES
    include/Gwork/Test/Test.h
    include/Gwork/Test/TestAPI.h
    include/Gwork/Test/TestFactory.h
    source/Test.cpp
    source/api/Button.cpp
    source/api/Checkbox.cpp
    source/api/CollapsibleList.cpp
    source/api/ColorPicker.cpp
    source/api/ComboBox.cpp
    source/api/CrossSplitter.cpp
    source/api/GroupBox.cpp
    source/api/ImagePanel.cpp
    source/api/Label.cpp
    source/api/LabelMultiline.cpp
    source/api/ListBox.cpp
    source/api/MenuStrip.cpp
    source/api/Numeric.cpp
    source/api/PageControl.cpp
    source/api/ProgressBar.cpp
    source/api/Properties.cpp
    source/api/RadioButton.cpp
    source/api/ScrollControl.cpp
    source/api/Slider.cpp
    source/api/StatusBar.cpp
    source/api/TabControl.cpp
    source/api/TextBox.cpp
    source/api/TestAPI.cpp
    source/api/TreeControl.cpp
    source/api/Window.cpp
    source/factory/TestFactory.cpp
    source/factory/ReflectButton.cpp
)

# header folder groups
source_group("${GWK_HEADER_FOLDER}"
             REGULAR_EXPRESSION "include/Gwork/Test/.*\\.h")
source_group("${GWK_SOURCE_FOLDER}\\API"
             REGULAR_EXPRESSION "source/api/.*\\.cpp")
source_group("${GWK_SOURCE_FOLDER}\\Factory"
             REGULAR_EXPRESSION "source/factory/.*\\.cpp")
source_group("${GWK_SOURCE_FOLDER}"
             REGULAR_EXPRESSION "source/[^/]+\\.cpp")

# Test library
if(BUILD_TEST)
    add_library(GworkTest STATIC ${GWK_TEST_SOURCES})
    target_compile_features(GworkTest PUBLIC cxx_override)
    target_link_libraries(GworkTest Gwork Gwork${GWK_RENDER_NAME})

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/Gwork
            DESTINATION include)

    install(TARGETS GworkTest
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
endif(BUILD_TEST)
