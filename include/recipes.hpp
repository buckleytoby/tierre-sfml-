// include guard
#ifndef RECIPES_HPP
#define RECIPES_HPP

/*
Recipes turn items into other items
Recipes require a building to craft

Time penalty when building lvl < recipe lvl (e^(-d))
Denote natural resources using [n]

@Workspace:
[n]cornstalk -> corn, cornseed, plantfiber
plantfiber -> plantrope
[n]lumber + [n]stone + plantrope -> stonesickle
[n]lumber + [n]stone + plantrope -> stoneaxe
[n]lumber + [n]stone + plantrope -> stonepickaxe
[n]lumber + [n]stone + plantrope -> stonehammer
[n]lumber + [n]stone + plantrope -> stonehoe

[n]lumber + plantrope -> woodraft
[n]lumber -> woodpaddle

@Farm:
water + cornseed -> cornstalk

@Workshop:
[all workspace recipes]


*/

#include <map>
#include <vector>
#include "resources.hpp"
#include "items.hpp"

enum class RecipeTypes {
    NONE,
    PROCESSCORNSTALK,
    MAKEPLANTROPE,
    MAKESTONESICKLE,
    MAKESTONEAXE,
    MAKESTONEPICKAXE,
    MAKESTONEHAMMER,
    MAKESTONEHOE,
    MAKEWOODRAFT,
    MAKEWOODPADDLE,
    FARMCORN,

};

class Recipe {
    public:
        std::map<ItemTypes, double> inputs_; // ItemTypes -> amount needed
        std::map<ItemTypes, double> outputs_; // ItemTypes -> amount produced
        double effort_req_{10}; // effort "units"
};

class ProcessCornstalk : public Recipe
{
    public:
        const static inline std::map<ItemTypes, double> inputs_{
            {ItemTypes::CORNSTALK, 1}
        };
        const static inline std::map<ItemTypes, double> outputs_{
            {ItemTypes::CORN, 1},
            {ItemTypes::CORNSEED, 1},
            {ItemTypes::PLANTFIBER, 1}
        };

        ProcessCornstalk(){
            effort_req_ = 2.0;
        }
};
class MakePlantRope : public Recipe
{
    public:
        const static inline std::map<ItemTypes, double> inputs_{
            {ItemTypes::PLANTFIBER, 1}
        };
        const static inline std::map<ItemTypes, double> outputs_{
            {ItemTypes::PLANTROPE, 1}
        };

        MakePlantRope(){
            effort_req_ = 1.0;
        }
};

class MakeWoodRaft : public Recipe
{
    public:
        const static inline std::map<ItemTypes, double> inputs_{
            {ItemTypes::LUMBER, 10},
            {ItemTypes::PLANTROPE, 10}
        };
        const static inline std::map<ItemTypes, double> outputs_{
            {ItemTypes::WOODRAFT, 1}
        };

        MakeWoodRaft(){
            effort_req_ = 60.0;
        }
};

class FarmCorn : public Recipe
{
    public:
        const static inline std::map<ItemTypes, double> inputs_{
            {ItemTypes::WATER, 1},
            {ItemTypes::CORNSEED, 1}
        };
        const static inline std::map<ItemTypes, double> outputs_{
            {ItemTypes::CORN, 1}
        };

        FarmCorn(){
            effort_req_ = 10.0;
        }
};


#endif // RECIPES_HPP