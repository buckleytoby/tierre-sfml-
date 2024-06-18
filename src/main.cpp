#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include "title_screen.hpp"

// initialize globals
int FRAMERATE = 60;
double DELTATIME = 1.0 / (double) FRAMERATE; // tie physics to framerate

/*
Create the main window and run the game loop.
*/
int main()
{
    enum class WindowState {
        TITLE_SCREEN,
        GAMEPLAY,
        PAUSE_MENU,
        GAME_OVER
    };

    auto window = sf::RenderWindow{ { 1920u, 1080u }, "CMake SFML Project"};
    window.setFramerateLimit(FRAMERATE);
    window.setVerticalSyncEnabled(true); // call it once, after creating the window

    // track current active window, starting with title screen
    auto windowState = WindowState::TITLE_SCREEN;

    // make the window classes
    auto titleScreen = TitleScreen{};

    // put window classes into map
    auto windowMap = std::map<WindowState, TitleScreen>{
        { WindowState::TITLE_SCREEN, titleScreen }
    };
    
    // set the active window
    auto activeWindow = windowMap[windowState];

    while (window.isOpen()){

        // loop through events
        for (auto event = sf::Event{}; window.pollEvent(event);){

            // main-level events
            switch (event.type){
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    // handle input in active window
                    activeWindow.handleInput(event);
                    break;
            }
        }

        window.clear();
        window.display();
    }
}