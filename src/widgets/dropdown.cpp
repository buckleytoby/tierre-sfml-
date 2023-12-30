
#include "widgets.hpp"

////////////////////////////// Dropdown //////////////////////////////
Dropdown::Dropdown(double x, double y, std::vector<std::string> strs){
    // update the transform
    transform_.translate(x, y);
    bounds_.left = x;
    bounds_.top = y;
    for (auto str: strs){
        AddItem(str);
    }
}
void Dropdown::AddItem(std::string str){
    // x, y is w.r.t. parent
    auto button = std::make_shared<Button>(0, bounds_.height, str);
    AddChild(button);
    CalculateBounds();
}
int Dropdown::GetHighlightedChild(){
    // iterate through children, return early if it's highlighted
    for (int i=0; i<children_.size(); i++){
        if (children_[i]->highlighted_){
            return i;
        }
    }
    return -1;
}
void Dropdown::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const {
    std::cout <<"Dropdown::onDraw"<<std::endl;
}
WidgetInputs Dropdown::HandleInput(sf::Event& event){
    return WidgetInputs::NONE;
}