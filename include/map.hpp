// include guards
#ifndef MAP_HPP
#define MAP_HPP

#include "tiles.hpp"
#include <vector>
#include <memory>
#include "dynamic_objects.hpp"

class Map {
    public:
        int width_, height_; // in meters
        std::vector<std::vector<std::shared_ptr<Tile>>> tiles_array_;
        std::vector<std::shared_ptr<DynamicObject>> dynamic_object_ptrs_;
        std::vector<std::shared_ptr<DynamicObject>> selected_dynamic_object_ptrs_;
        std::shared_ptr<Tile> selected_tile_ptr_;

        void SetWidth(int width);
        void SetHeight(int height);
        void CreateTiles();
        void SetTile(int x, int y, std::shared_ptr<Tile> tile);
        std::vector<std::shared_ptr<Tile>> GetTileSlice(Rect<double> rect);
        std::shared_ptr<Tile> GetTile(int x, int y);
        void update(double dt);
        std::map<ResourceTypes, std::shared_ptr<Resource>> GetResourceSlice(Rect<double> rect);
        std::map<BuildingTypes, std::shared_ptr<Building>> GetBuildingSlice(Rect<double> rect);
        void SelectObject(double x, double y);
        void SelectDynamicObject(double x, double y);
        void MakeBuilding(BuildingTypes building_type, double x, double y);
        void MakeWorker(double x, double y);
};

#endif // MAP_HPP