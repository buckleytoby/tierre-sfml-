#include "widgets.hpp"

TextBox::TextBox(double x, double y, std::string str): Widget(x, y)
{
    text_str_ = str;
    // load font
    // TODO: make this a reference?
    if (!font_.loadFromFile("c:\\Windows\\Fonts\\arial.ttf")){
        return;
    }
    text_width_ = text_str_.size() * char_width_;
    CalculateBounds();
}

void TextBox::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const {
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

    target.draw(text, transform);
}
sf::Rect<double> TextBox::onCalculateBounds() {
    // w.r.t. parent
    bounds_.left = GetParentX();
    bounds_.top = GetParentY();
    bounds_.width = text_width_;
    bounds_.height = char_height_;
    return bounds_;
}