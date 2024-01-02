// include guard
# ifndef BUILDINGS_HPP
# define BUILDINGS_HPP

#include <vector>
#include "recipes.hpp"

enum class BuildingTypes {
    NONE,
    WORKSPACE, // simplest workspace
    STORAGESPACE, // simplest storage
    FARM,
};

enum class BuildingStatus {
    PRECONSTRUCTION,
    CONSTRUCTION,
    READY,
    OPERATING,
};

const std::string to_string(BuildingTypes p);
const std::string to_full_string(BuildingTypes p);
const std::string to_full_string(BuildingStatus p);

class Building {
    public:
        Rect<double> footprint_{0, 0, 1, 1};
        XY<double> center_{0, 0};
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
typedef std::shared_ptr<Building> BuildingPtr;

class Workspace : public Building
{
    public:
        const static inline std::map<RecipeTypes, Recipe> recipes_{
            {RecipeTypes::PROCESSCORNSTALK, ProcessCornstalk()}
        };

        Workspace(){
            building_type_ = BuildingTypes::WORKSPACE;
            building_status_ = BuildingStatus::PRECONSTRUCTION;
            level_ = 0;
            construction_effort_req_ = 3.0;
            active_recipe_ = RecipeTypes::PROCESSCORNSTALK;
        }
};
class Storagespace : public Building
{
    public:
        Storagespace(){
            building_type_ = BuildingTypes::STORAGESPACE;
            building_status_ = BuildingStatus::PRECONSTRUCTION;
            level_ = 0;
            construction_effort_req_ = 3.0;
        }
};
class Farm : public Building
{
    public:
        Farm(){
            building_type_ = BuildingTypes::FARM;
            building_status_ = BuildingStatus::PRECONSTRUCTION;
            level_ = 0;
            construction_effort_req_ = 30.0;
            recipes_[RecipeTypes::FARMCORN] = FarmCorn();
            active_recipe_ = RecipeTypes::FARMCORN;
        }
};

class BuildingFactory
{
    public:
        static std::shared_ptr<Building> MakeBuilding(BuildingTypes building_type){
            switch (building_type)
            {
                case BuildingTypes::WORKSPACE:
                    return std::make_shared<Workspace>();
                    break;
                case BuildingTypes::STORAGESPACE:
                    return std::make_shared<Storagespace>();
                    break;
                case BuildingTypes::FARM:
                    return std::make_shared<Farm>();
                    break;
                default:
                    return std::make_shared<Building>();
                    break;
            }
        }
};


#endif // BUILDINGS_HPP