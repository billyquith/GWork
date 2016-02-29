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

    add_executable(${SAMPLE_NAME} ${SAMPLE_SOURCES})
    target_compile_features(${SAMPLE_NAME} PRIVATE cxx_override)
    target_link_libraries(${SAMPLE_NAME}
                          Gwork Gwork${GWK_RENDER_NAME} GworkUtil
                          ${GWK_RENDER_LIBRARIES} ${PONDER_LIBRARIES})

    if(BUILD_TEST)
        target_link_libraries(${SAMPLE_NAME} GworkTest)
    endif(BUILD_TEST)

    add_custom_command(
        TARGET ${SAMPLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy 
            "${CMAKE_SOURCE_DIR}/source/gwork/resource/DefaultSkin.png"
            "$<TARGET_FILE_DIR:${SAMPLE_NAME}>"
        COMMENT "Copy default skin")

    add_custom_command(
        TARGET ${SAMPLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy 
            "${CMAKE_SOURCE_DIR}/source/gwork/resource/OpenSans.ttf"
            "$<TARGET_FILE_DIR:${SAMPLE_NAME}>"
        COMMENT "Copy default font")

    add_custom_command(
        TARGET ${SAMPLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy 
            "${CMAKE_SOURCE_DIR}/source/test/resource/gwen.png"
            "$<TARGET_FILE_DIR:${SAMPLE_NAME}>"
        COMMENT "Copy test textures")

    add_custom_command(
        TARGET ${SAMPLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy 
            "${CMAKE_SOURCE_DIR}/source/test/resource/test16.png"
            "$<TARGET_FILE_DIR:${SAMPLE_NAME}>"
        COMMENT "Copy test textures")

    # install(TARGETS ${SAMPLE_NAME}
    #         RUNTIME DESTINATION bin
    #         LIBRARY DESTINATION lib
    #         ARCHIVE DESTINATION lib)
            
endif(BUILD_SAMPLE)
