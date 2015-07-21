if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/")
    file(GLOB TEST_HDR ${CMAKE_CURRENT_SOURCE_DIR}/test/*.h)
    
    include_directories(${utest_INCLUDES})
    
    foreach(TEST ${TEST_HDR})
        get_filename_component(FILENAME ${TEST} NAME_WE)
        set(TEST_SRC "${CMAKE_CURRENT_BINARY_DIR}/test/test_${FILENAME}.cpp")
        
        file(WRITE ${TEST_SRC} "/* This file has been generated automatically */\n\n")
        file(APPEND ${TEST_SRC} "#include \"${TEST}\"\n")
        file(APPEND ${TEST_SRC} "UTEST_RUN_ALL\n")
        
        set(TEST_TARGET "${PROJECT_NAME}_${FILENAME}")
        add_executable(${TEST_TARGET} ${TEST_SRC} ${TEST})
        target_link_libraries(${TEST_TARGET} ${PROJECT_NAME} ${DEPENDS} utest)
        
        add_test(${TEST_TARGET} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_TARGET})
    endforeach()
    
endif()
