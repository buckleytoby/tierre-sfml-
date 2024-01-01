#include "utils.hpp"

bool wasLeftClicked(sf::Event event){
    if (event.type == sf::Event::MouseButtonReleased){
        if (event.mouseButton.button == sf::Mouse::Left){
            return true;
        }
    }
    return false;
}