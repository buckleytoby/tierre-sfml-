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
        case ItemTypes::NONE: return "";
        case ItemTypes::CORN: return "Corn";
        case ItemTypes::CORNSEED: return "Corn Seed";
        case ItemTypes::PLANTFIBER: return "Plant Fiber";
        case ItemTypes::PLANTROPE: return "Plant Rope";
        case ItemTypes::WOODRAFT: return "Wood Raft";
        case ItemTypes::LUMBER: return "Lumber";
        case ItemTypes::STONE: return "Stone";
        case ItemTypes::WATER: return "Water";
        case ItemTypes::GRASS: return "Grass";
        case ItemTypes::CORNSTALK: return "Corn Stalk";
    }
    return ""; // or an empty string
}
/////////////////////////////////////// ResourceItem ///////////////////////////////////////




/////////////////////////////////////// End ResourceItem ///////////////////////////////////////