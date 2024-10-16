
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
    // x, y is w.r.t. parent
    auto button = std::make_shared<Button>(0, bounds_.height + 10, str);
    button->SetOnClickCallback([this, button](){
        // set clicked idx
        // TODO: change this to be id based
        this->clicked_idx_ = GetHoveredChild();
        this->clicked_idxs_[clicked_idx_] = true;
        // click me too. Can remove this if I choose to change the click behavior to click the parent object too
        this->clicked_ = true; 
        this->onClick(); // call the parent's onClick
        return true;
    });
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
std::vector<int> Dropdown::GetClickedIdxs(){
    std::vector<int> clicked_idxs;
    for (int i=0; i<children_.size(); i++){
        if (clicked_idxs_[i]){
            clicked_idxs.push_back(i);
        }
    }
    return clicked_idxs;
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
void Dropdown::SetTextColor(sf::Color color){
    // invert the color of the clicked child
    if (clicked_idx_ != -1){
        auto child = children_[clicked_idx_];
        // cast to button
        auto button = std::dynamic_pointer_cast<Button>(child);
        button->SetTextColor(color);
    }
}
