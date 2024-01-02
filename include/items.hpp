// include guard
#ifndef ITEMS_HPP
#define ITEMS_HPP

#include "globals.hpp"
#include "BitWise.hpp"
#include "resources.hpp"
#include <map>

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

    // copy paste from ResourceTypes
    LUMBER = 128,
    STONE,
    WATER,
    GRASS,
    CORNSTALK,
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
        double amount_{0.0};

        Item(){ItemTypes::NONE;}
        Item(ItemTypes item_type, double amount){item_type_ = item_type; amount_ = amount;}
        void SetAmount(double amount){
            double new_amount = std::max(0.0, amount);
            amount_ = new_amount;
        }
        double GetAmount(){return amount_;}
        void AddAmount(double amount){amount_ += amount;}
        void RemoveAmount(double amount){
            SetAmount(amount_ - amount);
            }
        bool IsItemCategory(ItemCategories item_category){return item_category == item_category_;}
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
        FoodItem(){};
};

class CornItem: public FoodItem
{
    public:
        CornItem(double amount){item_type_ = ItemTypes::CORN; item_category_ = ItemCategories::FOOD, amount_ = amount; nutrient_amount_ = 10.0;}
};

// TODO: make item factory
class ItemFactory
{
    public:
        static std::shared_ptr<Item> MakeItem(ItemTypes item_type, double amount){
            switch (item_type)
            {
                case ItemTypes::CORN:
                    return std::make_shared<CornItem>(amount);
                    break;
                default:
                    return std::make_shared<Item>(item_type, amount);
                    break;
            }
        }
};

class Inventory
{
    public:
        ItemMap inventory_map_;
        
        ItemMap GetItemMap(){return inventory_map_;}
        ItemPtr GetItem(ItemTypes type){return inventory_map_[type];}
        bool find(ItemTypes itemType){return inventory_map_.find(itemType) != inventory_map_.end();}
        void AddToInventory(ItemTypes itemType, double amount){
            if (find(itemType)){
                GetItem(itemType)->AddAmount(amount);
            } else {
                inventory_map_[itemType] = ItemFactory::MakeItem(itemType, amount);
            }
        }
        void RemoveFromInventory(ItemTypes itemType);

};

#endif // ITEMS_HPP