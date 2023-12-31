#include "widgets.hpp"

Button::Button(double x, double y, std::string str): Widget(x, y)
{
    auto textbox = std::make_shared<TextBox>(border_height_, border_height_, str);
    textbox->CalculateBounds();
    AddChild(textbox);
    CalculateBounds();
}
void Button::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const {
    auto textRect = children_[0]->bounds_;

    // make rect
    sf::RectangleShape shape(sf::Vector2f(textRect.width, textRect.height));
    shape.setOutlineThickness(10);
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
    bounds_.left = GetParentX();
    bounds_.top = GetParentY();
    bounds_.width = children_[0]->bounds_.width + 2 * border_height_;
    bounds_.height = shape_.getLocalBounds().height;
    return bounds_;
}