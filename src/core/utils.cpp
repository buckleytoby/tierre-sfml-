#include "utils.hpp"

bool wasLeftClicked(sf::Event event){
    if (event.type == sf::Event::MouseButtonReleased){
        if (event.mouseButton.button == sf::Mouse::Left){
            return true;
        }
    }
    return false;
}
bool wasLeftMouseDown(sf::Event event){
    if (event.type == sf::Event::MouseButtonPressed){
        if (event.mouseButton.button == sf::Mouse::Left){
            return true;
        }
    }
    return false;
}

double ConvertToPixelsX(double meters){
    return (meters - VIEWPORT_X) * PIXELS_PER_METER_X;
}
double ConvertToPixelsY(double meters){
    // takes care of the coordinate system flip for us
    return (VIEWPORT_H - (meters - VIEWPORT_Y)) * PIXELS_PER_METER_Y;
}