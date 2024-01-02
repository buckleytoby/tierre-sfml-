
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
        // TODO: change this to be id based
        clicked_idx_ = GetHoveredChild();
        // click me too. Can remove this if I choose to change the click behavior to click the parent object too
        clicked_ = true;
        onClick();
    };
    // x, y is w.r.t. parent
    auto button = std::make_shared<Button>(0, bounds_.height + 10, str);
    button->SetOnClickCallback(button_cb);
    AddChild(button);
    CalculateBounds();
}
int Dropdown::GetHoveredChild(){
    // iterate through children, return early if it's hovered
    for (int i=0; i<children_.size(); i++){
        if (children_[i]->hovered_){
            return i;
        }
    }
    return -1;
}
int Dropdown::GetClickedIdx(){
    // iterate through children, return early if it's clicked
    // for (int i=0; i<children_.size(); i++){
    //     if (children_[i]->clicked_){
    //         return i;
    //     }
    // }
    // return -1;
    return clicked_idx_;
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