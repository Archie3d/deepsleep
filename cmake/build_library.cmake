include(parse_src)

set(${PROJECT_NAME}_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "${PROJECT_NAME} library includes" FORCE)

add_library(${PROJECT_NAME} ${SRC} ${CSRC} ${HDR} ${IHDR})
target_link_libraries(${PROJECT_NAME} ${DEPENDS})

include(link_libs)

include(parse_test)
