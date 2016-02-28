
include_directories(
    ${CMAKE_SOURCE_DIR}/source/gwork/include
    ${RENDERER_INC}
)

set(GWORK_HEADERS
    include/Gwork/Align.h
    include/Gwork/Anim.h
    include/Gwork/BaseRender.h
    include/Gwork/Config.h
    include/Gwork/ControlList.h
    include/Gwork/Controls.h
    include/Gwork/DragAndDrop.h
    include/Gwork/Events.h
    include/Gwork/Exports.h
    include/Gwork/Font.h
    include/Gwork/Gwork.h
    include/Gwork/Hook.h
    include/Gwork/InputHandler.h
    include/Gwork/Macros.h
    include/Gwork/Platform.h
    include/Gwork/Skin.h
    include/Gwork/Structures.h
    include/Gwork/Texture.h
    include/Gwork/ToolTip.h
    include/Gwork/UserData.h
    include/Gwork/Utility.h
    include/Gwork/WindowProvider.h
    # controls
    include/Gwork/Controls/Base.h
    include/Gwork/Controls/Button.h
    include/Gwork/Controls/Canvas.h
    include/Gwork/Controls/CheckBox.h
    include/Gwork/Controls/CollapsibleCategory.h
    include/Gwork/Controls/CollapsibleList.h
    include/Gwork/Controls/ColorControls.h
    include/Gwork/Controls/ColorPicker.h
    include/Gwork/Controls/ComboBox.h
    include/Gwork/Controls/CrossSplitter.h
    include/Gwork/Controls/DockBase.h
    include/Gwork/Controls/DockedTabControl.h
    include/Gwork/Controls/Dragger.h
    include/Gwork/Controls/FieldLabel.h
    include/Gwork/Controls/FilePicker.h
    include/Gwork/Controls/FolderPicker.h
    include/Gwork/Controls/GroupBox.h
    include/Gwork/Controls/HSVColorPicker.h
    include/Gwork/Controls/Highlight.h
    include/Gwork/Controls/HorizontalScrollBar.h
    include/Gwork/Controls/HorizontalSlider.h
    include/Gwork/Controls/ImagePanel.h
    include/Gwork/Controls/Label.h
    include/Gwork/Controls/LabelClickable.h
    include/Gwork/Controls/ListBox.h
    include/Gwork/Controls/Menu.h
    include/Gwork/Controls/MenuItem.h
    include/Gwork/Controls/MenuStrip.h
    include/Gwork/Controls/Modal.h
    include/Gwork/Controls/NumericUpDown.h
    include/Gwork/Controls/PageControl.h
    include/Gwork/Controls/ProgressBar.h
    include/Gwork/Controls/Properties.h
    include/Gwork/Controls/PropertyTree.h
    include/Gwork/Controls/RadioButton.h
    include/Gwork/Controls/RadioButtonController.h
    include/Gwork/Controls/Rectangle.h
    include/Gwork/Controls/ResizableControl.h
    include/Gwork/Controls/Resizer.h
    include/Gwork/Controls/RichLabel.h
    include/Gwork/Controls/ScrollBar.h
    include/Gwork/Controls/ScrollBarBar.h
    include/Gwork/Controls/ScrollBarButton.h
    include/Gwork/Controls/ScrollControl.h
    include/Gwork/Controls/Slider.h
    include/Gwork/Controls/SplitterBar.h
    include/Gwork/Controls/Splitters.h
    include/Gwork/Controls/StatusBar.h
    include/Gwork/Controls/TabButton.h
    include/Gwork/Controls/TabControl.h
    include/Gwork/Controls/TabStrip.h
    include/Gwork/Controls/TabTitleBar.h
    include/Gwork/Controls/Text.h
    include/Gwork/Controls/TextBox.h
    include/Gwork/Controls/ToolBar.h
    include/Gwork/Controls/TreeControl.h
    include/Gwork/Controls/TreeNode.h
    include/Gwork/Controls/VerticalScrollBar.h
    include/Gwork/Controls/VerticalSlider.h
    include/Gwork/Controls/WindowButtons.h
    include/Gwork/Controls/WindowCanvas.h
    include/Gwork/Controls/WindowControl.h
    # layout
    include/Gwork/Controls/Layout/Position.h
    include/Gwork/Controls/Layout/Table.h
    include/Gwork/Controls/Layout/Tile.h
    # property
    include/Gwork/Controls/Property/BaseProperty.h
    include/Gwork/Controls/Property/Checkbox.h
    include/Gwork/Controls/Property/ColorSelector.h
    include/Gwork/Controls/Property/ComboBox.h
    include/Gwork/Controls/Property/File.h
    include/Gwork/Controls/Property/Folder.h
    include/Gwork/Controls/Property/Text.h
    # dialogs
    include/Gwork/Controls/Dialogs/FileOpen.h
    include/Gwork/Controls/Dialogs/FileSave.h
    include/Gwork/Controls/Dialogs/FolderOpen.h
    include/Gwork/Controls/Dialogs/Query.h
    # input
    include/Gwork/Input/Allegro.h
    include/Gwork/Input/SDL13.h
    include/Gwork/Input/SDL2.h
    include/Gwork/Input/SFML.h
    include/Gwork/Input/Windows.h
    # skins
    include/Gwork/Skins/Simple.h
    include/Gwork/Skins/TexturedBase.h
    include/Gwork/Skins/Texturing.h
)

