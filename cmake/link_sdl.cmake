if(USE_SDL)
    include_directories(${SDL_INCLUDE})
    target_link_libraries(${PROJECT_NAME} ${SDL_LIB})
    if(USE_SDL_MAIN)
        #add_definitions(-D)
        target_link_libraries(${PROJECT_NAME} ${SDL_MAIN_LIB})
    endif()
    
    if(USE_SDL_IMAGE)
        include_directories(${SDL_IMAGE_INCLUDE})
        target_link_libraries(${PROJECT_NAME} ${SDL_IMAGE_LIB})
    endif()

endif()

