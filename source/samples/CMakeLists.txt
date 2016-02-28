# Gwork samples

include_directories(
    ${CMAKE_SOURCE_DIR}/source/platform/include
    ${CMAKE_SOURCE_DIR}/source/gwork/include
    ${RENDERER_INC}
    ${CMAKE_SOURCE_DIR}/source/test/include
)

if(BUILD_SAMPLE)
    set(SAMPLE_SOURCES
        "${RENDERER_NAME}/${RENDERER_NAME}Sample.cpp")

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
