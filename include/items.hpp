// include guard
#ifndef ITEMS_HPP
#define ITEMS_HPP

#include <map>

#include "globals.hpp"
#include "BitWise.hpp"
#include "resources.hpp"
#include "level.hpp"

// typedefs
typedef double yield_kg;
typedef double amount_units;

// chain ResourceTypes and ItemTypes using this method: https://stackoverflow.com/a/18344942
enum class ItemCategories {
    NONE,
    RESOURCE,
    FOOD,
    TOOL,
};
// note: if an item is category NONE, then we don't have to make an explicit class for it, it'll just use the default Item class.
enum class ItemTypes {
    NONE,
    CORN,
    CORNSEED,
    PLANTFIBER,
    PLANTROPE,
    WOODRAFT,
    PLANTBASKET,

    // copy paste from ResourceTypes
    LUMBER = 128,
    STONE,
    WATER,
    GRASS,
    CORNSTALK,
};
const std::vector<ItemTypes> ITEM_TYPES = { // hacky, but it works
    ItemTypes::NONE,
    ItemTypes::CORN,
    ItemTypes::CORNSEED,
    ItemTypes::PLANTFIBER,
    ItemTypes::PLANTROPE,
    ItemTypes::WOODRAFT,
    ItemTypes::PLANTBASKET,
    ItemTypes::LUMBER,
    ItemTypes::STONE,
    ItemTypes::WATER,
    ItemTypes::GRASS,
    ItemTypes::CORNSTALK,
};
const std::string to_string(ItemTypes p);
const std::string to_full_string(ItemTypes p);

// define mapping from ResourceTypes to ItemTypes
extern std::map<ResourceTypes, ItemTypes> RESOURCE_TO_ITEM_MAP;

class Item
{
    public:
        ItemTypes item_type_;
        ItemCategories item_category_{ItemCategories::NONE};
        amount_units amount_{0.0}; // units of item
        double density_{1.0}; // kg per units of item
        Level level_;

        Item(){ItemTypes::NONE;}
        Item(ItemTypes item_type, double amount){item_type_ = item_type; amount_ = amount;}
        bool operator==(const Item& other);


        void SetAmount(amount_units amount){
            amount_units new_amount = std::max(0.0, amount);
            amount_ = new_amount;
        }
        double GetAmount(){return amount_;}
        void AddAmount(amount_units amount){amount_ += amount;}
        void AddYield(yield_kg yield){
            // calculate the amount
            auto amount = yield / density_;
            // add the amount
            AddAmount(amount);
        }
        void RemoveAmount(amount_units amount){
            SetAmount(amount_ - amount);
            }
        bool IsItemCategory(ItemCategories item_category){return item_category == item_category_;}
        double CalcMass(){return (double)amount_ * density_;}
};
using ItemPtr = std::shared_ptr<Item>;
using ItemMap = std::map<ItemTypes, ItemPtr>;

class ResourceItem: public Item
{
    public:
        ResourceTypes resource_type_;

        ResourceItem(){ResourceTypes::NONE;}
        ResourceItem(ResourceTypes resource_type, double amount){resource_type_ = resource_type; amount_ = amount;}
};

class FoodItem: public Item
{
    public:
        double nutrient_amount_{0.0}; // per unit of food
        FoodItem(){
            item_category_ = ItemCategories::FOOD;
        };
};

class ToolItem: public Item
{
    public:
        double additive_mod_{0.0}; // additive modifier
        double mult_mod_{1.0}; // multiplicative modifier

        ToolItem(){
            item_category_ = ItemCategories::TOOL;
        }
};

class CornItem: public FoodItem
{
    public:
        CornItem(double amount): FoodItem()
        {
            item_type_ = ItemTypes::CORN;
            amount_ = amount;
            nutrient_amount_ = 10.0;
            density_ = 0.3; // 300 g per cob
        }
};
class PlantBasket: public ToolItem
{
    public:
        PlantBasket(double amount): ToolItem()
        {
            item_type_ = ItemTypes::PLANTBASKET;
            amount_ = amount;
            density_ = 0.5; // 500 g per basket
            additive_mod_ = 20.0; // 20 kg
        }
};

// TODO: make item factory
class ItemFactory
{
    public:
        static std::map<ItemTypes, ItemPtr> item_blueprints_;

        static std::shared_ptr<Item> MakeItem(ItemTypes item_type, amount_units amount){
            switch (item_type)
            {
                case ItemTypes::CORN:
                    return std::make_shared<CornItem>(amount);
                    break;
                case ItemTypes::PLANTBASKET:
                    return std::make_shared<PlantBasket>(amount);
                    break;
                default:
                    return std::make_shared<Item>(item_type, amount);
                    break;
            }
        }
        static ItemPtr MakeItemYield(ItemTypes item_type, yield_kg yield){
            // get the density
            // check if item_type in item_blueprints_
            double density = 1.0;
            if (!(item_blueprints_.find(item_type) == item_blueprints_.end())){
                density = ItemFactory::item_blueprints_[item_type]->density_;
            }
            // calculate the amount
            auto amount = yield / density;
            // make the item
            return MakeItem(item_type, amount);
        }
};

class Inventory
{
    public:
        ItemMap inventory_map_; // itemType -> ItemPtr
        double max_capacity_{25.0}; // kgs
        
        ItemMap GetItemMap(){return inventory_map_;}
        ItemPtr GetItem(ItemTypes type){return inventory_map_[type];}
        bool find(ItemTypes itemType){return inventory_map_.find(itemType) != inventory_map_.end();}
        // void AddToInventory(ItemTypes itemType, double yield){
        //     if (find(itemType)){
        //         GetItem(itemType)->AddYield((yield_kg)yield);
        //     } else {
        //         inventory_map_[itemType] = ItemFactory::MakeItemYield(itemType, (yield_kg)yield);
        //     }
        // }
        void AddToInventory(ItemTypes itemType, double amount){
            if (find(itemType)){
                GetItem(itemType)->AddAmount((amount_units)amount);
            } else {
                inventory_map_[itemType] = ItemFactory::MakeItem(itemType, (amount_units)amount);
            }
        }
        void RemoveFromInventory(ItemTypes itemType);
        double CalcTotalMass();

        bool HasNoneOf(ItemTypes item);
        bool HasNoneOf(std::vector<ItemTypes> items);
        bool HasNoneOf(std::vector<int> items);

};
typedef std::shared_ptr<Inventory> InventoryPtr;

#endif // ITEMS_HPP