set(GWORK_SOURCES
    src/Anim.cpp
    src/BaseRender.cpp
    src/ControlList.cpp
    src/DragAndDrop.cpp
    src/Events.cpp
    src/Gwen.cpp
    src/Hook.cpp
    src/InputHandler.cpp
    src/Skin.cpp
    src/ToolTip.cpp
    src/Utility.cpp
    src/Controls/Base.cpp
    src/Controls/Button.cpp
    src/Controls/Canvas.cpp
    src/Controls/CheckBox.cpp
    src/Controls/CollapsibleCategory.cpp
    src/Controls/ColorControls.cpp
    src/Controls/ColorPicker.cpp
    src/Controls/ComboBox.cpp
    src/Controls/CrossSplitter.cpp
    src/Controls/DockBase.cpp
    src/Controls/DockedTabControl.cpp
    src/Controls/Dragger.cpp
    src/Controls/GroupBox.cpp
    src/Controls/HSVColorPicker.cpp
    src/Controls/HorizontalScrollBar.cpp
    src/Controls/HorizontalSlider.cpp
    src/Controls/ImagePanel.cpp
    src/Controls/Label.cpp
    src/Controls/LabelClickable.cpp
    src/Controls/ListBox.cpp
    src/Controls/Menu.cpp
    src/Controls/MenuItem.cpp
    src/Controls/MenuStrip.cpp
    src/Controls/NumericUpDown.cpp
    src/Controls/PageControl.cpp
    src/Controls/ProgressBar.cpp
    src/Controls/Properties.cpp
    src/Controls/PropertyTree.cpp
    src/Controls/RadioButton.cpp
    src/Controls/RadioButtonController.cpp
    src/Controls/Rectangle.cpp
    src/Controls/ResizableControl.cpp
    src/Controls/Resizer.cpp
    src/Controls/RichLabel.cpp
    src/Controls/ScrollBar.cpp
    src/Controls/ScrollBarBar.cpp
    src/Controls/ScrollBarButton.cpp
    src/Controls/ScrollControl.cpp
    src/Controls/Slider.cpp
    src/Controls/SplitterBar.cpp
    src/Controls/TabButton.cpp
    src/Controls/TabControl.cpp
    src/Controls/TabStrip.cpp
    src/Controls/Text.cpp
    src/Controls/TextBox.cpp
    src/Controls/TextBoxNumeric.cpp
    src/Controls/TreeControl.cpp
    src/Controls/TreeNode.cpp
    src/Controls/VerticalScrollBar.cpp
    src/Controls/VerticalSlider.cpp
    src/Controls/WindowCanvas.cpp
    src/Controls/WindowControl.cpp
    # dialog
    src/Controls/Dialog/FileOpen.cpp
    src/Controls/Dialog/FileSave.cpp
    src/Controls/Dialog/FolderOpen.cpp
    src/Controls/Dialog/Query.cpp
)

set(GWORK_RENDER_SOURCES
    include/Gwork/Renderers/${RENDERER_NAME}.h
    Renderers/${RENDERER_NAME}/${RENDERER_NAME}.cpp
    src/Platforms/${BUILD_PLATFORM}.cpp
)

set(GWORK_TEST_SOURCES
    include/Gwork/UnitTest/UnitTest.h
    UnitTest/Button.cpp
    UnitTest/Checkbox.cpp
    UnitTest/CollapsibleList.cpp
    UnitTest/ColorPicker.cpp
    UnitTest/ComboBox.cpp
    UnitTest/CrossSplitter.cpp
    UnitTest/GroupBox.cpp
    UnitTest/ImagePanel.cpp
    UnitTest/Label.cpp
    UnitTest/LabelMultiline.cpp
    UnitTest/ListBox.cpp
    UnitTest/MenuStrip.cpp
    UnitTest/Numeric.cpp
    UnitTest/PageControl.cpp
    UnitTest/ProgressBar.cpp
    UnitTest/Properties.cpp
    UnitTest/RadioButton.cpp
    UnitTest/ReadMe.txt
    UnitTest/ScrollControl.cpp
    UnitTest/Slider.cpp
    UnitTest/StatusBar.cpp
    UnitTest/TabControl.cpp
    UnitTest/TextBox.cpp
    UnitTest/TreeControl.cpp
    UnitTest/UnitTest.cpp
    UnitTest/Window.cpp
)

