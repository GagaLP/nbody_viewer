cmake_minimum_required(VERSION 3.25)

project(nBodyViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-fdiagnostics-color=always -O3)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG  4.5.0
            SYSTEM
    )
    FetchContent_MakeAvailable(raylib)
endif()

# raylib-cpp
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    FetchContent_Declare(
            raylib_cpp
            GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
            GIT_TAG v4.5.1
            SYSTEM
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()


find_package(raygui QUIET)
if (NOT raygui_FOUND)
    FetchContent_Declare(
            raygui
            GIT_REPOSITORY https://github.com/raysan5/raygui.git
            GIT_TAG 4.0
            SYSTEM
    )
    FetchContent_MakeAvailable(raygui)
endif()

# set raygui source directory as include directory
add_compile_options(-Wall -Wextra -pedantic)

set(SRC_HEADER Viewer/Utility.hpp Viewer/Viewer.hpp Viewer/Sphere.hpp Viewer/Light.hpp Viewer/OrbitalCamera.hpp)
set(SRC_SOURCE main.cpp Viewer/Viewer.cpp Viewer/Sphere.cpp Viewer/Light.cpp Viewer/OrbitalCamera.cpp)

add_executable(main ${SRC_HEADER} ${SRC_SOURCE})

target_link_libraries(main PUBLIC raylib raylib_cpp)

target_include_directories(main SYSTEM PUBLIC ${raygui_SOURCE_DIR}/src)
target_include_directories(main SYSTEM PUBLIC ${raygui_SOURCE_DIR}/styles)

# create shader folder
add_custom_command(TARGET main PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:main>/resources/)

# copy shaders to build directory only if shader files are newer than the ones in the build directory
if (EMSCRIPTEN)
# target_include_directories(main PUBLIC /Users/gabrielmitterrutzner/bin.nosync/emsdk/upstream/emscripten/system/include)
add_custom_command(TARGET main PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/Viewer/shader/wasm/*
        $<TARGET_FILE_DIR:main>/resources/)
else()
add_custom_command(TARGET main PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/Viewer/shader/gl/*
        $<TARGET_FILE_DIR:main>/resources/)
endif()

add_custom_command(TARGET main PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/*
        $<TARGET_FILE_DIR:main>/resources/)

add_custom_command(TARGET main PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/index.html
        $<TARGET_FILE_DIR:main>/)

if (EMSCRIPTEN)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPLATFORM_WEB")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s WASM=1 -s ASYNCIFY -s FORCE_FILESYSTEM=1 -s ALLOW_MEMORY_GROWTH=1 --preload-file resources")
        # set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
endif ()
