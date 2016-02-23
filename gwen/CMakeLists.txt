
include_directories(
    ${CMAKE_SOURCE_DIR}/gwen/include
    ${RENDERER_INC}
)

set(GWORK_SOURCES 
    src/ToolTip.cpp
    src/ControlList.cpp
    src/Anim.cpp
    src/events.cpp
    src/Utility.cpp
    src/inputhandler.cpp
    src/DragAndDrop.cpp
    src/Hook.cpp
    src/Gwen.cpp
    src/BaseRender.cpp
    src/Skin.cpp
    src/Controls/TabButton.cpp
    src/Controls/ScrollBar.cpp
    src/Controls/Button.cpp
    src/Controls/Dragger.cpp
    src/Controls/TextBoxNumeric.cpp
    src/Controls/Resizer.cpp
    src/Controls/VerticalScrollBar.cpp
    src/Controls/ResizableControl.cpp
    src/Controls/ScrollControl.cpp
    src/Controls/ImagePanel.cpp
    src/Controls/DockBase.cpp
    src/Controls/Label.cpp
    src/Controls/VerticalSlider.cpp
    src/Controls/PageControl.cpp
    src/Controls/MenuItem.cpp
    src/Controls/TabStrip.cpp
    src/Controls/Text.cpp
    src/Controls/RadioButtonController.cpp
    src/Controls/CrossSplitter.cpp
    src/Controls/HSVColorPicker.cpp
    src/Controls/HorizontalScrollBar.cpp
    src/Controls/NumericUpDown.cpp
    src/Controls/ListBox.cpp
    src/Controls/SplitterBar.cpp
    src/Controls/ColorControls.cpp
    src/Controls/Rectangle.cpp
    src/Controls/CollapsibleCategory.cpp
    src/Controls/Properties.cpp
    src/Controls/Base.cpp
    src/Controls/LabelClickable.cpp
    src/Controls/ColorPicker.cpp
    src/Controls/TreeControl.cpp
    src/Controls/Slider.cpp
    src/Controls/ProgressBar.cpp
    src/Controls/CheckBox.cpp
    src/Controls/TreeNode.cpp
    src/Controls/Canvas.cpp
    src/Controls/PropertyTree.cpp
    src/Controls/GroupBox.cpp
    src/Controls/ComboBox.cpp
    src/Controls/DockedTabControl.cpp
    src/Controls/WindowCanvas.cpp
    src/Controls/Menu.cpp
    src/Controls/HorizontalSlider.cpp
    src/Controls/ScrollBarButton.cpp
    src/Controls/MenuStrip.cpp
    src/Controls/ScrollBarBar.cpp
    src/Controls/TabControl.cpp
    src/Controls/WindowControl.cpp
    src/Controls/RichLabel.cpp
    src/Controls/RadioButton.cpp
    src/Controls/TextBox.cpp
    src/Controls/Dialog/Query.cpp
    src/Controls/Dialog/FolderOpen.cpp
    src/Controls/Dialog/FileSave.cpp
    src/Controls/Dialog/FileOpen.cpp
    src/Platforms/${BUILD_PLATFORM}.cpp     # platform
)

set(GWORK_TEST_SOURCES
    UnitTest/CollapsibleList.cpp
    UnitTest/Button.cpp
    UnitTest/ScrollControl.cpp
    UnitTest/ImagePanel.cpp
    UnitTest/Label.cpp
    UnitTest/PageControl.cpp
    UnitTest/CrossSplitter.cpp
    UnitTest/ReadMe.txt
    UnitTest/ListBox.cpp
    UnitTest/Numeric.cpp
    UnitTest/Properties.cpp
    UnitTest/Window.cpp
    UnitTest/StatusBar.cpp
    UnitTest/ColorPicker.cpp
    UnitTest/TreeControl.cpp
    UnitTest/Slider.cpp
    UnitTest/ProgressBar.cpp
    UnitTest/Checkbox.cpp
    UnitTest/GroupBox.cpp
    UnitTest/ComboBox.cpp
    UnitTest/LabelMultiline.cpp
    UnitTest/UnitTest.cpp
    UnitTest/MenuStrip.cpp
    UnitTest/TabControl.cpp
    UnitTest/RadioButton.cpp
    UnitTest/TextBox.cpp
)

# renderer to use
set(RENDER_SOURCES
    "Renderers/${RENDERER_NAME}/${RENDERER_NAME}.cpp")
    
# gwork library
add_library(gwork STATIC ${GWORK_SOURCES} ${RENDER_SOURCES})
target_compile_features(gwork PUBLIC cxx_override)

install(TARGETS gwork
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/gwen
        DESTINATION include
)
    
if(BUILD_TEST)
    add_library(gwork_test STATIC ${GWORK_TEST_SOURCES})
    target_compile_features(gwork_test PUBLIC cxx_override)
    target_link_libraries(gwork_test gwork)
    
    install(TARGETS gwork_test
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
endif(BUILD_TEST)

if(BUILD_SAMPLE)
    set(SAMPLE_SOURCES
        "Samples/${RENDERER_NAME}/${RENDERER_NAME}Sample.cpp")
    
    add_executable(gwork_sample ${SAMPLE_SOURCES})
    target_compile_features(gwork_sample PRIVATE cxx_override)
    target_link_libraries(gwork_sample gwork ${RENDERER_LIB})
    
    if(BUILD_TEST)
        target_link_libraries(gwork_sample gwork_test)
    endif(BUILD_TEST)
    
    install(TARGETS gwork_sample
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
endif(BUILD_SAMPLE)
      

