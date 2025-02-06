// include guard
# ifndef BUILDINGS_HPP
# define BUILDINGS_HPP

#include <vector>
#include "recipes.hpp"
#include "items.hpp"
#include "level.hpp"

enum class BuildingTypes {
    NONE,
    WORKSPACE, // simplest workspace
    STORAGESPACE, // simplest storage
    CHEST,
    FARM,
    WALL,
};
const std::string to_string(BuildingTypes p);
extern std::vector<BuildingTypes> BUILDINGTYPES;

enum class BuildingStatus {
    PRECONSTRUCTION,
    CONSTRUCTION,
    READY,
    OPERATING,
};
const std::string to_full_string(BuildingStatus p);

enum class BuildingCategory {
    NONE,
    CRAFT,
    FARM,
    STORAGE,
};


class Building: public GameObject
{
    private:
        static int id_counter_;
    public:
        int id_;
        Rect<double> footprint_{0, 0, 1, 1};
        XY<double> center_{0, 0};
        BuildingTypes building_type_;
        BuildingStatus building_status_;
        BuildingCategory building_category_{BuildingCategory::NONE};
        std::vector<RecipePtr> recipes_; // allowable recipes in this building
        Level level_; // level of building, 0 is base level
        Level inputs_level_;
        Level outputs_level_;
        Inventory inventory_; // ItemType -> amount
        std::map<ItemTypes, double> item_reqs_map_; // ItemType -> required amount
        double effort_val_{0.0}; // effort "units"
        double construction_effort_req_{1.0}; // effort "units"
        int active_recipe_idx_{-1}; // recipe currently being worked on
        
        // art
        sf::Texture texture_;
        sf::Sprite sprite_;


        Building(std::string name): GameObject(name) {id_ = id_counter_++;}
        int GetID(){return id_;}
        virtual void LoadArt();
        bool CheckItemReqs();
        bool CheckState(BuildingStatus building_status){return building_status_ == building_status;}
        bool IsCategory(BuildingCategory building_category){return building_category_ == building_category;}
        void update(double dt);
        void AddToInventory(ItemTypes itemType, double amount);
        BuildingStatus GetStatus(){return building_status_;}
        RecipePtr GetRecipe(int idx){return recipes_[idx];}
        RecipePtr GetActiveRecipe();
        bool HasActiveRecipe(){return active_recipe_idx_ >= 0;}
        void SetRecipes(std::vector<RecipePtr> recipes){recipes_ = recipes;}
        std::vector<std::string> GetRecipeNames();
        void SetActiveRecipe(int idx){active_recipe_idx_ = idx;}
        ItemMap GetItemMap(){return inventory_.GetItemMap();}
        virtual sf::Sprite& GetSprite(){return sprite_;}
        void Draw(sf::RenderTarget& target, double pixelX, double pixelY, double pixelsPerMeterW, double pixelsPerMeterH);
        virtual std::string to_string(){return std::string("Building");}

        // state cbs
        void Operating();

        bool Preconstruction(double dt);

        // checkers
        bool CanConstruct();
        bool CanCraft();

        std::vector<ItemPtr> GetNonRecipeItems();
        bool IsRecipeFulfilled();
};
typedef std::shared_ptr<Building> BuildingPtr;
const sf::Color to_color(BuildingPtr p);

class StoreBuilding: public Building
{
    public:
        StoreBuilding(std::string name): Building(name){
            building_category_ = BuildingCategory::STORAGE;
        }
        virtual std::string to_string(){return std::string("StoreBuilding");}
};
class CraftBuilding: public Building
{
    public:
        CraftBuilding(std::string name): Building(name){
            building_category_ = BuildingCategory::CRAFT;
        }
        virtual std::string to_string(){return std::string("CraftBuilding");}
};

class Workspace : public CraftBuilding
{
    public:
        sf::Texture texture_;
        sf::Sprite sprite_;

        Workspace(): CraftBuilding("Workspace"){
            SetRecipes({std::make_shared<ProcessCornstalk>(), std::make_shared<MakePlantRope>()});
            building_type_ = BuildingTypes::WORKSPACE;
            building_status_ = BuildingStatus::PRECONSTRUCTION;
            // level_ = 0;
            construction_effort_req_ = DEBUG? 1.0: 3.0;
            active_recipe_idx_ = 0;
            LoadArt();
        }
        virtual void LoadArt();
        virtual sf::Sprite& GetSprite(){return sprite_;}
        virtual std::string to_string(){return std::string("Workspace");}
};
class Storagespace : public StoreBuilding
{
    public:
        sf::Texture texture_;
        sf::Sprite sprite_;

        Storagespace(): StoreBuilding("StorageSpace"){
            building_type_ = BuildingTypes::STORAGESPACE;
            building_status_ = BuildingStatus::PRECONSTRUCTION;
            // level_ = 0;
            construction_effort_req_ = 3.0;
            inventory_.max_capacity_ = 100.0;
            LoadArt();
        }
        virtual void LoadArt();
        virtual sf::Sprite& GetSprite(){return sprite_;}
        virtual std::string to_string(){return std::string("StorageSpace");}
};
class Farm : public Building
{
    public:
        sf::Texture texture_;
        sf::Sprite sprite_;
        
        // Constructor
        Farm(): Building("Farm"){
            SetRecipes({std::make_shared<FarmCorn>()});
            building_type_ = BuildingTypes::FARM;
            building_status_ = BuildingStatus::PRECONSTRUCTION;
            // level_ = 0;
            construction_effort_req_ = DEBUG? 1.0: 30.0;
            active_recipe_idx_ = 0;
            LoadArt();
        }
        virtual void LoadArt();
        virtual sf::Sprite& GetSprite(){return sprite_;}
        virtual std::string to_string(){return std::string("Farm");}
};

class Wall: public Building
{
    public:
        sf::Texture texture_;
        sf::Sprite sprite_;
        
        // Constructor
        Wall(): Building("Wall"){
            building_type_ = BuildingTypes::FARM;
            building_status_ = BuildingStatus::PRECONSTRUCTION;
            // level_ = 0;
            construction_effort_req_ = DEBUG? 1.0: 30.0;
            LoadArt();

            // build requirements
            item_reqs_map_[ItemTypes::LUMBER] = 1.0;
        }
        virtual void LoadArt()
        {
            texture_.loadFromFile("../../../assets/wall.png");
            sprite_.setTexture(texture_);
        }
        virtual sf::Sprite& GetSprite(){return sprite_;}
        virtual std::string to_string(){return std::string("Wall");}
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
                case BuildingTypes::WALL:
                    return std::make_shared<Wall>();
                    break;
                default:
                    return std::make_shared<Building>("default");
                    break;
            }
        }
};


#endif // BUILDINGS_HPP