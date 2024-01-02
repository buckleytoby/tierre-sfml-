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
        case ItemTypes::CORNSEED: return "Corn Seed";
        case ItemTypes::LUMBER: return "Lumber";
        case ItemTypes::STONE: return "Stone";
        case ItemTypes::WATER: return "Water";
        case ItemTypes::GRASS: return "Grass";
        case ItemTypes::CORN: return "Corn";
    }
    return ""; // or an empty string
}
/////////////////////////////////////// ResourceItem ///////////////////////////////////////




/////////////////////////////////////// End ResourceItem ///////////////////////////////////////