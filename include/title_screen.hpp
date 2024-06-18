#include <SFML/Graphics.hpp>
#include "globals.hpp"

class TitleScreen 
{
    public:
        TitleScreen();
        ~TitleScreen();
        void draw();
        void update();
        void handleInput(sf::Event& event);
        void load();
        void unload();
};