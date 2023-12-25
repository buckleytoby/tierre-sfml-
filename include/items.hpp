// include guard
#ifndef ITEMS_HPP
#define ITEMS_HPP

#include "globals.hpp"
#include "BitWise.hpp"
#include "resources.hpp"


enum class ItemTypes {
    NONE,
    RESOURCE,
};
class Item
{
    public:
        ItemTypes item_type_;

        Item(){ItemTypes::NONE;}
        Item(ItemTypes item_type){item_type_ = item_type;}
};





class ResourceItem: public Item
{
    public:
        ResourceTypes resource_type_;
        double amount_{0.0};

        ResourceItem(){ResourceTypes::NONE;}
        ResourceItem(ResourceTypes resource_type, double amount){resource_type_ = resource_type; amount_ = amount;}
        void SetAmount(double amount){amount_ = amount;}
        double GetAmount(){return amount_;}
        void AddAmount(double amount){amount_ += amount;}
};

#endif // ITEMS_HPP