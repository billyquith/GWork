cmake_minimum_required(VERSION 3.2)

project(gwen)
find_package(SDL2 REQUIRED)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/gwen/include/" ${SDL2_INCLUDE_DIR})

add_definitions(-DGWEN_PLATFORM_WINDOWS)

set(GWEN_SOURCES 
    gwen/Renderers/SDL2/SDL2.cpp

    gwen/src/ToolTip.cpp
    gwen/src/ControlList.cpp
    gwen/src/Anim.cpp
    gwen/src/events.cpp
    gwen/src/Utility.cpp
#    gwen/src/Platforms/Null.cpp
    gwen/src/Platforms/Windows.cpp
    gwen/src/Controls/TabButton.cpp
    gwen/src/Controls/ScrollBar.cpp
    gwen/src/Controls/Button.cpp
    gwen/src/Controls/Dragger.cpp
    gwen/src/Controls/TextBoxNumeric.cpp
    gwen/src/Controls/Resizer.cpp
    gwen/src/Controls/VerticalScrollBar.cpp
    gwen/src/Controls/ResizableControl.cpp
    gwen/src/Controls/ScrollControl.cpp
    gwen/src/Controls/ImagePanel.cpp
    gwen/src/Controls/DockBase.cpp
    gwen/src/Controls/Label.cpp
    gwen/src/Controls/VerticalSlider.cpp
    gwen/src/Controls/PageControl.cpp
    gwen/src/Controls/MenuItem.cpp
    gwen/src/Controls/TabStrip.cpp
    gwen/src/Controls/Text.cpp
    gwen/src/Controls/RadioButtonController.cpp
    gwen/src/Controls/CrossSplitter.cpp
    gwen/src/Controls/HSVColorPicker.cpp
    gwen/src/Controls/HorizontalScrollBar.cpp
    gwen/src/Controls/NumericUpDown.cpp
    gwen/src/Controls/ListBox.cpp
    gwen/src/Controls/SplitterBar.cpp
    gwen/src/Controls/ColorControls.cpp
    gwen/src/Controls/Rectangle.cpp
    gwen/src/Controls/CollapsibleCategory.cpp
    gwen/src/Controls/Properties.cpp
    gwen/src/Controls/Base.cpp
    gwen/src/Controls/LabelClickable.cpp
    gwen/src/Controls/ColorPicker.cpp
    gwen/src/Controls/TreeControl.cpp
    gwen/src/Controls/Dialog/Query.cpp
    gwen/src/Controls/Dialog/FolderOpen.cpp
    gwen/src/Controls/Dialog/FileSave.cpp
    gwen/src/Controls/Dialog/FileOpen.cpp
    gwen/src/Controls/Slider.cpp
    gwen/src/Controls/ProgressBar.cpp
    gwen/src/Controls/CheckBox.cpp
    gwen/src/Controls/TreeNode.cpp
    gwen/src/Controls/Canvas.cpp
    gwen/src/Controls/PropertyTree.cpp
    gwen/src/Controls/GroupBox.cpp
    gwen/src/Controls/ComboBox.cpp
    gwen/src/Controls/DockedTabControl.cpp
    gwen/src/Controls/WindowCanvas.cpp
    gwen/src/Controls/Menu.cpp
    gwen/src/Controls/HorizontalSlider.cpp
    gwen/src/Controls/ScrollBarButton.cpp
    gwen/src/Controls/MenuStrip.cpp
    gwen/src/Controls/ScrollBarBar.cpp
    gwen/src/Controls/TabControl.cpp
    gwen/src/Controls/WindowControl.cpp
    gwen/src/Controls/RichLabel.cpp
    gwen/src/Controls/RadioButton.cpp
    gwen/src/Controls/TextBox.cpp
    gwen/src/inputhandler.cpp
    gwen/src/DragAndDrop.cpp
    gwen/src/Hook.cpp
    gwen/src/Gwen.cpp
    gwen/src/BaseRender.cpp
    gwen/src/Skin.cpp
)

set(GWEN_TEST_SOURCES 
gwen/UnitTest/CollapsibleList.cpp
    gwen/UnitTest/Button.cpp
    gwen/UnitTest/ScrollControl.cpp
    gwen/UnitTest/ImagePanel.cpp
    gwen/UnitTest/Label.cpp
    gwen/UnitTest/PageControl.cpp
    gwen/UnitTest/CrossSplitter.cpp
    gwen/UnitTest/ReadMe.txt
    gwen/UnitTest/ListBox.cpp
    gwen/UnitTest/Numeric.cpp
    gwen/UnitTest/Properties.cpp
    gwen/UnitTest/Window.cpp
    gwen/UnitTest/StatusBar.cpp
    gwen/UnitTest/ColorPicker.cpp
    gwen/UnitTest/TreeControl.cpp
    gwen/UnitTest/Slider.cpp
    gwen/UnitTest/ProgressBar.cpp
    gwen/UnitTest/Checkbox.cpp
    gwen/UnitTest/GroupBox.cpp
    gwen/UnitTest/ComboBox.cpp
    gwen/UnitTest/LabelMultiline.cpp
    gwen/UnitTest/UnitTest.cpp
    gwen/UnitTest/MenuStrip.cpp
    gwen/UnitTest/TabControl.cpp
    gwen/UnitTest/RadioButton.cpp
    gwen/UnitTest/TextBox.cpp
)
    

add_library(gwen ${GWEN_SOURCES})
install(TARGETS gwen
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
    
add_library(gwen_unittest ${GWEN_TEST_SOURCES})
install(TARGETS gwen_unittest
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
  
    
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/gwen/include/gwen
    DESTINATION include
)