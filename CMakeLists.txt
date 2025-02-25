cmake_minimum_required(VERSION 3.10)
project(Sen8)



include_directories(${LUA_INCLUDE_DIR})

#Dependencies
include(FetchContent)

set(RAYLIB_VERSION "5.5")
FetchContent_Declare(
  raylib
  URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP true
  FIND_PACKAGE_ARGS ${RAYLIB_VERSION} EXACT
)
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(raylib)

#our project

set(SOURCE_FILES 
    graphics.c 
    console.c
    core.c
    customLog.c
    lua.c
    os.c
    ./editor/editor.c
    ./editor/scriptEditor.c
    ./editor/tileEditor.c
)
file(GLOB TYPE_FILES ./types/*.c ./types/*.h)
list(APPEND SOURCE_FILES ${TYPE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} raylib ${LUA_LIBRARIES})
