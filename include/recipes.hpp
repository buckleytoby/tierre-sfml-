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

const std::string to_full_string(RecipeTypes p);

class Recipe {
    public:
        RecipeTypes type_;
        std::map<ItemTypes, double> inputs_; // ItemTypes -> amount needed
        std::map<ItemTypes, double> outputs_; // ItemTypes -> amount produced
        double effort_req_{1.0}; // effort "units"

        Recipe(RecipeTypes type){type_ = type;}
        void SetInputs(std::map<ItemTypes, double> inputs){inputs_ = inputs;}
        void SetOutputs(std::map<ItemTypes, double> outputs){outputs_ = outputs;}
        RecipeTypes GetType(){return type_;}
};
typedef std::shared_ptr<Recipe> RecipePtr;

class ProcessCornstalk : public Recipe
{
    public:
        std::map<ItemTypes, double> inputs_{
            {ItemTypes::CORNSTALK, 1}
        };
        std::map<ItemTypes, double> outputs_{
            {ItemTypes::CORN, 1},
            {ItemTypes::CORNSEED, 1},
            {ItemTypes::PLANTFIBER, 1}
        };

        ProcessCornstalk(): Recipe(RecipeTypes::PROCESSCORNSTALK)
        {
            effort_req_ = DEBUG? 1.0: 2.0;
            SetInputs(inputs_);
            SetOutputs(outputs_);
        }
};
class MakePlantRope : public Recipe
{
    public:
        std::map<ItemTypes, double> inputs_{
            {ItemTypes::PLANTFIBER, 1}
        };
        std::map<ItemTypes, double> outputs_{
            {ItemTypes::PLANTROPE, 1}
        };

        MakePlantRope(): Recipe(RecipeTypes::MAKEPLANTROPE)
        {
            effort_req_ = 1.0;
            SetInputs(inputs_);
            SetOutputs(outputs_);
        }
};

class MakeWoodRaft : public Recipe
{
    public:
        std::map<ItemTypes, double> inputs_{
            {ItemTypes::LUMBER, 10},
            {ItemTypes::PLANTROPE, 10}
        };
        std::map<ItemTypes, double> outputs_{
            {ItemTypes::WOODRAFT, 1}
        };

        MakeWoodRaft(): Recipe(RecipeTypes::MAKEWOODRAFT)
        {
            effort_req_ = DEBUG? 1.0 : 60.0;
            SetInputs(inputs_);
            SetOutputs(outputs_);
        }
};

class FarmCorn : public Recipe
{
    public:
        std::map<ItemTypes, double> inputs_{
            {ItemTypes::WATER, 1},
            {ItemTypes::CORNSEED, 1}
        };
        std::map<ItemTypes, double> outputs_{
            {ItemTypes::CORN, 1}
        };

        FarmCorn(): Recipe(RecipeTypes::FARMCORN)
        {
            effort_req_ = DEBUG? 1.0: 10.0;
            SetInputs(inputs_);
            SetOutputs(outputs_);
        }
};


#endif // RECIPES_HPP