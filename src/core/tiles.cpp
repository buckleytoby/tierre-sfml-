#include "tiles.hpp"

/////////////////////////////////////// Tile ///////////////////////////////////////
Tile::Tile(int i, int j): GameObject()
{
    x_ = i;
    y_ = j;
    SetPos(i, j);
}
void Tile::AddResource(ResourcePtr resource_ptr){
    SetResource(resource_ptr);
}
bool Tile::CanConstruct(){
    if (building_ptr_ == nullptr){
        return false;
    } 
    return building_ptr_->CanConstruct();
}
bool Tile::CanGather(){
    if (resource_ptr_ == nullptr){
        return false;
    } 
    return resource_ptr_->CanGather();
}
bool Tile::CanCraft(){
    if (building_ptr_ == nullptr){
        return false;
    } 
    return building_ptr_->CanCraft();
}
/////////////////////////////////////// End Tile ///////////////////////////////////////
