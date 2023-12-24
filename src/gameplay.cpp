#include "gameplay.hpp"

// write class methods
GamePlay::GamePlay(){
    // constructor
}
GamePlay::~GamePlay(){
    // destructor
}
void GamePlay::draw(){
    // draw the gameplay
}
void GamePlay::update(){
    // update the gameplay
}
BitFlag GamePlay::handleInput(sf::Event& event){
    BitFlag handleInputFlags;
    // handle input for the title screen
    switch (event.type){
        // key pressed
        case sf::Event::KeyPressed:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::Escape:
                    std::cout << "Change active gamescreen TitleScreen" << std::endl;
                    handleInputFlags.SetFlag((EFlagValue)HandleInputActions::CHANGEACTIVEGAMESCREENTITLESCREEN);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return handleInputFlags;
}
void GamePlay::load(){
    // load the gameplay
}
void GamePlay::unload(){
    // unload the gameplay
}