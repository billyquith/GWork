# Gwork samples

include_directories(
    ${CMAKE_SOURCE_DIR}/source/platform/include
    ${CMAKE_SOURCE_DIR}/source/gwork/include
    ${GWK_RENDER_INCLUDES}
    ${CMAKE_SOURCE_DIR}/source/test/include
)

if(BUILD_SAMPLE)
    set(SAMPLE_SOURCES
        "${GWK_RENDER_NAME}/${GWK_RENDER_NAME}Sample.cpp")

    set(SAMPLE_NAME Gwork${GWK_RENDER_NAME}Sample)
    
    if(APPLE)
        set(APP_BUILD_TYPE MACOSX_BUNDLE)
        set(APP_RESOURCE_PATH "$<TARGET_FILE_DIR:${SAMPLE_NAME}>/../Resources")
    else()
        set(APP_BUILD_TYPE )
        set(APP_RESOURCE_PATH "$<TARGET_FILE_DIR:${SAMPLE_NAME}>")
    endif()    

    add_executable(${SAMPLE_NAME} ${APP_BUILD_TYPE} ${SAMPLE_SOURCES})
    
    target_link_libraries(${SAMPLE_NAME}
                          Gwork Gwork${GWK_RENDER_NAME}
                          ${GWK_RENDER_LIBRARIES})

    if(BUILD_TEST)
        target_link_libraries(${SAMPLE_NAME} GworkTest)
    endif(BUILD_TEST)
    
    if(APPLE)
        set(APP_VERSION "0.1")
        set(APP_COPYRIGHT "Copyright 2016 Bill Quith")
        # make MacOS bundle
        set_target_properties(${SAMPLE_NAME} PROPERTIES
            MACOSX_BUNDLE_COPYRIGHT "${APP_COPYRIGHT}"
            MACOSX_BUNDLE_INFO_STRING "${APP_VERSION}, ${APP_COPYRIGHT}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${APP_VERSION}"
            MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} sample v${APP_VERSION}"
            MACOSX_BUNDLE_GUI_IDENTIFIER "org.quith.${PROJECT_NAME}.${SAMPLE_NAME}"
            )
        # set_source_files_properties(${DEMO_GFX} PROPERTIES
        #     MACOSX_PACKAGE_LOCATION "${APP_RESOURCE_DIR}/data/gfx"
        #     )
    endif()
    
    
    macro(CopyAppResource RES_FROM WHAT)
        get_filename_component(FNANE "${RES_FROM}" NAME)
        add_custom_command(
            TARGET ${SAMPLE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${RES_FROM}" "${APP_RESOURCE_PATH}/${FNANE}"
            COMMENT "Copy ${WHAT}")
    endmacro(CopyAppResource)

    CopyAppResource("${CMAKE_SOURCE_DIR}/source/gwork/resource/DefaultSkin.png"
                    "default skin")

    CopyAppResource("${CMAKE_SOURCE_DIR}/source/gwork/resource/OpenSans.ttf"
                    "default font")

    CopyAppResource("${CMAKE_SOURCE_DIR}/source/test/resource/gwen.png"
                    "test textures")

    CopyAppResource("${CMAKE_SOURCE_DIR}/source/test/resource/test16.png"
                    "test textures")
            
endif(BUILD_SAMPLE)
