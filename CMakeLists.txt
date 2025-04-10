cmake_minimum_required(VERSION 4.0)

project(Line_Tracker C)

set(CMAKE_C_STANDARD 17)

file(GLOB_RECURSE SRC_FILES
    src/*.c
    src/*.h
)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "5.5"
)

FetchContent_MakeAvailable(raylib)

add_executable(
    line_tracker
    ${SRC_FILES}
)

target_link_libraries(line_tracker PRIVATE raylib)