# Gwork util library

include_directories(
    ${CMAKE_SOURCE_DIR}/source/gwork/include
    ${RENDERER_INC}
    ${CMAKE_SOURCE_DIR}/source/util/include
)

set(GWORK_UTIL_SOURCES
    # control factory
    include/Gwork/Util/ControlFactory.h
    source/ControlFactory/Base_Factory.cpp
    source/ControlFactory/Button_Factory.cpp
    source/ControlFactory/CheckBox_Factory.cpp
    source/ControlFactory/ComboBox_Factory.cpp
    source/ControlFactory/ControlFactory.cpp
    source/ControlFactory/DesignerCanvas_Factory.cpp
    source/ControlFactory/FilePicker_Factory.cpp
    source/ControlFactory/FolderPicker_Factory.cpp
    source/ControlFactory/Image_Factory.cpp
    source/ControlFactory/Label_Factory.cpp
    source/ControlFactory/ListBox_Factory.cpp
    source/ControlFactory/MenuStrip_Factory.cpp
    source/ControlFactory/PageControl_Factory.cpp
    source/ControlFactory/ProgressBar_Factory.cpp
    source/ControlFactory/Rectangle_Factory.cpp
    source/ControlFactory/Slider_Factory.cpp
    source/ControlFactory/TextBox_Factory.cpp
    source/ControlFactory/VerticalSplitter_Factory.cpp
    # import/export
    include/Gwork/Util/ImportExport.h
    source/ImportExport/Base.cpp
    source/ImportExport/DesignerFormat.cpp
    source/ImportExport/DesignerFormat.h
    source/ImportExport/GworkUtil.h         # private
    source/ImportExport/GworkUtil.cpp
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

# Gwork utils
add_library(GworkUtil STATIC ${GWORK_UTIL_SOURCES})
target_compile_features(GworkUtil PUBLIC cxx_override)
target_link_libraries(GworkUtil Gwork)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/util/include/Gwork
        DESTINATION include)

install(TARGETS GworkUtil
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)


