cmake_minimum_required(VERSION 3.16)

# Set the project name
project(VisionGuard VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)


# C++ standard settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Ensure CMAKE_BUILD_TYPE is set for single-config generators
get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT GENERATOR_IS_MULTI_CONFIG AND NOT DEFINED CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE not defined, 'Release' will be used")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
endif()

# Set architecture-specific output directories
if(NOT BIN_FOLDER)
    string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} ARCH)
    if(ARCH STREQUAL "x86_64" OR ARCH STREQUAL "amd64")
        set(ARCH intel64)
    elseif(ARCH STREQUAL "i386")
        set(ARCH ia32)
    endif()
    set(BIN_FOLDER ${ARCH})
endif()

foreach(artifact IN ITEMS ARCHIVE COMPILE_PDB LIBRARY PDB RUNTIME)
    set("CMAKE_${artifact}_OUTPUT_DIRECTORY" "${CMAKE_CURRENT_BINARY_DIR}/${BIN_FOLDER}/$<CONFIG>")
endforeach()

# Windows-specific settings
if(WIN32)
    if(NOT "${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        message(FATAL_ERROR "Only 64-bit supported on Windows")
    endif()
    add_definitions(-DNOMINMAX)
endif()

# Compiler-specific settings
if(MSVC)
    add_compile_options(/wd4251 /wd4275 /wd4267 /W3 /EHsc)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "^GNU|(Apple)?Clang$")
    add_compile_options(-Wall)
endif()

# ARM-specific settings
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm64.*|aarch64.*|AARCH64.*)")
    set(AARCH64 ON)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm.*|ARM.*)")
    set(ARM ON)
endif()

if(ARM AND NOT CMAKE_CROSSCOMPILING)
    add_compile_options(-march=armv7-a+fp)
endif()

# Set the Qt installation path
set(CMAKE_PREFIX_PATH "/Users/inbasekaranperumal/Qt/6.7.1/macos")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

qt_add_executable(helloworld
    src/app/main.cpp
    src/app/mainwindow.cpp
    src/app/mainwindow.ui
)

target_link_libraries(helloworld PRIVATE Qt6::Widgets)

set_target_properties(helloworld PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

# Include directories for OpenCV and OpenVINO
set(OpenCV_DIR /Users/inbasekaranperumal/Developer/OpenSource/build_opencv)
find_package(OpenCV REQUIRED COMPONENTS core highgui videoio imgproc imgcodecs)
set(OpenVINO_DIR /Users/inbasekaranperumal/Developer/Programs/OpenVINO-install-dir/runtime/cmake)
find_package(OpenVINO REQUIRED COMPONENTS Runtime)

# Add subdirectories
add_subdirectory(src/gaze_estimation_backend/thirdparty/gflags)
add_subdirectory(src/gaze_estimation_backend/common/cpp)
add_subdirectory(src/gaze_estimation_backend/multi_channel_common/cpp)
add_subdirectory(src/gaze_estimation_backend/gaze_estimation/cpp)
