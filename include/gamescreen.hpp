// include guards
#ifndef GAMESCREEN_HPP
#define GAMESCREEN_HPP

#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include "BitWise.hpp"


enum class GameScreenInputs {
    NONE,
    HANDLED,
    QUIT,
    CHANGEACTIVEGAMESCREENTITLESCREEN,
    CHANGEACTIVEGAMESCREENGAMEPLAY,
};

class GameScreen
{
    public:
        virtual void draw(sf::RenderWindow& window){};
        virtual void update(double dt){};
        virtual GameScreenInputs HandleInput(sf::Event& event){return GameScreenInputs::NONE;};
        virtual void load(sf::RenderWindow& window){};
        virtual void unload(){};
};

#endif // GAMESCREEN_HPP