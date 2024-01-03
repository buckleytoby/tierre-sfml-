#include "gameplay.hpp"
#include "gui.hpp"


////////////////////////////// GUI //////////////////////////////
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
    for (int i=viewport_ref_->GetIntX(); i<viewport_ref_->GetIntWidth()+1; i++){
        for (int j=viewport_ref_->GetIntY(); j<viewport_ref_->GetIntHeight()+1; j++){
            // get the tile from map_ref_
            auto tile = map_ref_->GetTile(i, j);
            // skip if no tile (including OOB)
            if (tile == nullptr){
                continue;
            }       

            // get the string
            std::string str = "";
            sf::Color color(0, 0, 0);

            // check for building
            if (tile->building_type_ != BuildingTypes::NONE){
                str += to_string(tile->building_type_);
                color = to_color(tile->building_ptr_);
            } else {
                // iterate through resources
                for (auto& resource : map_ref_->GetTile(i, j)->resource_map_){
                    str += to_string(resource.second->resource_type_);
                    color = to_color(resource.second->resource_type_);
                }
            }
            if (str == ""){
                continue;
            }

            // calculate pixel position for i, j
            double pixelX = viewport_ref_->ConvertMeterToPixelX(window, i);
            double pixelY = viewport_ref_->ConvertMeterToPixelY(window, j);
            double pixelsPerMeterW = viewport_ref_->GetPixelsPerMeterX(window);
            double pixelsPerMeterH = viewport_ref_->GetPixelsPerMeterY(window);  

            // draw the tile
            sf::Text text;
            text.setFont(font);
            text.setString(str);
            text.setPosition(pixelX, pixelY);
            text.setOrigin(0, pixelsPerMeterH); // set origin to bottom left
            text.setCharacterSize(pixelsPerMeterH); // pixels
            // text.setScale(pixelsPerMeterW, pixelsPerMeterH); // isn't working well
            text.setFillColor(color);
            text.setStyle(sf::Text::Bold);
            window.draw(text);
        }
    }

    // iterate through dynamic game objects
    for (auto& dynamic_object_ptr : map_ref_->dynamic_object_ptrs_){
        // calculate pixel position for i, j
        double pixelX = viewport_ref_->ConvertMeterToPixelX(window, dynamic_object_ptr->footprint_.x_);
        double pixelY = viewport_ref_->ConvertMeterToPixelY(window, dynamic_object_ptr->footprint_.y_);   
        double pixelsPerMeterH = viewport_ref_->GetPixelsPerMeterY(window);         

        // draw the tile
        sf::Text text;
        text.setFont(font);
        text.setString("W");
        text.setPosition(pixelX, pixelY);
        text.setCharacterSize(pixelsPerMeterH); // pixels
        text.setOrigin(0, pixelsPerMeterH); // set origin to bottom left
        // text.setScale(pixelsPerMeterW, pixelsPerMeterH); // isn't working well
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
        window.draw(text);
    }

    // draw all widgets
    for (auto& widget: widgets_){
        widget->draw(window, sf::Transform::Identity);
    }

}

HUD::HUD(){
}
// void HUD::Draw(sf::RenderWindow& window){
//     // draw everything else
//     inherited::Draw(window);

//     // draw the task manager on top (if visible)
//     task_manager_panel_ptr_->draw(window, sf::Transform::Identity);
// }
