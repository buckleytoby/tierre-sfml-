#include <any>
#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include "BitWise.hpp"
#include "gameplay.hpp"
#include "gamescreen.hpp"
#include "handleinput.hpp"

// declare extern variables
std::map<ResourceTypes, ItemTypes> RESOURCE_TO_ITEM_MAP;

// initialize globals
bool DEBUG = true;
int FRAMERATE = 30;
double TIME_MULTIPLIER = 1.0;
double DELTATIME = TIME_MULTIPLIER / (double) FRAMERATE; // tie physics to framerate
int PIXELS_PER_METER = 100;
unsigned int SCREENWIDTH = 1600;
unsigned int SCREENHEIGHT = 800;
double PIXELS_PER_METER_X = 0;
double PIXELS_PER_METER_Y = 0;
double VIEWPORT_X = 0;
double VIEWPORT_Y = 0;
double VIEWPORT_W = 0;
double VIEWPORT_H = 0;

// init the static handle input mutex
// std::mutex HandleInputNS::HandleInput::handleInputMutex;
bool HandleInputNS::HandleInput::global_lock = false;

// init the TEXTURES global
Textures* TEXTURES = NULL;

/*
Create the main gamescreen and run the game loop.
*/
int main()
{
    auto window = sf::RenderWindow{ {SCREENWIDTH, SCREENHEIGHT}, "Historia Terra"};
    window.setFramerateLimit(FRAMERATE);
    window.setVerticalSyncEnabled(true); // call it once, after creating the gamescreen

    // make the textures global class
    TEXTURES = new Textures;

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

    // construct inline function for handling program-level inputs
    auto handleinputFCN = [&window, &gamescreenState, &gamescreen_map, &activeGamescreen](sf::Event& event){
        switch (event.type){
            case sf::Event::Closed:
                window.close();
                return HandleInputNS::InputResult::HANDLED;
                break;
            case sf::Event::KeyReleased:
                switch (event.key.scancode){
                    case sf::Keyboard::Scan::Escape:
                        window.close();
                        return HandleInputNS::InputResult::HANDLED;
                        break;
                    case sf::Keyboard::Scan::Space:
                        gamescreenState = GamescreenStates::GAMEPLAY;
                        activeGamescreen = gamescreen_map[gamescreenState];
                        return HandleInputNS::InputResult::HANDLED;
                }
                break;
        }
        return HandleInputNS::InputResult::NOTHANDLED;
    };
    auto interactive_mgr = Interactive(handleinputFCN);
    interactive_mgr.SetHandleInputOverride(true);

    // MAIN GAME LOOP
    while (window.isOpen()){

        // loop through events
        for (auto event = sf::Event{}; window.pollEvent(event);){

            // program-level events
            if (interactive_mgr.HandleInput(event) >= HandleInputNS::InputResult::HANDLED)
                continue;
            
            // active gamescreen takes next precedence
            activeGamescreen->HandleInput(event);
        }

        // updates
        activeGamescreen->update(DELTATIME);

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw
        activeGamescreen->Draw(window);

        // display
        window.display();
    }

    // garbage collection
    delete TEXTURES;
    delete titleScreen;
    delete gamePlay;
}