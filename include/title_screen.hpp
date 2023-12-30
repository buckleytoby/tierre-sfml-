#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include "BitWise.hpp"
#include "gamescreen.hpp"

class TitleScreen : GameScreen
{
    public:
        TitleScreen();
        ~TitleScreen();
        void draw(sf::RenderWindow &window);
        void update(double dt);
        GameScreenInputs HandleInput(sf::Event& event);
        void load(sf::RenderWindow& window);
        void unload();
};