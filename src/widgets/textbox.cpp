#include "widgets.hpp"

TextBox::TextBox(double x, double y, std::string str): Widget(x, y)
{
    text_str_ = str;
    // load font
    // TODO: make this a reference?
    if (!font_.loadFromFile("c:\\Windows\\Fonts\\arial.ttf")){
        return;
    }
    // make text
    text_.setFont(font_);
    text_.setCharacterSize(char_height_);
    text_.setString(text_str_);
    CalculateBounds();
}
TextBox::TextBox(double x, double y, std::string str, double border_thickness): TextBox(x, y, str)
{
    border_thickness_ = border_thickness;
    text_.setOutlineThickness(border_thickness_);
    CalculateBounds();
}

void TextBox::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const {
    target.draw(text_, transform);
}
sf::Rect<double> TextBox::onCalculateBounds() {
    // w.r.t. parent
    bounds_.left = GetParentX();
    bounds_.top = GetParentY();
    bounds_.width = text_.getGlobalBounds().width;
    bounds_.height = text_.getGlobalBounds().height;
    return bounds_;
}