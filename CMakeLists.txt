if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

cmake_minimum_required(VERSION 3.20)
project(Softplayer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_shadercross REQUIRED CONFIG)

find_package(Angelscript REQUIRED CONFIG)

# -- Angelscript Addons
get_target_property(ANGELSCRIPT_INCLUDE_DIR Angelscript::angelscript INTERFACE_INCLUDE_DIRECTORIES)
set(ANGELSCRIPT_ADDON_DIR "${ANGELSCRIPT_INCLUDE_DIR}/angelscript")

add_library(angelscript_addons STATIC
    ${ANGELSCRIPT_ADDON_DIR}/scriptbuilder/scriptbuilder.cpp
    ${ANGELSCRIPT_ADDON_DIR}/scriptstdstring/scriptstdstring.cpp
    ${ANGELSCRIPT_ADDON_DIR}/scriptstdstring/scriptstdstring_utils.cpp
)
target_include_directories(angelscript_addons PUBLIC ${ANGELSCRIPT_INCLUDE_DIR})
target_link_libraries(angelscript_addons PUBLIC Angelscript::angelscript)
add_library(Angelscript::addons ALIAS angelscript_addons)
# -- End Angelscript Addons

find_package(imgui CONFIG REQUIRED)

find_package(fastgltf CONFIG REQUIRED)

# Softplayer sources
set(SOFTPLAYER_SOURCES
    src/main.cpp
    src/gui_manager.cpp
    src/script_manager.cpp
    src/shader.cpp

    src/script/addons.cpp
    src/script/globals.cpp
)

add_executable(Softplayer ${SOFTPLAYER_SOURCES})

target_link_libraries(Softplayer PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_shadercross::SDL3_shadercross
    imgui::imgui
    Angelscript::angelscript
    Angelscript::addons
    fastgltf::fastgltf)
