// include guards
#ifndef TILES_HPP
#define TILES_HPP

#include <memory>
#include <map>
#include "resources.hpp"
#include "buildings.hpp"

enum class ArtCodes {
};

// each Tile is 1x1 meters
class Tile: public GameObject
{
    public:
        ResourcePtr resource_ptr_; // ResourceType -> Resource
        BuildingTypes building_type_{BuildingTypes::NONE}; // for now each tile can have 1 building
        std::shared_ptr<Building> building_ptr_;
        int x_{0}, y_{0}; // location in meters
        ArtCodes art_code_{0}; // used for buildings that take up multiple tiles

        Tile(int i, int j);
        ResourcePtr GetResource(){return resource_ptr_;}
        void SetResource(ResourcePtr resource_ptr){resource_ptr_ = resource_ptr;}
        bool HasResource(){return resource_ptr_ != nullptr;}
        void AddResource(ResourcePtr resource_ptr);
        bool HasBuildingPtr(){return building_ptr_ != nullptr;}
        void SetBuildingPtr(BuildingPtr building_ptr){building_ptr_ = building_ptr;}
        BuildingPtr GetBuildingPtr(){return building_ptr_;}
        int GetX(){return x_;}
        int GetY(){return y_;}

        // passthroughs
        bool CanConstruct();
        bool CanGather();
        bool CanCraft();
};
typedef std::shared_ptr<Tile> TilePtr;

#endif // TILES_HPP