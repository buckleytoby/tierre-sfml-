#include "gameplay.hpp"
#include "gui.hpp"


////////////////////////////// GUI //////////////////////////////
GUI::GUI(){
    // ctor
    SetHandleInputCb(std::bind(&GUI::onHandleInput, this, std::placeholders::_1));
}
void GUI::Update(double dt){
    // update all widgets
    for (auto& widget: widgets_){
        widget->Update(dt, mouse_x_, mouse_y_);
    }
}
void GUI::Draw(sf::RenderWindow& window){
    /*
    Layout: selected object status
    [----------------------------------------------]
    [----]
    [----]
    [----]
    [----]
    [----]
    [----]
    [----]
    [----]
    [----]
    [----------------------------------------------]
    left panel: selected object status
    */

    // nullptr protection
    if (map_ref_ == nullptr || viewport_ref_ == nullptr){
        std::cout << "map_ref_ or viewport_ref_ is nullptr" << std::endl;
        return;
    }

    // draw the gameplay
    sf::Font font;
    if (!font.loadFromFile("c:\\Windows\\Fonts\\arial.ttf"))
    {
        return;
    }

    // iterate through the viewport, size + 1 to draw the tiles on the edge
    for (int i=viewport_ref_->GetLeft(); i<viewport_ref_->GetRight()+1; i++){
        for (int j=viewport_ref_->GetBottom(); j<viewport_ref_->GetTop()+1; j++){
            // get the tile from map_ref_
            auto tile = map_ref_->GetTile(i, j);
            // skip if no tile (including OOB)
            if (tile == nullptr){
                continue;
            }       

            // get the string
            std::string str = "";
            sf::Color color(0, 0, 0);

            // calculate pixel position for i, j
            double pixelX = viewport_ref_->ConvertMeterToPixelX(window, i);
            double pixelY = viewport_ref_->ConvertMeterToPixelY(window, j);
            double pixelsPerMeterW = viewport_ref_->GetPixelsPerMeterX(window);
            double pixelsPerMeterH = viewport_ref_->GetPixelsPerMeterY(window);  

            // draw the resource first
            if (tile->HasResource()){
                tile->GetResource()->Draw(window);
            }
            // check for building
            if (tile->building_type_ != BuildingTypes::NONE){
                tile->GetBuildingPtr()->Draw(window, pixelX, pixelY, pixelsPerMeterW, pixelsPerMeterH);
            }

        }
    }

    // iterate through dynamic game objects
    for (auto& dynamic_object_ptr : map_ref_->dynamic_object_ptrs_){
        // calculate pixel position for i, j
        dynamic_object_ptr->Draw(window);
    }

    // draw all widgets
    for (auto& widget: widgets_){
        widget->draw(window, sf::Transform::Identity);
    }

}

void GUI::RemoveWidget(int id){
    // find the widget
    for (int i=0; i<widgets_.size(); i++){
        if (widgets_[i]->id_ == id){
            // remove the widget
            widgets_.erase(widgets_.begin() + i);
            return;
        }
    }
}

// HUD::HUD(){
// }
// void HUD::Draw(sf::RenderWindow& window){
//     // draw everything else
//     inherited::Draw(window);

//     // draw the task manager on top (if visible)
//     task_manager_panel_ptr_->draw(window, sf::Transform::Identity);
// }
