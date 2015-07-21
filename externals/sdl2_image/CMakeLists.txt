set(SDL_IMAGE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "SDL_image directory")
set(SDL_IMAGE_INCLUDE ${SDL_IMAGE_DIR}/include CACHE INTERNAL "SDL_image includes")
set(SDL_IMAGE_LIB ${SDL_IMAGE_DIR}/lib/${TARGET_PLATFORM}/SDL2_image.lib CACHE INTERNAL "SDL_image library path")
set(SDL_IMAGE_DLLS
    ${SDL_IMAGE_DIR}/lib/${TARGET_PLATFORM}/SDL2_image.dll
    ${SDL_IMAGE_DIR}/lib/${TARGET_PLATFORM}/libjpeg-9.dll
    ${SDL_IMAGE_DIR}/lib/${TARGET_PLATFORM}/libpng16-16.dll
    ${SDL_IMAGE_DIR}/lib/${TARGET_PLATFORM}/libtiff-5.dll
    ${SDL_IMAGE_DIR}/lib/${TARGET_PLATFORM}/libwebp-4.dll
    ${SDL_IMAGE_DIR}/lib/${TARGET_PLATFORM}/zlib1.dll
    CACHE INTERNAL "SDL_image dlls")

# Copy SDL DLL to target directory
file(COPY ${SDL_IMAGE_DLLS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Install SDL_image dlls
install(FILES ${SDL_IMAGE_DLLS} DESTINATION ${INSTALL_BIN} COMPONENT runtime)
