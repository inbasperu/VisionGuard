cmake_minimum_required(VERSION 3.16)

# Set the project name
project(VisionGuard VERSION 1.0.0 LANGUAGES CXX)

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

# Include directories for OpenCV
set(OpenCV_DIR /Users/inbasekaranperumal/Developer/OpenSource/build_opencv)

# Include directories for OpenVINO
set(OpenVINO_DIR /Users/inbasekaranperumal/Developer/Programs/OpenVINO-install-dir/runtime/cmake)


find_package(OpenCV REQUIRED COMPONENTS core highgui videoio imgproc imgcodecs)

find_package(OpenVINO REQUIRED COMPONENTS Runtime)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia MultimediaWidgets Gui Charts)

qt_standard_project_setup()

# Add subdirectories
add_subdirectory(src/backend/thirdparty/gflags)
add_subdirectory(src/backend/common)
add_subdirectory(src/backend/multi_channel_common)
# add_subdirectory(src/backend/vision_guard_core)

qt_add_executable(VisionGuardApp
    src/app/main.cpp
    src/app/mainwindow.cpp
    src/app/mainwindow.ui
)

target_include_directories(VisionGuardApp PRIVATE ${OpenCV_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/src/backend/vision_guard_core/include")

target_link_libraries(VisionGuardApp PRIVATE Qt6::Core Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets Qt6::Gui Qt6::Charts ${OpenCV_LIBS} openvino::runtime gaze_estimation ${OpenCV_LIBRARIES} openvino::runtime monitors utils gflags)

set_target_properties(VisionGuardApp PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

# Define the path to the omz_models directory
set(OMZ_MODELS_DIR "/Users/inbasekaranperumal/Developer/OpenSource/GSoC/code/VisionGuard/omz_models")

# Copy the omz_models directory to the output directory after build
add_custom_command(TARGET VisionGuardApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${OMZ_MODELS_DIR}"
    "$<TARGET_FILE_DIR:VisionGuardApp>/../omz_models"
)
