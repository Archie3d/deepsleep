include(parse_src)

add_executable(${PROJECT_NAME} ${SRC} ${HDR})
target_link_libraries(${PROJECT_NAME} ${DEPENDS})

include(link_libs)
