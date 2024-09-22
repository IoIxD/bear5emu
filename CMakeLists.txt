cmake_minimum_required(VERSION 3.11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(bear5emu
    LANGUAGES C CXX
)

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
    set(BUILD_TESTING  OFF CACHE BOOL "" FORCE)
    add_subdirectory(vendor/raylib vendor/raygui)
endif()

add_link_options("-lm")

FILE(GLOB_RECURSE sources src/*.c src/*.cpp)

include_directories(vendor/libretro-common/include)

# bear5emu binary
add_executable(bear5emu
    ${sources}
    ../vendor/libretro-common/compat/compat_strl.c
    ../vendor/libretro-common/compat/compat_posix_string.c
    ../vendor/libretro-common/string/stdstring.c
    ../vendor/libretro-common/encodings/encoding_utf.c
    ../vendor/libretro-common/file/file_path.c
    ../vendor/libretro-common/dynamic/dylib.c
)
target_link_libraries(bear5emu PUBLIC
    raylib
    ${CMAKE_DL_LIBS}
)
target_include_directories(bear5emu PUBLIC
    ../vendor/raygui/src
    ../vendor/libretro-common/include
)
target_compile_definitions(bear5emu PUBLIC HAVE_DYNAMIC)