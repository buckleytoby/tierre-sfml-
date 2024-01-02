// include guard
#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>
#include <algorithm>
#include "sfml/Graphics.hpp"

enum class ResourceTypes {
    NONE,
    LUMBER = 128,
    STONE,
    WATER,
    GRASS,
    CORN,
};
const std::string to_string(ResourceTypes p);
const std::string to_full_string(ResourceTypes p);
const sf::Color to_color(ResourceTypes p);

class Resource
{
    public:
        // resource immutable parameters
        ResourceTypes resource_type_;
        double time_to_gather_; // seconds
        double yield_per_gather_; // kg

        // resource mutable parameters
        double total_{0.0};
        int level_{0}; // level of resource, 0 is base level

        // resource methods
        Resource(){};
        void SetTotal(double total){total_ = total;}
        double GetTotal(){return total_;}
        void AddTotal(double total){total_ += total;}
        void RemoveTotal(double total){total_ -= total;}
        double Extract(){
            // get min of GetTotal & yield_per_gather_
            double amount = std::min(GetTotal(), yield_per_gather_);
            RemoveTotal(amount);
            return amount;
        }

};

class Lumber : public Resource
{
    public:
        Lumber(double total){resource_type_ = ResourceTypes::LUMBER; time_to_gather_ = 1.0; yield_per_gather_ = 1.0; total_ = total;}
};
class Stone : public Resource
{
    public:
        Stone(double total){resource_type_ = ResourceTypes::STONE; time_to_gather_ = 2.0; yield_per_gather_ = 1.0; total_ = total;}
};

class CornResource : public Resource
{
    public:
        CornResource(double total){resource_type_ = ResourceTypes::CORN; time_to_gather_ = 1.0; yield_per_gather_ = 1.0; total_ = total;}
};


// TODO: make resource factory

#endif // RESOURCES_HPP