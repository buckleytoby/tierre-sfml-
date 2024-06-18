#include "items.hpp"

const std::string to_string(ItemTypes p){
    switch(p)
    {
        case ItemTypes::NONE: return "";
    }
    return ""; // or an empty string
}
const std::string to_full_string(ItemTypes p){
    switch(p)
    {
        case ItemTypes::NONE: return "NONE";
        case ItemTypes::CORN: return "Corn";
        case ItemTypes::CORNSEED: return "Corn Seed";
        case ItemTypes::PLANTFIBER: return "Plant Fiber";
        case ItemTypes::PLANTROPE: return "Plant Rope";
        case ItemTypes::WOODRAFT: return "Wood Raft";
        case ItemTypes::PLANTBASKET: return "Plant Basket";
        case ItemTypes::LUMBER: return "Lumber";
        case ItemTypes::STONE: return "Stone";
        case ItemTypes::WATER: return "Water";
        case ItemTypes::GRASS: return "Grass";
        case ItemTypes::CORNSTALK: return "Corn Stalk";
    }
    return ""; // or an empty string
}

bool Item::operator==(const Item& other){
    return item_type_ == other.item_type_ && level_ == other.level_;
}

/////////////////////////////////////// ResourceItem ///////////////////////////////////////





/////////////////////////////////////// End ResourceItem ///////////////////////////////////////
/////////////////////////////////////// ItemFactory ///////////////////////////////////////
std::map<ItemTypes, ItemPtr> ItemFactory::item_blueprints_ = {
    {ItemTypes::NONE, std::make_shared<Item>(ItemTypes::NONE, 0.0)},
    {ItemTypes::CORN, std::make_shared<CornItem>(0.0)},
    {ItemTypes::PLANTBASKET, std::make_shared<PlantBasket>(0.0)},
};
/////////////////////////////////////// End ItemFactory ///////////////////////////////////////

double Inventory::CalcTotalMass(){
    double total_mass = 0.0;
    for (auto& item : inventory_map_){
        total_mass += item.second->CalcMass();
    }
    return total_mass;
}
bool Inventory::HasNoneOf(ItemTypes item_type){
    return GetItem(item_type)->GetAmount() <= 0.0;
}
bool Inventory::HasNoneOf(std::vector<ItemTypes> item_types){
    bool none = true;
    for (auto& item_type : item_types){
        none &= HasNoneOf(item_type);
    }
    return none;
}
bool Inventory::HasNoneOf(std::vector<int> item_ids){
    // convert these ids into item types
    std::vector<ItemTypes> item_types;
    for (auto& item_id : item_ids){
        item_types.push_back(static_cast<ItemTypes>(item_id));
    }
    return HasNoneOf(item_types);
}