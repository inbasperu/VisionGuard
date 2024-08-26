cmake_minimum_required(VERSION 3.16)

project(VisionGuard VERSION 1.1.0 LANGUAGES CXX)

# C++ standard settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Platform-specific settings
if(WIN32)
    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(FATAL_ERROR "Only 64-bit is supported on Windows")
    endif()
    add_definitions(-DNOMINMAX)
    set(GUI_TYPE WIN32)
elseif(APPLE)
    set(GUI_TYPE MACOSX_BUNDLE)
endif()

# Compiler-specific settings
if(MSVC)
    add_compile_options(/wd4251 /wd4275 /wd4267 /W3 /EHsc)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "^GNU|(Apple)?Clang$")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find required packages
find_package(OpenCV REQUIRED COMPONENTS core highgui videoio imgproc imgcodecs)
find_package(OpenVINO REQUIRED COMPONENTS Runtime)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia MultimediaWidgets Gui Charts Network)

qt_standard_project_setup()

# Add subdirectories for third-party libraries
add_subdirectory(third_party/gflags)
add_subdirectory(third_party/utils)
add_subdirectory(third_party/monitors)

# Collect all source files
file(GLOB_RECURSE SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# Collect all header files
file(GLOB_RECURSE HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

# Set up resources
set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
qt_add_resources(RESOURCES ${RESOURCES_DIR}/resources.qrc)

if(WIN32)
    enable_language(RC)
    set(WIN32_RESOURCES ${RESOURCES_DIR}/windows_resource.rc)
endif()

# Define the executable
qt_add_executable(VisionGuardApp ${GUI_TYPE}
    ${SOURCES}
    ${HEADERS}
    src/mainwindow.ui
    ${RESOURCES}
    ${WIN32_RESOURCES}
)

# Include directories
target_include_directories(VisionGuardApp PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
    ${OpenCV_INCLUDE_DIRS}
    ${OpenVINO_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(VisionGuardApp PRIVATE 
    Qt6::Core Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets Qt6::Gui Qt6::Charts Qt6::Network
    ${OpenCV_LIBS} 
    openvino::runtime 
    monitors 
    utils 
    gflags
)

# Link pthread on Unix
if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(VisionGuardApp PRIVATE Threads::Threads)
endif()

# Platform-specific configurations
if(APPLE)
    set_target_properties(VisionGuardApp PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER com.summerofcode.withgoogle.OpenVINO-Toolkit.VisionGuardApp
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
        MACOSX_BUNDLE_ICON_FILE vision-guard-removebg.icns
    )
    
    # Copy resources to the correct location in the app bundle
    add_custom_command(TARGET VisionGuardApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_BUNDLE_CONTENT_DIR:VisionGuardApp>/Resources"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${OMZ_MODELS_DIR}"
        "$<TARGET_BUNDLE_CONTENT_DIR:VisionGuardApp>/Resources/omz_models"
        COMMAND ${CMAKE_COMMAND} -E copy
        "${RESOURCES_DIR}/vision-guard-removebg.icns"
        "$<TARGET_BUNDLE_CONTENT_DIR:VisionGuardApp>/Resources/"
        COMMAND ${CMAKE_COMMAND} -E copy
        "${RESOURCES_DIR}/vision-guard-removebg.png"
        "$<TARGET_BUNDLE_CONTENT_DIR:VisionGuardApp>/Resources/"
    )
elseif(WIN32)
    # Copy resources for Windows
    add_custom_command(TARGET VisionGuardApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:VisionGuardApp>"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${OMZ_MODELS_DIR}"
        "$<TARGET_FILE_DIR:VisionGuardApp>/omz_models"
        COMMAND ${CMAKE_COMMAND} -E copy
        "${RESOURCES_DIR}/vision-guard-removebg.png"
        "$<TARGET_FILE_DIR:VisionGuardApp>/"
    )
else()
    # Copy resources for Linux
    add_custom_command(TARGET VisionGuardApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:VisionGuardApp>/Resources"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${OMZ_MODELS_DIR}"
        "$<TARGET_FILE_DIR:VisionGuardApp>/Resources/omz_models"
        COMMAND ${CMAKE_COMMAND} -E copy
        "${RESOURCES_DIR}/vision-guard-removebg.png"
        "$<TARGET_FILE_DIR:VisionGuardApp>/Resources/"
    )
endif()

# Install rules
install(TARGETS VisionGuardApp
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# If using CPack for packaging
set(CPACK_PACKAGE_NAME "VisionGuard")
set(CPACK_PACKAGE_VENDOR "GSoC 2024 OpenVINO Toolkit")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "VisionGuard Application")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "VisionGuard")
include(CPack)