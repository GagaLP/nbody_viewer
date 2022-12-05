cmake_minimum_required(VERSION 2.8.12.2)

project(hpc22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
add_compile_options(-Wall -Wextra -pedantic -O3)


set(SRC_HEADER Viewer/Utility.hpp Viewer/Vector3D.hpp Viewer/Particle.hpp Viewer/Game.hpp Viewer/Sphere.hpp Viewer/Light.hpp)
set(SRC_SOURCE main.cpp Viewer/Game.cpp Viewer/Sphere.cpp Viewer/Light.cpp)

include(FetchContent)

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 4.2.0
    )
    FetchContent_MakeAvailable(raylib)
endif()

# raylib-cpp
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    FetchContent_Declare(
            raylib_cpp
            GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
            GIT_TAG v4.2.6
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

add_executable(main ${SRC_HEADER} ${SRC_SOURCE})

target_link_libraries(main PUBLIC raylib raylib_cpp)

# create shader folder
add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:main>/shader)

# copy shaders to build directory only if shader files are newer than the ones in the build directory
add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/Viewer/shader/*
        $<TARGET_FILE_DIR:main>/shader/)

