#include "recipes.hpp"



const std::string to_full_string(RecipeTypes p){
    switch(p)
    {
        case RecipeTypes::NONE: return "";
        case RecipeTypes::PROCESSCORNSTALK: return "Process Cornstalk";
        case RecipeTypes::MAKEPLANTROPE: return "Make Plant Rope";
        case RecipeTypes::MAKESTONESICKLE: return "Make Stone Sickle";
        case RecipeTypes::MAKESTONEAXE: return "Make Stone Axe";
        case RecipeTypes::MAKESTONEPICKAXE: return "Make Stone Pickaxe";
        case RecipeTypes::MAKESTONEHAMMER: return "Make Stone Hammer";
        case RecipeTypes::MAKESTONEHOE: return "Make Stone Hoe";
        case RecipeTypes::MAKEWOODRAFT: return "Make Wood Raft";
        case RecipeTypes::MAKEWOODPADDLE: return "Make Wood Paddle";
        case RecipeTypes::FARMCORN: return "Farm Corn";
    }
    return ""; // or an empty string
}