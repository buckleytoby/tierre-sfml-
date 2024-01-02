#include "widgets.hpp"

RectWidget::RectWidget(double x, double y, double w, double h) : Widget(x, y, w, h)
{
    // make rect
    shape_.setSize(sf::Vector2f(w - 2*border_height_, h - 2*border_height_));
    shape_.setOutlineThickness(border_height_);
    shape_.setOutlineColor(sf::Color::Blue);
    shape_.setFillColor(sf::Color::Black);
}
void RectWidget::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const {
    target.draw(shape_, transform);
}