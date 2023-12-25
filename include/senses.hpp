// include guards
#ifndef SENSES_HPP
#define SENSES_HPP

#include <memory>
#include <map>
#include "globals.hpp"
#include "resources.hpp"
#include "items.hpp"
#include "tiles.hpp"

class Surroundings
{
    public:
        std::map<ResourceTypes, std::shared_ptr<Resource>> resources_;
        std::map<ItemTypes, std::shared_ptr<Item>> items_;

        double range_{0.0};
        double x_{0.0}, y_{0.0};

        void AddResource(std::shared_ptr<Resource> resource_ptr);
        void SetResource(std::map<ResourceTypes, std::shared_ptr<Resource>> resources);
        void AddItem(std::shared_ptr<Item> item_ptr);
        Rect<double> GetLocalRect();
};

class ImmediateSurroundings : public Surroundings
{
    public:
        ImmediateSurroundings();
};

class NearbySurroundings : public Surroundings
{
    public:
        NearbySurroundings();
};

#endif // SENSES_HPP