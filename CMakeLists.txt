cmake_minimum_required(VERSION 4.1)
project(sfml_mario_test)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_WARN_DEPRECATED OFF)

include(FetchContent)

set(SFML_USE_DRM_RENDERING TRUE)
set(SFML_USE_SYSTEM_DEPS FALSE)
set(FETCHCONTENT_QUIET ON)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
        SYSTEM
)
FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
        SYSTEM
)
FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        SYSTEM
)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.17.0
        SYSTEM
)

FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(stb)
FetchContent_MakeAvailable(sfml)
FetchContent_MakeAvailable(spdlog)

add_executable(sfml_mario_test
        src/main.cpp
        # src/Player.cpp
        # src/Player.h
        src/Globals.cpp
        src/Globals.h
        src/Types/AnimationSet.cpp
        src/Types/AnimationSet.h
        src/Types/Animation.h
        src/Types/Frame.h
        src/Managers/AnimationManager.cpp
        src/Managers/AnimationManager.h
        src/Managers/ManagerUtilities.cpp
        src/Managers/ManagerUtilities.h
)


add_library(stb_interface INTERFACE)
target_include_directories(stb_interface SYSTEM INTERFACE ${stb_SOURCE_DIR})

target_link_libraries(sfml_mario_test PRIVATE
        SFML::Graphics
        SFML::Audio
        nlohmann_json::nlohmann_json
        stb_interface
        spdlog
)

install(TARGETS sfml_mario_test RUNTIME DESTINATION bin)

# Copy Assets Folder
#add_custom_command(TARGET sfml_mario_test POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/bin/assets
#)
