// include guard
#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>
#include <algorithm>
#include "sfml/Graphics.hpp"

#include "globals.hpp"
#include "utils.hpp"
#include "BitWise.hpp"
#include "gameobject.hpp"

enum class ResourceTypes {
    NONE,
    LUMBER = 128,
    STONE,
    WATER,
    GRASS,
    CORNSTALK,
};
enum class ResourceAttributes {
    NONE,
    GATHERABLE,
    NAVIGABLE,
};
enum class ResourceStatus {
    IDLE,
    GATHERING,
    DEPLETED,
};

const std::string to_string(ResourceTypes p);
const std::string to_full_string(ResourceTypes p);
const sf::Color to_color(ResourceTypes p);

// this is the blueprint
class Resource: public GameObject
{
    public:
        // resource immutable parameters
        ResourceTypes resource_type_;
        ResourceStatus resource_status_;
        int nb_gatherers_{0};
        BitFlag resource_attributes_;
        double time_to_gather_; // seconds
        double yield_per_gather_; // kg

        // resource mutable parameters
        double total_{0.0};
        int level_{0}; // level of resource, 0 is base level

        // resource methods
        Resource(std::string name);
        ResourceTypes GetResourceType(){return resource_type_;}
        bool CheckState(ResourceStatus resource_status){return resource_status_ == resource_status;}
        void IncrementGatherers();
        void DecrementGatherers();
        void SetTotal(double total){total_ = total;}
        double GetTotal(){return total_;}
        void AddTotal(double total){total_ += total;}
        void RemoveTotal(double total);
        double GetYieldPerGather(){return yield_per_gather_;}
        double Extract();
        void Draw(sf::RenderTarget& target);
        virtual std::string to_string(){return std::string("Resource");}


        // attribute check
        bool isGatherable();
        bool isNavigable();

        // checkers
        bool HasResource(){return GetTotal() > 0;}
        bool CanGather();
};
typedef std::shared_ptr<Resource> ResourcePtr;

class Lumber : public Resource
{
    public:
        Lumber(double total);
        virtual std::string to_string(){return std::string("Lumber");}
};
class Stone : public Resource
{
    public:
        Stone(double total);
        virtual std::string to_string(){return std::string("Stone");}
};

class CornstalkResource : public Resource
{
    public:
        CornstalkResource(double total);
        virtual std::string to_string(){return std::string("CornstalkResource");}
};

class Grass: public Resource
{
    public:
        Grass(double total);
};

class ResourceFactory
{
    public:
        static std::shared_ptr<Resource> MakeResource(ResourceTypes resource_type, double val){
            switch (resource_type)
            {
                case ResourceTypes::LUMBER:
                    return std::make_shared<Lumber>(val);
                    break;
                case ResourceTypes::STONE:
                    return std::make_shared<Stone>(val);
                    break;
                case ResourceTypes::CORNSTALK:
                    return std::make_shared<CornstalkResource>(val);
                    break;
                case ResourceTypes::GRASS:
                    return std::make_shared<Grass>(val);
                    break;
                default:
                    return std::make_shared<Resource>("default");
                    break;
            }
        }
};

#endif // RESOURCES_HPP