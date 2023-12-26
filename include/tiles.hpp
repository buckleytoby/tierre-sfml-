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
class Tile {
    public:
        std::map<ResourceTypes, std::shared_ptr<Resource>> resource_map_; // ResourceType -> Resource
        BuildingTypes building_type_{BuildingTypes::NONE}; // for now each tile can have 1 building
        std::shared_ptr<Building> building_ptr_;
        int x_{0}, y_{0}; // location in meters
        ArtCodes art_code_{0}; // used for buildings that take up multiple tiles

        std::shared_ptr<Resource> GetResource(ResourceTypes resource_type);
        void AddResource(std::shared_ptr<Resource> resource_ptr);
};

#endif // TILES_HPP