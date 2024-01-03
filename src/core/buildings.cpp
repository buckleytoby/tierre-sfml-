#include "buildings.hpp"

const std::string to_string(BuildingTypes p){
  switch(p)
  {
    case BuildingTypes::NONE: return "";
    case BuildingTypes::WORKSPACE: return "Ws";
    case BuildingTypes::FARM: return "Fa";
  }
  return ""; // or an empty string
}
const std::string to_full_string(BuildingTypes p){
  switch(p)
  {
    case BuildingTypes::NONE: return "";
    case BuildingTypes::WORKSPACE: return "Workspace";
    case BuildingTypes::FARM: return "Farm";
  }
  return ""; // or an empty string
}
const std::string to_full_string(BuildingStatus p){
    switch(p)
    {
        case BuildingStatus::PRECONSTRUCTION: return "Preconstruction";
        case BuildingStatus::CONSTRUCTION: return "Construction";
        case BuildingStatus::READY: return "Ready";
        case BuildingStatus::OPERATING: return "Operating";
    }
    return ""; // or an empty string
}
const sf::Color to_color(BuildingPtr p){
    switch(p->building_status_)
    {
        case BuildingStatus::PRECONSTRUCTION: return sf::Color::Red;
        case BuildingStatus::CONSTRUCTION: return sf::Color::Yellow;
        case BuildingStatus::READY: return sf::Color::Green;
        case BuildingStatus::OPERATING: return sf::Color::Blue;
    }
    return sf::Color::White;
} 











//////////////////////////////////// Building ///////////////////////////////////////
std::vector<std::string> Building::GetRecipeNames(){
    std::vector<std::string> out;
    for (auto& recipe : recipes_){
        out.push_back(to_full_string(recipe->GetType()));
    }
    return out;
}