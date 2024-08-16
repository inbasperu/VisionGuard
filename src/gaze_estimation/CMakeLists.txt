# Collect source and header files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

# Create named folders for the sources within the project
source_group("src" FILES ${SOURCES})
source_group("include" FILES ${HEADERS})

# Create library target
add_library(gaze_estimation STATIC ${SOURCES} ${HEADERS})

# Set properties for Windows
if(WIN32)
    set_target_properties(gaze_estimation PROPERTIES COMPILE_PDB_NAME gaze_estimation)
endif()

# Include directories
target_include_directories(gaze_estimation 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        ${OpenVINO_INCLUDE_DIRS}
)

# Find required packages
find_package(OpenCV REQUIRED)
find_package(OpenVINO REQUIRED COMPONENTS Runtime)
find_package(Qt6 REQUIRED COMPONENTS Core)

# Link libraries
target_link_libraries(gaze_estimation 
    PUBLIC
        ${OpenCV_LIBS}
        openvino::runtime
        monitors
        utils
        gflags
        Qt6::Core
)

# Link pthread on Unix
if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(gaze_estimation PRIVATE Threads::Threads)
endif()

# Set C++ standard
set_target_properties(gaze_estimation PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Add compile definitions if needed
target_compile_definitions(gaze_estimation PRIVATE
    # Add any necessary compile definitions here
)