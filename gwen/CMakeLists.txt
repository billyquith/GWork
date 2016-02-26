
include_directories(
    ${CMAKE_SOURCE_DIR}/gwen/include
    ${RENDERER_INC}
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
    src/Controls/Dialog/FileOpen.cpp
    src/Controls/Dialog/FileSave.cpp
    src/Controls/Dialog/FolderOpen.cpp
    src/Controls/Dialog/Query.cpp
)

set(GWORK_RENDER_SOURCES
    Renderers/${RENDERER_NAME}/${RENDERER_NAME}.cpp     # renderer
    src/Platforms/${BUILD_PLATFORM}.cpp                 # platform
)

set(GWORK_TEST_SOURCES
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

# Gwork library
add_library(Gwork STATIC ${GWORK_SOURCES})
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
      

