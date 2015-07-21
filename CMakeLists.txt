cmake_minimum_required(VERSION 2.8.10)

project(DeepSleep)

# Where to look for additional CMake scripts
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Output configuration
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/target/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/target/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

# Configure installation directories
set(INSTALL_BIN "bin")

# Enable unit-testing
enable_testing()

# Determine target platform
set(TARGET_PLATFORM "x86")
if(CMAKE_MODULE_LINKER_FLAGS MATCHES "^.*x64.*")
    set(TARGET_PLATFORM "x64")
endif()

# Process source code
add_subdirectory(externals)
add_subdirectory(source)