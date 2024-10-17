#include "dynamic_objects.hpp"

// set class variable
int DynamicObject::id_counter_ = 0;

const std::string to_string(NeedsTypes p){
  switch(p)
  {
    case NeedsTypes::FOOD: return "food";
    case NeedsTypes::SLEEP: return "sleep";
    case NeedsTypes::WATER: return "water";

  }
  return ""; // or an empty string
}


/////////////////////////////////////// DynamicObject ///////////////////////////////////////
DynamicObject::DynamicObject(std::string name): GameObject(name){
    immediate_surroundings_ = std::make_shared<ImmediateSurroundings>();
    nearby_surroundings_ = std::make_shared<NearbySurroundings>();
}
/// @brief Common update for all dynamic objects. Mostly simple physics.
/// @param dt 
void DynamicObject::onUpdate(double dt){
    // simple move
    Move(dt);
}

void DynamicObject::Move(double dt)
{
    // local vars
    double dx = 0;
    double dy = 0;

    // if over the bitwise actions
    if (dynamic_object_actions_.HasFlag((BitEnum)DynamicObjectActions::MOVE_UP)){
        dy += speed_;
    }
    if (dynamic_object_actions_.HasFlag((BitEnum)DynamicObjectActions::MOVE_DOWN)){
        dy -= speed_;
    }
    if (dynamic_object_actions_.HasFlag((BitEnum)DynamicObjectActions::MOVE_LEFT)){
        dx -= speed_;
    }
    if (dynamic_object_actions_.HasFlag((BitEnum)DynamicObjectActions::MOVE_RIGHT)){
        dx += speed_;
    }

    // diagonal movement check
    if ((dynamic_object_actions_.HasFlag((BitEnum)DynamicObjectActions::MOVE_UP) || dynamic_object_actions_.HasFlag((BitEnum)DynamicObjectActions::MOVE_DOWN)) && (dynamic_object_actions_.HasFlag((BitEnum)DynamicObjectActions::MOVE_LEFT) || dynamic_object_actions_.HasFlag((BitEnum)DynamicObjectActions::MOVE_RIGHT))){
        dx /= 1.41;
        dy /= 1.41;
    }

    // move away from overlapping dynamic objects
    if (nearby_surroundings_->dynamic_objects_.size() > 0){
        for (auto dynamic_object : nearby_surroundings_->dynamic_objects_){
            // check if it's me
            if (id_ == dynamic_object->id_){
                continue;
            }
            // check if overlapping
            if (dynamic_object->GetBoundingRect().intersects(GetBoundingRect())){
                // move away from dynamic object
                auto dist = eucl_dist<double>(GetCenter(), dynamic_object->GetCenter());
                auto dx_away = (GetCenter().x_ - dynamic_object->GetCenter().x_);
                auto dy_away = (GetCenter().y_ - dynamic_object->GetCenter().y_);
                dx += std_sign<double>(dx_away) * speed_ / 10;
                dy += std_sign<double>(dy_away) * speed_ / 10;
            }
        }
    }

    // update x & y
    MoveX(dx * dt);
    MoveY(dy * dt);
}
void DynamicObject::SetSpeed(double speed){
    speed_ = speed;
}
void DynamicObject::ClearAttention(){
	// clear the attention
	selected_building_ptr_ = nullptr;
	selected_resource_ptr_ = nullptr;
    selected_tile_ptr_ = nullptr;
}
bool DynamicObject::HasAttention(){
    bool yes = false;
    yes |= AttentionHasObject(); // I think this is all that's needed
    // yes |= AttentionHasBuilding();
    // yes |= AttentionHasResource();
    // yes |= AttentionHasTile();
    // yes |= AttentionHasRect();
    return yes;
}
void DynamicObject::SelectBuilding(std::shared_ptr<Building> building_ptr){
    SetAttention(building_ptr);
}
void DynamicObject::SelectResource(ResourcePtr resource_ptr){
    SetAttention(resource_ptr);
}
void DynamicObject::SetAttention(TaskPtr ptr){
    // sets the goal as well
    if (ptr == nullptr)
        return;
    // if task has a rect
    if (ptr->GetActiveAction()->identifier_rects_.size() > 0){
        auto rect = ptr->GetActiveAction()->identifier_rects_[0];
        SetAttention(rect);
    }
    // if task has a building
    if (ptr->GetActiveAction()->building_ptr_ != nullptr){
        SetAttention(ptr->GetActiveAction()->building_ptr_);
        SetGoalToSelectedBuilding();
    }
    // if task has a resource
    if (ptr->GetActiveAction()->resource_ptr_ != nullptr){
        SetAttention(ptr->GetActiveAction()->resource_ptr_);
        SetGoalToSelectedResource();
    }
}
void DynamicObject::SetAttention(BuildingPtr building_ptr){
    if (building_ptr != selected_building_ptr_){
        AttentionChanged();
        selected_building_ptr_ = building_ptr;
        selected_gameobject_ = building_ptr;
    }
}
void DynamicObject::SetAttention(ResourcePtr resource_ptr){
    if (resource_ptr != selected_resource_ptr_){
        AttentionChanged();
        selected_resource_ptr_ = resource_ptr;
        selected_gameobject_ = resource_ptr;
    }
}
void DynamicObject::SetAttention(TilePtr tile_ptr){
    if (tile_ptr != selected_tile_ptr_){
        AttentionChanged();
        selected_tile_ptr_ = tile_ptr;
        selected_gameobject_ = tile_ptr;
    }
}
void DynamicObject::SetGoalToSelectedGameObject(){
    if (selected_gameobject_ == nullptr){
        return;
    }
    goal_.x_ = selected_gameobject_->GetCenter().x_;
    goal_.y_ = selected_gameobject_->GetCenter().y_;
}
void DynamicObject::SetGoalToSelectedBuilding(){
    // set the goal to the selected building
    if (selected_building_ptr_ == nullptr){
        return;
    }
    goal_.x_ = selected_building_ptr_->center_.x_;
    goal_.y_ = selected_building_ptr_->center_.y_;
}
void DynamicObject::SetGoalToSelectedResource(){
    // set the goal to the selected resource
    if (selected_resource_ptr_ == nullptr){
        return;
    }
    goal_.x_ = selected_resource_ptr_->GetCenter().x_;
    goal_.y_ = selected_resource_ptr_->GetCenter().y_;
}
/////////////////////////////////////// End DynamicObject ///////////////////////////////////////
