#include "widgets.hpp"

Button::Button(double x, double y, std::string str): Widget(x, y)
{
    textbox_ = std::make_shared<TextBox>(0, 0, str, border_height_);
    textbox_->SetOnHoverCallback([this](){
        textbox_->text_.setFillColor(sf::Color::Black);
        textbox_->text_.setOutlineColor(sf::Color::White);
    });
    textbox_->SetOnLeaveHoverCallback([this](){
        textbox_->text_.setFillColor(sf::Color::White);
        textbox_->text_.setOutlineColor(sf::Color::Black);
    });

    AddChild(textbox_);
    CalculateBounds();
}
sf::Rect<double> Button::onCalculateBounds() {
    // w.r.t. parent
    bounds_.left = GetParentX();
    bounds_.top = GetParentY();
    bounds_.width = children_[0]->bounds_.width;
    bounds_.height = shape_.getLocalBounds().height;
    return bounds_;
}