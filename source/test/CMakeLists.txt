# Gwork library

include_directories(
    ${CMAKE_SOURCE_DIR}/source/gwork/include
    ${RENDERER_INC}
    ${CMAKE_SOURCE_DIR}/source/test/include
)

set(GWORK_TEST_SOURCES
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

# set(GWORK_HEADER_FOLDER "Headers")
# set(GWORK_SOURCE_FOLDER "Source")
#
# # header folder groups
# source_group("${GWORK_HEADER_FOLDER}"
#              REGULAR_EXPRESSION "include/Gwork/[^/]+\\.h")
# source_group("${GWORK_HEADER_FOLDER}\\Controls"
#              REGULAR_EXPRESSION "include/Gwork/Controls/[^/]+\\.h")
# source_group("${GWORK_HEADER_FOLDER}\\Controls\\Dialogs"
#              REGULAR_EXPRESSION "include/Gwork/Controls/Dialogs/[^/]+\\.h")
# source_group("${GWORK_HEADER_FOLDER}\\Input"
#              REGULAR_EXPRESSION "include/Gwork/Input/[^/]+\\.h")
# source_group("${GWORK_HEADER_FOLDER}\\Controls\\Layout"
#              REGULAR_EXPRESSION "include/Gwork/Controls/Layout/[^/]+\\.h")
# source_group("${GWORK_HEADER_FOLDER}\\Controls\\Property"
#              REGULAR_EXPRESSION "include/Gwork/Controls/Property/[^/]+\\.h")
# source_group("${GWORK_HEADER_FOLDER}\\Skins"
#              REGULAR_EXPRESSION "include/Gwork/Skins/[^/]+\\.h")
#
# # source folder groups
# source_group("${GWORK_SOURCE_FOLDER}"
#              REGULAR_EXPRESSION "[^/]+\\.cpp")
# source_group("${GWORK_SOURCE_FOLDER}\\Controls"
#              REGULAR_EXPRESSION "source/Controls/[^/]+\\.cpp")
# source_group("${GWORK_SOURCE_FOLDER}\\Controls\\Dialogs"
#              REGULAR_EXPRESSION "source/Controls/Dialogs/[^/]+\\.cpp")

# Test library
if(BUILD_TEST)
    add_library(GworkTest STATIC ${GWORK_TEST_SOURCES})
    target_compile_features(GworkTest PUBLIC cxx_override)
    target_link_libraries(GworkTest Gwork Gwork${RENDERER_NAME})

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/Gwork
            DESTINATION include)

    install(TARGETS GworkTest
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
endif(BUILD_TEST)
