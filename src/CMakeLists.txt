# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MazeGame VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(OPENAL_LIBRARY ${PROJECT_SOURCE_DIR}/../SFML/extlibs/libs-msvc/x64/openal32.lib)


# Add source files
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MazeGenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Pathfinder.cpp
)

# Add the executable
add_executable(MazeGame ${SOURCES})

# target based include and link directories
target_include_directories(MazeGame PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../SFML/include)
target_link_directories(MazeGame PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../SFML/lib)

# Link the executable to the libraries in the lib directory
target_link_libraries(MazeGame PUBLIC sfml-graphics sfml-system sfml-window)

# settings for VS debugging
set_target_properties(
    MazeGame PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${COMMON_OUTPUT_DIR}/bin"
)
# copy fonts folder to bin directory
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/fonts/" DESTINATION "${COMMON_OUTPUT_DIR}/bin/")