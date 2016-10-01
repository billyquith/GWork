# Gwork util library

find_package(Ponder REQUIRED) # TODO - temporary hack whilst testing

include_directories(
    ${GWK_SOURCE_DIR}/source/platform/include
    ${GWK_SOURCE_DIR}/source/gwork/include
    ${GWK_RENDER_INCLUDES}
    ${CMAKE_SOURCE_DIR}/source/util/include
    ${PONDER_INCLUDE_DIRS}
)

set(GWK_UTIL_SOURCES
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
    source/ImportExport/DesignerFormat.h        # private
    source/ImportExport/DesignerFormat.cpp
    source/ImportExport/GworkUtil.h             # private
    source/ImportExport/GworkUtil.cpp
)

# header folder groups
source_group("${GWK_HEADER_FOLDER}\\ControlFactory"
             FILES "include/Gwork/Util/ControlFactory.h")
source_group("${GWK_HEADER_FOLDER}\\ImportExport"
             FILES "include/Gwork/Util/ImportExport.h")

# source folder groups
source_group("${GWK_SOURCE_FOLDER}\\ControlFactory"
             REGULAR_EXPRESSION "source/ControlFactory/.*")
source_group("${GWK_SOURCE_FOLDER}\\ImportExport"
             REGULAR_EXPRESSION "source/ImportExport/.*")

# Gwork utils
add_library(GworkUtil STATIC ${GWK_UTIL_SOURCES})
target_compile_features(GworkUtil PUBLIC cxx_override)
target_link_libraries(GworkUtil Gwork)

install(DIRECTORY ${GWK_SOURCE_DIR}/util/include/Gwork
        DESTINATION include)

install(TARGETS GworkUtil
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)


