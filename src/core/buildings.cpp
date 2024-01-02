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

void Building::SetActiveRecipe(int idx){
    for (auto& recipe : recipes_){
        if (idx == static_cast<int>(recipe.first)){
            active_recipe_ = recipe.first;
            return;
        }
    }
}
std::vector<std::string> Building::GetRecipeNames(){
    std::vector<std::string> out;
    for (auto& recipe : recipes_){
        out.push_back(to_full_string(recipe.first));
    }
    return out;
}