set(GWORK_UTIL_SOURCES
    ../util/src/ControlFactory/Base_Factory.cpp
    ../util/src/ControlFactory/Button_Factory.cpp
    ../util/src/ControlFactory/CheckBox_Factory.cpp
    ../util/src/ControlFactory/ComboBox_Factory.cpp
    ../util/src/ControlFactory/ControlFactory.cpp
    ../util/src/ControlFactory/DesignerCanvas_Factory.cpp
    ../util/src/ControlFactory/FilePicker_Factory.cpp
    ../util/src/ControlFactory/FolderPicker_Factory.cpp
    ../util/src/ControlFactory/Image_Factory.cpp
    ../util/src/ControlFactory/Label_Factory.cpp
    ../util/src/ControlFactory/ListBox_Factory.cpp
    ../util/src/ControlFactory/MenuStrip_Factory.cpp
    ../util/src/ControlFactory/PageControl_Factory.cpp
    ../util/src/ControlFactory/ProgressBar_Factory.cpp
    ../util/src/ControlFactory/Rectangle_Factory.cpp
    ../util/src/ControlFactory/Slider_Factory.cpp
    ../util/src/ControlFactory/TextBox_Factory.cpp
    ../util/src/ControlFactory/VerticalSplitter_Factory.cpp
    ../util/src/ImportExport/Base.cpp
    ../util/src/ImportExport/DesignerFormat.cpp
    ../util/src/ImportExport/DesignerFormat.h
    ../util/src/ImportExport/GwenUtil.cpp
    ../util/src/ImportExport/GwenUtil.h
)

set(GWORK_HEADER_FOLDER "Headers")
set(GWORK_SOURCE_FOLDER "Source")

# header folder groups
source_group("${GWORK_HEADER_FOLDER}"
             REGULAR_EXPRESSION "include/Gwork/[^/]+\\.h")
source_group("${GWORK_HEADER_FOLDER}\\Controls"
             REGULAR_EXPRESSION "include/Gwork/Controls/[^/]+\\.h")
source_group("${GWORK_HEADER_FOLDER}\\Controls\\Dialogs"
             REGULAR_EXPRESSION "include/Gwork/Controls/Dialogs/[^/]+\\.h")
source_group("${GWORK_HEADER_FOLDER}\\Input"
             REGULAR_EXPRESSION "include/Gwork/Input/[^/]+\\.h")
source_group("${GWORK_HEADER_FOLDER}\\Controls\\Layout"
             REGULAR_EXPRESSION "include/Gwork/Controls/Layout/[^/]+\\.h")
source_group("${GWORK_HEADER_FOLDER}\\Controls\\Property"
             REGULAR_EXPRESSION "include/Gwork/Controls/Property/[^/]+\\.h")
source_group("${GWORK_HEADER_FOLDER}\\Skins"
             REGULAR_EXPRESSION "include/Gwork/Skins/[^/]+\\.h")

# source folder groups
source_group("${GWORK_SOURCE_FOLDER}"
             REGULAR_EXPRESSION "[^/]+\\.cpp")
source_group("${GWORK_SOURCE_FOLDER}\\Controls"
             REGULAR_EXPRESSION "src/Controls/[^/]+\\.cpp")
source_group("${GWORK_SOURCE_FOLDER}\\Controls\\Dialogs"
             REGULAR_EXPRESSION "src/Controls/Dialogs/[^/]+\\.cpp")

# Gwork library
add_library(Gwork STATIC ${GWORK_HEADERS} ${GWORK_SOURCES})
target_compile_features(Gwork PUBLIC cxx_override)

install(TARGETS Gwork
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/Gwork
        DESTINATION include)

# Gwork renderer & platform library
add_library(Gwork${RENDERER_NAME} STATIC ${GWORK_RENDER_SOURCES})
target_compile_features(Gwork${RENDERER_NAME} PUBLIC cxx_override)
target_link_libraries(Gwork${RENDERER_NAME} Gwork)

include_directories(
    ${CMAKE_SOURCE_DIR}/util/include
)

# Gwork utils
add_library(GworkUtil STATIC ${GWORK_UTIL_SOURCES})
target_compile_features(GworkUtil PUBLIC cxx_override)
target_link_libraries(GworkUtil Gwork)

install(TARGETS GworkUtil
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/util/include/GworkUtil
        DESTINATION include)

# Test library
if(BUILD_TEST)
    add_library(GworkTest STATIC ${GWORK_TEST_SOURCES})
    target_compile_features(GworkTest PUBLIC cxx_override)
    target_link_libraries(GworkTest Gwork Gwork${RENDERER_NAME})

    install(TARGETS GworkTest
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
endif(BUILD_TEST)

# Sample executable
if(BUILD_SAMPLE)
    set(SAMPLE_SOURCES
        "Samples/${RENDERER_NAME}/${RENDERER_NAME}Sample.cpp")

    add_executable(Gwork${RENDERER_NAME}Sample ${SAMPLE_SOURCES})
    target_compile_features(Gwork${RENDERER_NAME}Sample PRIVATE cxx_override)
    target_link_libraries(Gwork${RENDERER_NAME}Sample
                          Gwork Gwork${RENDERER_NAME} GworkUtil ${RENDERER_LIB} ${PONDER_LIBRARIES})

    if(BUILD_TEST)
        target_link_libraries(Gwork${RENDERER_NAME}Sample GworkTest)
    endif(BUILD_TEST)

    install(TARGETS Gwork${RENDERER_NAME}Sample
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
endif(BUILD_SAMPLE)


      

