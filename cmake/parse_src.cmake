# Parse source code folder to fetch source files

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB CSRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB HDR ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB IHDR ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

foreach(DEP ${DEPENDS})
	if(${DEP}_INCLUDES)
		include_directories(${${DEP}_INCLUDES})
	else()
		message(FATAL_ERROR "Library ${DEP} has not been parsed yet, while parsing depending target ${PROJECT_NAME}")
	endif()
endforeach()

add_definitions(-DPROJECT=\"${PROJECT_NAME}\")
