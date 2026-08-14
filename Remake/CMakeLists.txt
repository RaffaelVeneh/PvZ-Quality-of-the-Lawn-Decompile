cmake_minimum_required(VERSION 3.16)
project(PvZRemake CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Vendor Libraries
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/raylib)
add_subdirectory(vendor/json)

# Remake Source Files
file(GLOB_RECURSE REMAKE_SOURCES
    "src/*.cpp"
    "src/*.h"
)

add_executable(PvZRemake ${REMAKE_SOURCES})

target_link_libraries(PvZRemake PRIVATE raylib nlohmann_json::nlohmann_json)

# Copy Assets to output directory after build
add_custom_command(TARGET PvZRemake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:PvZRemake>/assets
)
