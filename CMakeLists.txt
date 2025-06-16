cmake_minimum_required(VERSION 4.0)

set(LIBRARY_NAME "FuzzyMath")

project(FuzzyMath 
            LANGUAGES CXX
            VERSION 0.1.0 
            DESCRIPTION "C++ Library for representation and manipulation of fuzzy numbers.")

option(BUILD_TESTS "Build tests." ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Boost interval arithmetic is required
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(src)

# Create and install package configuration files
include(CMakePackageConfigHelpers)

# Configure version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Configure config file template
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIBRARY_NAME}
)

# Install configuration files
install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIBRARY_NAME}
)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()