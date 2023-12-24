#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include "BitWise.hpp"
#include "gamescreen.hpp"

class GamePlay : GameScreen
{
    public:
        GamePlay();
        ~GamePlay();
        void draw();
        void update();
        BitFlag handleInput(sf::Event& event);
        void load();
        void unload();
};