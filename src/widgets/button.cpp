#include "widgets.hpp"

Button::Button(double x, double y, std::string str): Widget(x, y)
{
    auto textbox = std::make_shared<TextBox>(0, 0, str);
    textbox->CalculateBounds();
    AddChild(textbox);
    CalculateBounds();
}
void Button::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const {
    auto textRect = children_[0]->bounds_;

    // make rect
    sf::RectangleShape shape(sf::Vector2f(textRect.width, textRect.height));
    shape.setOutlineThickness(border_height_);
    shape.setOutlineColor(sf::Color::Blue);
    if (highlighted_){
        shape.setFillColor(sf::Color::White);
    } else {
        shape.setFillColor(sf::Color::Black);
    }

    // draw rect first
    target.draw(shape, transform);
}
sf::Rect<double> Button::onCalculateBounds() {
    // w.r.t. parent
    bounds_.left = GetParentX() - border_height_;
    bounds_.top = GetParentY() - border_height_;
    bounds_.width = children_[0]->bounds_.width + 2 * border_height_;
    bounds_.height = shape_.getLocalBounds().height + 2 * border_height_;
    return bounds_;
}