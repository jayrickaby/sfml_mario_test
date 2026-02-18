set(SFML_BUILD_NETWORK FALSE)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/dependencies/imgui)
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)

add_subdirectory(json)
add_subdirectory(sfml)
add_subdirectory(spdlog)
add_subdirectory(imgui-sfml)
add_library(imgui INTERFACE)
add_library(stb INTERFACE)

target_include_directories(imgui INTERFACE imgui)
target_include_directories(stb INTERFACE stb)

target_link_libraries(sfml_mario_test PRIVATE
        ImGui-SFML::ImGui-SFML
        nlohmann_json::nlohmann_json
        SFML::Graphics
        SFML::Audio
        spdlog
        stb

)