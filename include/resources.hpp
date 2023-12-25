// include guard
#ifndef RESOURCES_HPP
#define RESOURCES_HPP

enum class ResourceTypes {
    NONE,
    LUMBER,
    STONE,
    WATER,
    GRASS,
};

class Resource
{
    public:
        // resource immutable parameters
        ResourceTypes resource_type_;
        double time_to_gather_; // seconds
        double yield_per_gather_; // kg

        // resource mutable parameters
        double total_{0.0};

        // resource methods
        Resource(){};
        void SetTotal(double total){total_ = total;}
        double GetTotal(){return total_;}
        void AddTotal(double total){total_ += total;}
        void RemoveTotal(double total){total_ -= total;}

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

#endif // RESOURCES_HPP