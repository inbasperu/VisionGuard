# Collect source and header files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

# Create named folders for the sources within the project
source_group("src" FILES ${SOURCES})
source_group("include" FILES ${HEADERS})

# Create executable file from sources
add_executable(gaze_estimation ${SOURCES} ${HEADERS})

# Set properties for Windows
if(WIN32)
    set_target_properties(gaze_estimation PROPERTIES COMPILE_PDB_NAME gaze_estimation)
endif()

# Include directories
target_include_directories(gaze_estimation PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Link libraries
target_link_libraries(gaze_estimation PRIVATE ${OpenCV_LIBRARIES} openvino::runtime monitors utils gflags)

# Link pthread on Unix
if(UNIX)
    target_link_libraries(gaze_estimation PRIVATE pthread)
endif()
