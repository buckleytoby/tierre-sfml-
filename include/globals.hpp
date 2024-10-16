// import protection
#ifndef GLOBALS_HPP_INCLUDED
#define GLOBALS_HPP_INCLUDED

#include <memory>
#include <mutex>
#include "SFML/Graphics.hpp"

// globals
extern bool DEBUG;
extern int FRAMERATE;
extern double DELTATIME; // tie physics to framerate
extern double PIXELS_PER_METER_X; // meters * PIXELS_PER_METER = pixels
extern double PIXELS_PER_METER_Y; // meters * PIXELS_PER_METER = pixels
extern double VIEWPORT_X;
extern double VIEWPORT_Y;
extern double VIEWPORT_W;
extern double VIEWPORT_H;

extern unsigned int SCREENWIDTH;
extern unsigned int SCREENHEIGHT;

// texture class global handle
struct Textures
{
        sf::Texture art;
        sf::Texture worker_m;
        sf::Texture worker_f;
        sf::Texture lumber;
        sf::Texture stone;
        sf::Texture water;
        sf::Texture grass;
        sf::Texture cornstalk;
};
extern Textures* TEXTURES;

enum class GamescreenStates {
    TITLE_SCREEN,
    GAMEPLAY,
    PAUSE_MENU,
    GAME_OVER
};




#endif // GLOBALS_HPP_INCLUDED