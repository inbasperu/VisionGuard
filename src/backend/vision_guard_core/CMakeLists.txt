# Collect source and header files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

# Create named folders for the sources within the project
source_group("src" FILES ${SOURCES})
source_group("include" FILES ${HEADERS})

# Create library target
add_library(vision_guard_core STATIC ${SOURCES} ${HEADERS})

# Set properties for Windows
if(WIN32)
    set_target_properties(vision_guard_core PROPERTIES COMPILE_PDB_NAME vision_guard_core)
endif()

# Include directories
target_include_directories(vision_guard_core PUBLIC ${HEADERS})

# Link libraries
target_link_libraries(vision_guard_core PRIVATE ${OpenCV_LIBRARIES} openvino::runtime monitors utils gflags)

# Link pthread on Unix
if(UNIX)
    target_link_libraries(vision_guard_core PRIVATE pthread)
endif()
