cmake_minimum_required(VERSION 3.26)

message("CMake version: ${CMAKE_VERSION}")

#########################################################################################
# Set build (sub) directories and install directory
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "default install path" FORCE)
    set (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT FALSE)
endif()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(BUILD_SHARED_LIBS TRUE)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_SOURCES_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src)
# We add WIN32_LEAN_AND_MEAN to not have include errors between WinSock and WinSock2
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_compile_definitions(WIN32_LEAN_AND_MEAN)
endif()

#########################################################################################
# Add directories
add_subdirectory(src)
