//
// Created by jay on 08/02/2026.
//

#ifndef SFML_MARIO_TEST_MANAGERUTILITIES_H
#define SFML_MARIO_TEST_MANAGERUTILITIES_H
#include <string>
#include <vector>


class ManagerUtilities{
public:
    ManagerUtilities() = default;
    ~ManagerUtilities() = default;

    static std::vector<std::string> findFiles(
    const std::string& directory,
    const std::vector<std::string>& extension
    );

    template <typename Container>
    static bool isInitialised(const Container& container){
        return !container.empty();
    }

private:

};


#endif //SFML_MARIO_TEST_MANAGERUTILITIES_H