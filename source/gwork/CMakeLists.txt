# Gwork library

include_directories(
    ${GWK_SOURCE_DIR}/source/platform/include
    ${GWK_SOURCE_DIR}/source/gwork/include
    ${GWK_RENDER_INCLUDES}
)

set(GWK_HEADERS
    include/Gwork/Align.h
    include/Gwork/Anim.h
    include/Gwork/ControlList.h
    include/Gwork/Controls.h
    include/Gwork/DragAndDrop.h
    include/Gwork/Events.h
    include/Gwork/Gwork.h
    include/Gwork/Hook.h
    include/Gwork/InputHandler.h
    include/Gwork/Skin.h
    include/Gwork/Tooltip.h
    include/Gwork/UiTypes.h
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
    # skins
    include/Gwork/Skins/Simple.h
    include/Gwork/Skins/TexturedBase.h
    include/Gwork/Skins/Texturing.h
)

set(GWK_SOURCES
    source/Anim.cpp
    source/BaseRender.cpp
    source/ControlList.cpp
    source/DebugBreak.h
    source/DragAndDrop.cpp
    source/Events.cpp
    source/Gwork.cpp
    source/Hook.cpp
    source/InputHandler.cpp
    source/Skin.cpp
    source/Tooltip.cpp
    source/Utility.cpp
    source/Controls/Base.cpp
    source/Controls/Button.cpp
    source/Controls/Canvas.cpp
    source/Controls/CheckBox.cpp
    source/Controls/CollapsibleCategory.cpp
    source/Controls/ColorControls.cpp
    source/Controls/ColorPicker.cpp
    source/Controls/ComboBox.cpp
    source/Controls/CrossSplitter.cpp
    source/Controls/DockBase.cpp
    source/Controls/DockedTabControl.cpp
    source/Controls/Dragger.cpp
    source/Controls/GroupBox.cpp
    source/Controls/HSVColorPicker.cpp
    source/Controls/HorizontalScrollBar.cpp
    source/Controls/HorizontalSlider.cpp
    source/Controls/ImagePanel.cpp
    source/Controls/Label.cpp
    source/Controls/LabelClickable.cpp
    source/Controls/ListBox.cpp
    source/Controls/Menu.cpp
    source/Controls/MenuItem.cpp
    source/Controls/MenuStrip.cpp
    source/Controls/NumericUpDown.cpp
    source/Controls/PageControl.cpp
    source/Controls/ProgressBar.cpp
    source/Controls/Properties.cpp
    source/Controls/PropertyTree.cpp
    source/Controls/RadioButton.cpp
    source/Controls/RadioButtonController.cpp
    source/Controls/Rectangle.cpp
    source/Controls/ResizableControl.cpp
    source/Controls/Resizer.cpp
    source/Controls/RichLabel.cpp
    source/Controls/ScrollBar.cpp
    source/Controls/ScrollBarBar.cpp
    source/Controls/ScrollBarButton.cpp
    source/Controls/ScrollControl.cpp
    source/Controls/Slider.cpp
    source/Controls/SplitterBar.cpp
    source/Controls/TabButton.cpp
    source/Controls/TabControl.cpp
    source/Controls/TabStrip.cpp
    source/Controls/Text.cpp
    source/Controls/TextBox.cpp
    source/Controls/TextBoxNumeric.cpp
    source/Controls/TreeControl.cpp
    source/Controls/TreeNode.cpp
    source/Controls/VerticalScrollBar.cpp
    source/Controls/VerticalSlider.cpp
    source/Controls/WindowCanvas.cpp
    source/Controls/WindowControl.cpp
    # dialog
    source/Controls/Dialog/FileOpen.cpp
    source/Controls/Dialog/FileSave.cpp
    source/Controls/Dialog/FolderOpen.cpp
    source/Controls/Dialog/Query.cpp
)

set(GWK_RENDER_SOURCES
    include/Gwork/Renderers/${GWK_RENDERER_NAME}.h
    renderers/${GWK_RENDERER_NAME}/${GWK_RENDERER_NAME}.cpp
    source/Platforms/${GWK_PLATFORM_NAME}.cpp
)

if(WANT_REFLECTION)
    set(GWK_REFLECT_SOURCES
        include/Gwork/Reflection.h
        source/Reflection.cpp
    )    
endif()

# header folder groups
source_group("${GWK_HEADER_FOLDER}"
             REGULAR_EXPRESSION "include/Gwork/[^/]+\\.h")
source_group("${GWK_HEADER_FOLDER}\\Controls"
             REGULAR_EXPRESSION "include/Gwork/Controls/[^/]+\\.h")
source_group("${GWK_HEADER_FOLDER}\\Controls\\Dialogs"
             REGULAR_EXPRESSION "include/Gwork/Controls/Dialogs/[^/]+\\.h")
source_group("${GWK_HEADER_FOLDER}\\Input"
             REGULAR_EXPRESSION "include/Gwork/Input/[^/]+\\.h")
source_group("${GWK_HEADER_FOLDER}\\Controls\\Layout"
             REGULAR_EXPRESSION "include/Gwork/Controls/Layout/[^/]+\\.h")
source_group("${GWK_HEADER_FOLDER}\\Controls\\Property"
             REGULAR_EXPRESSION "include/Gwork/Controls/Property/[^/]+\\.h")
source_group("${GWK_HEADER_FOLDER}\\Skins"
             REGULAR_EXPRESSION "include/Gwork/Skins/[^/]+\\.h")

# source folder groups
source_group("${GWK_SOURCE_FOLDER}"
             REGULAR_EXPRESSION "[^/]+\\.cpp")
source_group("${GWK_SOURCE_FOLDER}\\Controls"
             REGULAR_EXPRESSION "source/Controls/[^/]+\\.cpp")
source_group("${GWK_SOURCE_FOLDER}\\Controls\\Dialog"
             REGULAR_EXPRESSION "source/Controls/Dialog/[^/]+\\.cpp")

# Gwork library
add_library(Gwork STATIC ${GWK_HEADERS} ${GWK_SOURCES} ${GWK_REFLECT_SOURCES})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/Gwork
        DESTINATION include)

install(TARGETS Gwork
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)      
