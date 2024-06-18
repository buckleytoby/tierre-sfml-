// include guards
#ifndef SENSES_HPP
#define SENSES_HPP

#include <memory>
#include <map>
#include "globals.hpp"
#include "utils.hpp"
#include "resources.hpp"
#include "items.hpp"
#include "tiles.hpp"
#include "buildings.hpp"
#include "map.hpp"
// #include "dynamic_objects.hpp"

// forward decls
class DynamicObject;
class Map;

class Surroundings
{
    public:
        Map* map_ref_; // more efficient to simply keep a reference to the map and query it for stuff
        std::vector<ResourcePtr> resources_;
        std::vector<BuildingPtr> buildings_;
        std::vector<std::shared_ptr<DynamicObject>> dynamic_objects_;
        // std::map<ItemTypes, std::shared_ptr<Item>> items_;

        double range_{0.0}; // square "radius"
        double x_{0.0}, y_{0.0};

        // ctor
        Surroundings(){};
        Surroundings(Map* map_ref){map_ref_ = map_ref;}

        void AddResource(std::shared_ptr<Resource> resource_ptr);
        void SetResource(std::vector<ResourcePtr> resources);
        void SetBuilding(std::vector<BuildingPtr> buildings);
        void SetDynamicObjects(std::vector<std::shared_ptr<DynamicObject>> dynamic_objects);
        void AddItem(std::shared_ptr<Item> item_ptr);
        ResourcePtr GetResource(double x, double y);
        BuildingPtr GetBuilding(double x, double y);
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