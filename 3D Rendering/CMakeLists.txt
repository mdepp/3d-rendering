cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
set(CMAKE_CXX_STANDARD 14)

set(SOURCES main.cpp
            DepthPixel.cpp
            GameEngine.cpp
            Light.cpp
            Model.cpp
            Player.cpp
            RenderingEngine.cpp
            Texture.cpp
            Triangle.cpp
            Vertex.cpp)

set(PROJECT_NAME 3d-rendering)
project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} ${SOURCES})

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
pkg_search_module(ASSIMP REQUIRED assimp)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${ASSIMP_LIBRARIES})

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS -Wall)