
include_directories(
    ${CMAKE_SOURCE_DIR}/gwen/include
    ${RENDERER_INC}
)

set(GWORK_HEADERS
    include/Gwen/Align.h
    include/Gwen/Anim.h
    include/Gwen/BaseRender.h
    include/Gwen/Config.h
    include/Gwen/ControlList.h
    include/Gwen/Controls.h
    include/Gwen/DragAndDrop.h
    include/Gwen/Events.h
    include/Gwen/Exports.h
    include/Gwen/Font.h
    include/Gwen/Gwen.h
    include/Gwen/Hook.h
    include/Gwen/InputHandler.h
    include/Gwen/Macros.h
    include/Gwen/Platform.h
    include/Gwen/Skin.h
    include/Gwen/Structures.h
    include/Gwen/Texture.h
    include/Gwen/ToolTip.h
    include/Gwen/UserData.h
    include/Gwen/Utility.h
    include/Gwen/WindowProvider.h
    # controls
    include/Gwen/Controls/Base.h
    include/Gwen/Controls/Button.h
    include/Gwen/Controls/Canvas.h
    include/Gwen/Controls/CheckBox.h
    include/Gwen/Controls/CollapsibleCategory.h
    include/Gwen/Controls/CollapsibleList.h
    include/Gwen/Controls/ColorControls.h
    include/Gwen/Controls/ColorPicker.h
    include/Gwen/Controls/ComboBox.h
    include/Gwen/Controls/CrossSplitter.h
    include/Gwen/Controls/DockBase.h
    include/Gwen/Controls/DockedTabControl.h
    include/Gwen/Controls/Dragger.h
    include/Gwen/Controls/FieldLabel.h
    include/Gwen/Controls/FilePicker.h
    include/Gwen/Controls/FolderPicker.h
    include/Gwen/Controls/GroupBox.h
    include/Gwen/Controls/HSVColorPicker.h
    include/Gwen/Controls/Highlight.h
    include/Gwen/Controls/HorizontalScrollBar.h
    include/Gwen/Controls/HorizontalSlider.h
    include/Gwen/Controls/ImagePanel.h
    include/Gwen/Controls/Label.h
    include/Gwen/Controls/LabelClickable.h
    include/Gwen/Controls/ListBox.h
    include/Gwen/Controls/Menu.h
    include/Gwen/Controls/MenuItem.h
    include/Gwen/Controls/MenuStrip.h
    include/Gwen/Controls/Modal.h
    include/Gwen/Controls/NumericUpDown.h
    include/Gwen/Controls/PageControl.h
    include/Gwen/Controls/ProgressBar.h
    include/Gwen/Controls/Properties.h
    include/Gwen/Controls/PropertyTree.h
    include/Gwen/Controls/RadioButton.h
    include/Gwen/Controls/RadioButtonController.h
    include/Gwen/Controls/Rectangle.h
    include/Gwen/Controls/ResizableControl.h
    include/Gwen/Controls/Resizer.h
    include/Gwen/Controls/RichLabel.h
    include/Gwen/Controls/ScrollBar.h
    include/Gwen/Controls/ScrollBarBar.h
    include/Gwen/Controls/ScrollBarButton.h
    include/Gwen/Controls/ScrollControl.h
    include/Gwen/Controls/Slider.h
    include/Gwen/Controls/SplitterBar.h
    include/Gwen/Controls/Splitters.h
    include/Gwen/Controls/StatusBar.h
    include/Gwen/Controls/TabButton.h
    include/Gwen/Controls/TabControl.h
    include/Gwen/Controls/TabStrip.h
    include/Gwen/Controls/TabTitleBar.h
    include/Gwen/Controls/Text.h
    include/Gwen/Controls/TextBox.h
    include/Gwen/Controls/ToolBar.h
    include/Gwen/Controls/TreeControl.h
    include/Gwen/Controls/TreeNode.h
    include/Gwen/Controls/VerticalScrollBar.h
    include/Gwen/Controls/VerticalSlider.h
    include/Gwen/Controls/WindowButtons.h
    include/Gwen/Controls/WindowCanvas.h
    include/Gwen/Controls/WindowControl.h
    # layout
    include/Gwen/Controls/Layout/Position.h
    include/Gwen/Controls/Layout/Table.h
    include/Gwen/Controls/Layout/Tile.h
    # property
    include/Gwen/Controls/Property/BaseProperty.h
    include/Gwen/Controls/Property/Checkbox.h
    include/Gwen/Controls/Property/ColorSelector.h
    include/Gwen/Controls/Property/ComboBox.h
    include/Gwen/Controls/Property/File.h
    include/Gwen/Controls/Property/Folder.h
    include/Gwen/Controls/Property/Text.h
    # dialogs
    include/Gwen/Controls/Dialogs/FileOpen.h
    include/Gwen/Controls/Dialogs/FileSave.h
    include/Gwen/Controls/Dialogs/FolderOpen.h
    include/Gwen/Controls/Dialogs/Query.h    
    # input
    include/Gwen/Input/Allegro.h
    include/Gwen/Input/SDL13.h
    include/Gwen/Input/SDL2.h
    include/Gwen/Input/SFML.h
    include/Gwen/Input/Windows.h
    # skins
    include/Gwen/Skins/Simple.h
    include/Gwen/Skins/TexturedBase.h
    include/Gwen/Skins/Texturing.h
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
    include/Gwen/Renderers/${RENDERER_NAME}.h
    Renderers/${RENDERER_NAME}/${RENDERER_NAME}.cpp
    src/Platforms/${BUILD_PLATFORM}.cpp
)

set(GWORK_TEST_SOURCES
    include/Gwen/UnitTest/UnitTest.h
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
             REGULAR_EXPRESSION "include/Gwen/[^/]+\\.h")             
source_group("${GWORK_HEADER_FOLDER}\\Controls"
             REGULAR_EXPRESSION "include/Gwen/Controls/[^/]+\\.h")             
source_group("${GWORK_HEADER_FOLDER}\\Controls\\Dialogs"
             REGULAR_EXPRESSION "include/Gwen/Controls/Dialogs/[^/]+\\.h")             
source_group("${GWORK_HEADER_FOLDER}\\Input"
             REGULAR_EXPRESSION "include/Gwen/Input/[^/]+\\.h")
source_group("${GWORK_HEADER_FOLDER}\\Controls\\Layout"
             REGULAR_EXPRESSION "include/Gwen/Controls/Layout/[^/]+\\.h")
source_group("${GWORK_HEADER_FOLDER}\\Controls\\Property"
             REGULAR_EXPRESSION "include/Gwen/Controls/Property/[^/]+\\.h")             
source_group("${GWORK_HEADER_FOLDER}\\Skins"
             REGULAR_EXPRESSION "include/Gwen/Skins/[^/]+\\.h")

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

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/Gwen
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

install(DIRECTORY ${CMAKE_SOURCE_DIR}/util/include/GwenUtil
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
      

