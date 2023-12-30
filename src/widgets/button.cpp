#include "widgets.hpp"

Button::Button(double x, double y, std::string str){
    // update the transform
    transform_.translate(x, y);
    bounds_.left = x;
    bounds_.top = y;
    text_str_ = str;
    // load font
    // TODO: make this a reference?
    if (!font_.loadFromFile("c:\\Windows\\Fonts\\arial.ttf")){
        return;
    }
    text_width_ = text_str_.size() * char_width_;
}
void Button::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const {
    // make text
    sf::Text text;
    text.setFont(font_);
    text.setCharacterSize(char_height_);
    text.setString(text_str_);
    if (highlighted_){
        text.setFillColor(sf::Color::Black);
    } else {
        text.setFillColor(sf::Color::White);
    }
    auto textRect = text.getLocalBounds();

    // make rect
    sf::RectangleShape shape(sf::Vector2f(text_width_, char_height_ + 2 * border_height_));
    shape.setOutlineThickness(10);
    shape.setOutlineColor(sf::Color::Blue);
    if (highlighted_){
        shape.setFillColor(sf::Color::White);
    } else {
        shape.setFillColor(sf::Color::Black);
    }

    // draw rect first
    target.draw(shape, transform);
    // draw text on top
    target.draw(text, transform);
}
sf::Rect<double> Button::onCalculateBounds() {
    // w.r.t. parent
    bounds_.left = GetParentX();
    bounds_.top = GetParentY();
    bounds_.width = text_width_;
    bounds_.height = char_height_ + 2 * border_height_;
    return bounds_;
}