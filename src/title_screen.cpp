#include "title_screen.hpp"

// write class methods
TitleScreen::TitleScreen(){
    // constructor
}
TitleScreen::~TitleScreen(){
    // destructor
}
void TitleScreen::draw(){
    // draw the title screen
}
void TitleScreen::update(){
    // update the title screen
}
void TitleScreen::handleInput(sf::Event& event){
    // handle input for the title screen
    switch (event.type){
        case sf::Event::Closed:
            break;
        default:
            break;
    }
}
void TitleScreen::load(){
    // load the title screen
}
void TitleScreen::unload(){
    // unload the title screen
}