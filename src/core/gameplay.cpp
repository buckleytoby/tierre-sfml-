#include "gameplay.hpp"
#include "map.hpp"





/////////////////////////////////////// GamePlay ///////////////////////////////////////
// write class methods
GamePlay::~GamePlay(){
    // destructor
}
void GamePlay::Draw(sf::RenderWindow& window){
    // draw map
    // TODO: move the map draw from hud_ to map_

    // draw hud
    hud_->Draw(window); // contains the map Draw

    // draw task manager
    task_manager_ptr_->Draw(window);
}
void GamePlay::update(double dt){
    //// meta updates
    // TODO: change mouse_x_ and mouse_y_ to be in a class which is passed to all of gameplay's children

    // update mouse position
    viewport_->UpdateMousePosition();
    map_->SetMousePosition(viewport_->map_mouse_x_, viewport_->map_mouse_y_);
    hud_->SetMousePosition(viewport_->mouse_x_, viewport_->mouse_y_);
    task_manager_ptr_->mouse_x_ = viewport_->mouse_x_;
    task_manager_ptr_->mouse_y_ = viewport_->mouse_y_; // hack

    // update the GUI
    hud_->Update(dt);

    //// update the gameplay
    // update the map
    map_->update(dt);

    // update the viewport
    viewport_->update(dt);

    // update the task manager
    task_manager_ptr_->Update();
}
void GamePlay::load(sf::RenderWindow& window){
    // load the gameplay
    // set the viewport window reference
    viewport_->SetWindowHandle(&window);

}
void GamePlay::unload(){
    // unload the gameplay
}
/////////////////////////////////////// End GamePlay ///////////////////////////////////////


