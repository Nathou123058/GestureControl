cmake_minimum_required(VERSION 3.21)
project(GestureControl)

# Add Geode
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/geode)

# Create the mod
create_geode_mod(GestureControl)

# Add source files
target_sources(GestureControl PRIVATE
    src/main.cpp
    src/GestureDetector.cpp
    src/WebcamHandler.cpp
)

# Include directories
target_include_directories(GestureControl PRIVATE
    include
)

# Link libraries
target_link_libraries(GestureControl PRIVATE
    geode-sdk
)

# Add OpenCV for Windows (lightweight)
find_package(OpenCV REQUIRED COMPONENTS core videoio imgproc)
target_link_libraries(GestureControl PRIVATE ${OpenCV_LIBS})
