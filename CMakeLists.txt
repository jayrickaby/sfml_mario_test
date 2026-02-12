cmake_minimum_required(VERSION 4.1)
project(sfml_mario_test)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(sfml_mario_test src/main.cpp
        src/Player.cpp
        src/Player.h
        src/Managers/TextureManager.cpp
        src/Managers/TextureManager.h
        src/Types/Animation.cpp
        src/Types/Animation.h
        src/Managers/AnimationManager.cpp
        src/Managers/AnimationManager.h
        src/Managers/TileManager.cpp
        src/Managers/TileManager.h
        src/Types/Tile.cpp
        src/Types/Tile.h
        src/Managers/LevelManager.cpp
        src/Managers/LevelManager.h
        src/Managers/GameManager.cpp
        src/Managers/GameManager.h
        src/Managers/InputManager.cpp
        src/Managers/InputManager.h
        src/Types/CollisionSide.cpp
        src/Types/CollisionSide.h
        src/Managers/AnimationSubManager.cpp
        src/Managers/AnimationSubManager.h
        src/Managers/ManagerUtilities.cpp
        src/Managers/ManagerUtilities.h
        src/Globals.cpp
        src/Globals.h
        src/Managers/SoundManager.cpp
        src/Managers/SoundManager.h
)

find_package(SFML 3.0 COMPONENTS Graphics Audio REQUIRED)

include(FetchContent)

set(SFML_USE_DRM_RENDERING TRUE)
set(SFML_USE_SYSTEM_DEPS FALSE)

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
FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
)

FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(stb)
FetchContent_MakeAvailable(sfml)

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
