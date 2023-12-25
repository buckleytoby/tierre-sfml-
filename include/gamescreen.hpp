// include guards
#ifndef GAMESCREEN_HPP
#define GAMESCREEN_HPP

#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include "BitWise.hpp"

class GameScreen
{
    public:
        virtual void draw(sf::RenderWindow& window){};
        virtual void update(double dt){};
        virtual BitFlag handleInput(sf::Event& event){return BitFlag();};
        virtual void load(sf::RenderWindow& window){};
        virtual void unload(){};
};

#endif // GAMESCREEN_HPP