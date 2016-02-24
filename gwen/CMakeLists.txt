
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

# gwork library
add_library(gwork STATIC ${GWORK_SOURCES})
target_compile_features(gwork PUBLIC cxx_override)

install(TARGETS gwork
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/Gwen
        DESTINATION include
)

# gwork renderer & platform
add_library(gwork_${RENDERER_NAME} STATIC ${GWORK_RENDER_SOURCES})
target_compile_features(gwork_${RENDERER_NAME} PUBLIC cxx_override)
target_link_libraries(gwork_${RENDERER_NAME} gwork)
   
if(BUILD_TEST)
    add_library(gwork_test STATIC ${GWORK_TEST_SOURCES})
    target_compile_features(gwork_test PUBLIC cxx_override)
    target_link_libraries(gwork_test gwork gwork_${RENDERER_NAME})
    
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
    target_link_libraries(gwork_sample gwork gwork_${RENDERER_NAME} ${RENDERER_LIB})
    
    if(BUILD_TEST)
        target_link_libraries(gwork_sample gwork_test)
    endif(BUILD_TEST)
    
    install(TARGETS gwork_sample
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
endif(BUILD_SAMPLE)
      

