#include "senses.hpp"

/////////////////////////////////////// Surroundings ///////////////////////////////////////
void Surroundings::SetResource(std::vector<ResourcePtr> resources){
    resources_ = resources;
}
void Surroundings::SetBuilding(std::vector<BuildingPtr> buildings){
    buildings_ = buildings;
}
void Surroundings::SetDynamicObjects(std::vector<std::shared_ptr<DynamicObject>> dynamic_objects){
    dynamic_objects_ = dynamic_objects;
}
Rect<double> Surroundings::GetLocalRect(){
    // rect local to the parent
    // calculate min max x, y
    Rect<double> rect{x_ - range_, y_ - range_, range_*2, range_*2};
    return rect;
}
ResourcePtr Surroundings::GetResource(double x, double y){
    return map_ref_->GetResource(x, y);
}
BuildingPtr Surroundings::GetBuilding(double x, double y){
    return map_ref_->GetBuilding(x, y);
}
    
/////////////////////////////////////// End Surroundings ///////////////////////////////////////

/////////////////////////////////////// ImmediateSurroundings ///////////////////////////////////////
/////////////////////////////////////// End ImmediateSurroundings ///////////////////////////////////////

/////////////////////////////////////// NearbySurroundings ///////////////////////////////////////
/////////////////////////////////////// End NearbySurroundings ///////////////////////////////////////