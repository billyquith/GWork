# Gwork util library

include_directories(
    ${GWK_SOURCE_DIR}/source/platform/include
    ${GWK_SOURCE_DIR}/source/gwork/include
    ${GWK_RENDER_INCLUDES}
    ${CMAKE_SOURCE_DIR}/source/util/include
    ${GWK_REFLECT_INCLUDE}
)

set(GWK_UTIL_SOURCES
    include/Gwork/Util/ControlFactory.h
    # import/export
    include/Gwork/Util/ImportExport.h
    source/ImportExport/Base.cpp
    source/ImportExport/DesignerFormat.h        # private
    source/ImportExport/DesignerFormat.cpp
    source/ImportExport/GworkUtil.h             # private
    source/ImportExport/GworkUtil.cpp
)

# header folder groups
source_group("${GWK_HEADER_FOLDER}\\ControlReflect"
             FILES "include/Gwork/Util/ControlReflect.h")
source_group("${GWK_HEADER_FOLDER}\\ImportExport"
             FILES "include/Gwork/Util/ImportExport.h")

# source folder groups
source_group("${GWK_SOURCE_FOLDER}\\ControlReflect"
             REGULAR_EXPRESSION "source/ControlReflect/.*")
source_group("${GWK_SOURCE_FOLDER}\\ImportExport"
             REGULAR_EXPRESSION "source/ImportExport/.*")

# Gwork utils
add_library(GworkUtil STATIC ${GWK_UTIL_SOURCES})
target_compile_features(GworkUtil PUBLIC cxx_override)
target_link_libraries(GworkUtil Gwork)

install(DIRECTORY ${GWK_SOURCE_DIR}/util/include/Gwork
        DESTINATION include)

install(TARGETS GworkUtil
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)


