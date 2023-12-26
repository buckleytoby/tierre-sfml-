// include guard
# ifndef BUILDINGS_HPP
# define BUILDINGS_HPP

#include <vector>
#include "recipes.hpp"

enum class BuildingTypes {
    NONE,
    WORKSPACE, // simplest workspace
    FARM,
};

enum class BuildingStatus {
    PRECONSTRUCTION,
    CONSTRUCTION,
    READY,
    OPERATING,
};

class Building {
    public:
        BuildingTypes building_type_;
        BuildingStatus building_status_;
        std::map<RecipeTypes, Recipe> recipes_; // allowable recipes in this building
        int level_{0}; // level of building, 0 is base level
        std::map<ItemTypes, double> inventory_map_; // ItemType -> amount
        std::map<ItemTypes, double> item_reqs_map_; // ItemType -> required amount
        double effort_val_{0.0}; // effort "units"
        double construction_effort_req_{10.0}; // effort "units"
        RecipeTypes active_recipe_{RecipeTypes::NONE}; // recipe currently being worked on

        bool CheckItemReqs();
        void update(double dt);
        void AddToInventory(ItemTypes itemType, double amount);
        BuildingStatus GetStatus(){return building_status_;}
};

class Workspace : public Building
{
    public:
        Workspace(){
            building_type_ = BuildingTypes::WORKSPACE;
            building_status_ = BuildingStatus::PRECONSTRUCTION;
            level_ = 0;
            construction_effort_req_ = 3.0;
            recipes_[RecipeTypes::CORNSEED] = CornSeedRecipe(); // TODO: right now this is wasteful since recipes don't change no matter the building, so would be more efficient to have a global recipe map
            active_recipe_ = RecipeTypes::CORNSEED;
        }
};
class Farm : public Building
{
    public:
        Farm(){
            building_type_ = BuildingTypes::FARM;
            building_status_ = BuildingStatus::PRECONSTRUCTION;
            level_ = 0;
            construction_effort_req_ = 5.0;
            recipes_[RecipeTypes::CORN] = CornRecipe();
            active_recipe_ = RecipeTypes::CORN;
        }
};

class BuildingFactory
{
    public:
        static Building MakeBuilding(BuildingTypes building_type){
            switch (building_type)
            {
                case BuildingTypes::WORKSPACE:
                    return Workspace();
                    break;
                case BuildingTypes::FARM:
                    return Farm();
                    break;
                default:
                    return Building();
                    break;
            }
        }
};


#endif // BUILDINGS_HPP