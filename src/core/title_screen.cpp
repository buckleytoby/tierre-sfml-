#include "title_screen.hpp"
#include <iostream>

// write class methods
TitleScreen::TitleScreen(){
    // constructor
}
TitleScreen::~TitleScreen(){
    // destructor
}
void TitleScreen::draw(sf::RenderWindow& window){
    // draw the title screen
    sf::Font font;
    if (!font.loadFromFile("c:\\Windows\\Fonts\\arial.ttf"))
    {
        return;
    }

    // update the title screen
    sf::Text text;

    // select the font
    text.setFont(font); // font is a sf::Font

    // set the string to display
    text.setString("Press Space to Start");

    // set the character size
    text.setCharacterSize(24); // in pixels, not points!

    // set the color
    text.setFillColor(sf::Color::Red);

    // set the text style
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    // inside the main loop, between window.clear() and window.display()
    window.draw(text);
}
void TitleScreen::update(double dt){

}
GameScreenInputs TitleScreen::HandleInput(sf::Event& event){
    GameScreenInputs output = GameScreenInputs::NONE;
    // handle input for the title screen
    switch (event.type){
        // key pressed
        case sf::Event::KeyPressed:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::Escape:
                    std::cout << "Quit" << std::endl;
                    return GameScreenInputs::QUIT;
                    break;
                case sf::Keyboard::Scan::Space:
                    std::cout << "Change active gamescreen GamePlay" << std::endl;
                    return GameScreenInputs::CHANGEACTIVEGAMESCREENGAMEPLAY;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return output;
}
void TitleScreen::load(sf::RenderWindow& window){
    // load the title screen
}
void TitleScreen::unload(){
    // unload the title screen
}