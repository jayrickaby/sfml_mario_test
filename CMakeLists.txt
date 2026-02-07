cmake_minimum_required(VERSION 4.1)
project(sfml_mario_test)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(sfml_mario_test src/main.cpp
        src/Player.cpp
        src/Player.h
        src/TextureManager.cpp
        src/TextureManager.h
        src/Animation.cpp
        src/Animation.h
        src/constants.h
        src/AnimationManager.cpp
        src/AnimationManager.h
        src/TileManager.cpp
        src/TileManager.h
        src/Tile.cpp
        src/Tile.h
        src/LevelManager.cpp
        src/LevelManager.h
        src/GameManager.cpp
        src/GameManager.h
        src/InputManager.cpp
        src/InputManager.h
        src/CollisionSide.cpp
        src/CollisionSide.h
)

find_package(SFML 3.0 COMPONENTS Graphics Audio REQUIRED)

include(FetchContent)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
)
FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
)
FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(stb)

add_library(stb_interface INTERFACE)
target_include_directories(stb_interface INTERFACE ${stb_SOURCE_DIR})

install(TARGETS sfml_mario_test RUNTIME DESTINATION bin)

target_link_libraries(sfml_mario_test PRIVATE SFML::Graphics SFML::Audio nlohmann_json::nlohmann_json stb_interface)

set_target_properties(nlohmann_json PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations")

# Copy Assets Folder
#add_custom_command(TARGET sfml_mario_test POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/bin/assets
#)
