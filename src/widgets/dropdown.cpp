
#include "widgets.hpp"

////////////////////////////// Dropdown //////////////////////////////
Dropdown::Dropdown(double x, double y, std::vector<std::string> strs): Widget(x, y)
{
    for (auto str: strs){
        AddItem(str);
    }
    CalculateBounds();
}
void Dropdown::AddItem(std::string str){
    // button callback
    auto button_cb = [this](){
        // set clicked idx
        clicked_ = true;

    };
    // x, y is w.r.t. parent
    auto button = std::make_shared<Button>(0, bounds_.height, str);
    button->SetOnClickCallback(button_cb);
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
int Dropdown::GetClickedIdx(){
    // iterate through children, return early if it's clicked
    for (int i=0; i<children_.size(); i++){
        if (children_[i]->clicked_){
            return i;
        }
    }
    return -1;
}
void Dropdown::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const {

}
WidgetInputs Dropdown::HandleInput(sf::Event& event){
    return WidgetInputs::NONE;
}
// bool Dropdown::onClick(){
//     std::cout <<"Dropdown::onClick"<<std::endl;
//     clicked_ = false;
//     clicked_idx_ = -1;

//     // call onClick for each child
//     for (int i=0; i<children_.size(); i++){
//         children_[i]->onClick();
//         if (clicked_){
//             clicked_idx_ = i;
//             return;
//         }
//     }
// }