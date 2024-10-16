#include "gamescreen.hpp"
#include <iostream>

// write class methods
TitleScreen::TitleScreen(): GameScreen()
{
    // constructor
    SetHandleInputCb(std::bind(&TitleScreen::onHandleInput, this, std::placeholders::_1));
}
TitleScreen::~TitleScreen(){
    // destructor
}
void TitleScreen::Draw(sf::RenderWindow& window){
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
HandleInputNS::InputResult TitleScreen::onHandleInput(sf::Event& event){
    return HandleInputNS::InputResult::NOTHANDLED;
}
void TitleScreen::load(sf::RenderWindow& window){
    // load the title screen
}
void TitleScreen::unload(){
    // unload the title screen
}