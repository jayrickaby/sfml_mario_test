//
// Created by jay on 08/02/2026.
//

#ifndef SFML_MARIO_TEST_MANAGERUTILITIES_H
#define SFML_MARIO_TEST_MANAGERUTILITIES_H
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include <SFML/Graphics.hpp>


class ManagerUtilities{
public:
    ManagerUtilities() = default;
    ~ManagerUtilities() = default;

    static std::vector<std::string> findFiles(
        const std::string& directory,
        const std::vector<std::string>& extension
    );

    static std::string getStringFromJson(const nlohmann::basic_json<>& data);
    static sf::Vector2f getVector2fFromJson(const nlohmann::json& data);
    static sf::Vector2i getVector2iFromJson(const nlohmann::json& data);

    template <typename Container>
    static bool isInitialised(const Container& container){
        return !container.empty();
    }

private:

};


#endif //SFML_MARIO_TEST_MANAGERUTILITIES_H