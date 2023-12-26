// include guard
#ifndef RECIPES_HPP
#define RECIPES_HPP

/*
Recipes turn items into other items
Recipes require a building to perform

Time penalty when building lvl < recipe lvl (e^(-d))
Denote natural resources using [n]

@Workspace:
corn seed: [n]corn -> corn seed

@Farm:
corn: water + corn seed -> corn

@Workshop:
plant rope: [n]plants -> plant rope
stone sickle: [n]lumber + [n]stone + plant rope -> stone sickle

@silo:
store grains

@supply depot
store any item



*/

#include <map>
#include <vector>
#include "resources.hpp"
#include "items.hpp"

enum class RecipeTypes {
    NONE,
    CORNSEED,
    CORN,
};

class Recipe {
    public:
        std::map<ItemTypes, double> inputs_; // ItemTypes -> amount needed
        std::map<ItemTypes, double> outputs_; // ItemTypes -> amount produced
        double effort_req_{10}; // effort "units"
};

class CornSeedRecipe : public Recipe
{
    public:

        CornSeedRecipe(){
            inputs_[ItemTypes::CORN] = 1;
            outputs_[ItemTypes::CORNSEED] = 1;
            effort_req_ = 2.0;
        }
};

class CornRecipe : public Recipe
{
    public:

        CornRecipe(){
            // inputs_[ItemTypes::WATER] = 1;
            inputs_[ItemTypes::CORNSEED] = 1;
            outputs_[ItemTypes::CORN] = 1;
            effort_req_ = 10.0;
        }
};


#endif // RECIPES_HPP