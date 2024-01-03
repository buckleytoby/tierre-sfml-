// include guards
#ifndef SENSES_HPP
#define SENSES_HPP

#include <memory>
#include <map>
#include "globals.hpp"
#include "resources.hpp"
#include "items.hpp"
#include "tiles.hpp"
#include "buildings.hpp"

class Surroundings
{
    public:
        std::vector<ResourcePtr> resources_;
        std::vector<BuildingPtr> buildings_;
        // std::map<ItemTypes, std::shared_ptr<Item>> items_;

        double range_{0.0}; // square "radius"
        double x_{0.0}, y_{0.0};

        void AddResource(std::shared_ptr<Resource> resource_ptr);
        void SetResource(std::vector<ResourcePtr> resources);
        void SetBuilding(std::vector<BuildingPtr> buildings);
        void AddItem(std::shared_ptr<Item> item_ptr);
        Rect<double> GetLocalRect();
};

class ImmediateSurroundings : public Surroundings
{
    public:
        ImmediateSurroundings(){range_ = 0.5;}
        ImmediateSurroundings(double range){range_ = range;}
};

class NearbySurroundings : public Surroundings
{
    public:
        NearbySurroundings(){range_ = 10.0;}
        NearbySurroundings(double range){range_ = range;}
};

#endif // SENSES_HPP