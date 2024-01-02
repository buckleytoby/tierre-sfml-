#include <any>
#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include "BitWise.hpp"
#include "title_screen.hpp"
#include "gameplay.hpp"
#include "gamescreen.hpp"

// declare extern variables
std::map<ResourceTypes, ItemTypes> RESOURCE_TO_ITEM_MAP;

// initialize globals
int FRAMERATE = 30;
double DELTATIME = 1.0 / (double) FRAMERATE; // tie physics to framerate
int PIXELS_PER_METER = 100;
unsigned int SCREENWIDTH = 1900;
unsigned int SCREENHEIGHT = 900;

/*
Create the main gamescreen and run the game loop.
*/
int main()
{
    auto window = sf::RenderWindow{ {SCREENWIDTH, SCREENHEIGHT}, "Tierre"};
    window.setFramerateLimit(FRAMERATE);
    window.setVerticalSyncEnabled(true); // call it once, after creating the gamescreen

    // track current active gamescreen, starting with title screen
    auto gamescreenState = GamescreenStates::TITLE_SCREEN;

    // make gamescreen classes using pointers
    TitleScreen* titleScreen = new TitleScreen();
    GamePlay* gamePlay = new GamePlay();

    // load gamescreen classes
    titleScreen->load(window);
    gamePlay->load(window);

    // put gamescreen classes into map
    std::map<GamescreenStates, GameScreen*> gamescreen_map;
    gamescreen_map[GamescreenStates::TITLE_SCREEN] = (GameScreen*)titleScreen;
    gamescreen_map[GamescreenStates::GAMEPLAY] = (GameScreen*)gamePlay;
    
    // set the active gamescreen
    auto activeGamescreen = gamescreen_map[gamescreenState];

    while (window.isOpen()){

        // loop through events
        for (auto event = sf::Event{}; window.pollEvent(event);){

            // main-level events
            if (event.type == sf::Event::Closed){
                window.close();
                continue;
            }
            
            // active gamescreen takes next precedence
            auto flag = activeGamescreen->HandleInput(event);

            // main-level response to gamescreen HandleInput
            switch(flag){
                case GameScreenInputs::QUIT:
                    window.close();
                    break;
                case GameScreenInputs::CHANGEACTIVEGAMESCREENTITLESCREEN:
                    gamescreenState = GamescreenStates::TITLE_SCREEN;
                    activeGamescreen = gamescreen_map[gamescreenState];
                    break;
                case GameScreenInputs::CHANGEACTIVEGAMESCREENGAMEPLAY:
                    gamescreenState = GamescreenStates::GAMEPLAY;
                    activeGamescreen = gamescreen_map[gamescreenState];
                    break;
            }
        }

        // updates
        activeGamescreen->update(DELTATIME);

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw
        activeGamescreen->draw(window);

        // display
        window.display();
    }
}