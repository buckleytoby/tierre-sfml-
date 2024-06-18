#include "resources.hpp"


const std::string to_string(ResourceTypes p){
  switch(p)
  {
    case ResourceTypes::LUMBER: return "L";
    case ResourceTypes::STONE: return "S";
    case ResourceTypes::WATER: return "W";
    case ResourceTypes::GRASS: return "G";
    case ResourceTypes::CORNSTALK: return "C";

  }
  return ""; // or an empty string
}
const std::string to_full_string(ResourceTypes p){
  switch(p)
  {
    case ResourceTypes::LUMBER: return "Lumber";
    case ResourceTypes::STONE: return "Stone";
    case ResourceTypes::WATER: return "Water";
    case ResourceTypes::GRASS: return "Grass";
    case ResourceTypes::CORNSTALK: return "Cornstalk";

  }
  return ""; // or an empty string
}

const sf::Color to_color(ResourceTypes p){
  switch(p)
  {
    case ResourceTypes::LUMBER: return sf::Color::Red;
    case ResourceTypes::STONE: return sf::Color::Yellow;
    case ResourceTypes::WATER: return sf::Color::Blue;
    case ResourceTypes::GRASS: return sf::Color::Green;

  }
  return sf::Color::White;
}

//////////////////////////////////// Resource ///////////////////////////////////////
Resource::Resource(std::string name): GameObject(name)
{
}
void Resource::Draw(sf::RenderTarget& target){
    // apply mask depending on state
    switch (resource_status_){
        case ResourceStatus::IDLE:
            // no change
            break;
        case ResourceStatus::GATHERING:
            // no change
            break;
        case ResourceStatus::DEPLETED:
            GetArt()->GetSprite().setColor(sf::Color(255, 255, 255, 128));
            break;
        default:
            break;
    }

    // call base class draw
    GameObject::Draw(target);

    // reset mask
    GetArt()->GetSprite().setColor(sf::Color(255, 255, 255, 255));
}
double Resource::Extract(){
    // get min of GetTotal & yield_per_gather_
    double amount = std::min(GetTotal(), yield_per_gather_);
    RemoveTotal(amount);
    return amount;
}
void Resource::RemoveTotal(double total){
    total_ -= total;
    if (total_ <= 0){
        resource_status_ = ResourceStatus::DEPLETED;
    }
}
bool Resource::isGatherable(){
    return resource_attributes_.HasFlag((BitEnum)ResourceAttributes::GATHERABLE) && !CheckState(ResourceStatus::DEPLETED);
}
bool Resource::isNavigable(){
    return resource_attributes_.HasFlag((BitEnum)ResourceAttributes::NAVIGABLE);
}
void Resource::IncrementGatherers(){
    nb_gatherers_++;
    if (nb_gatherers_ > 0){
        resource_status_ = ResourceStatus::GATHERING;
    }
}
void Resource::DecrementGatherers(){
    nb_gatherers_--;
    if (nb_gatherers_ < 0){
        std::cout << "Resource::DecrementGatherers: This should never happen" << std::endl;
    }
    if (nb_gatherers_ <= 0){
        resource_status_ = ResourceStatus::IDLE;
    }
}
bool Resource::CanGather(){
    return isGatherable() && HasResource();
}


//////////////////////////////////// End Resource ///////////////////////////////////////

Lumber::Lumber(double total) : Resource("lumber")
{
    resource_type_ = ResourceTypes::LUMBER;
    resource_attributes_.SetFlag((BitEnum)ResourceAttributes::GATHERABLE);
    time_to_gather_ = 1.0;
    yield_per_gather_ = 1.0;
    total_ = total;
}

Stone::Stone(double total) : Resource("stone")
{
    resource_type_ = ResourceTypes::STONE;
    resource_attributes_.SetFlag((BitEnum)ResourceAttributes::GATHERABLE);
    time_to_gather_ = 2.0;
    yield_per_gather_ = 1.0;
    total_ = total;
}

CornstalkResource::CornstalkResource(double total) : Resource("cornstalk")
{
    resource_type_ = ResourceTypes::CORNSTALK;
    resource_attributes_.SetFlag((BitEnum)ResourceAttributes::GATHERABLE);
    time_to_gather_ = 1.0;
    yield_per_gather_ = 1.0;
    total_ = total;
}
Grass::Grass(double total): Resource("grass")
{
    resource_type_ = ResourceTypes::GRASS;
    resource_attributes_.SetFlag((BitEnum)ResourceAttributes::GATHERABLE);
    time_to_gather_ = 1.0;
    yield_per_gather_ = 1.0;
    total_ = total;
}