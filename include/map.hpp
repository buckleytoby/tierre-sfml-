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
        std::vector<std::vector<Tile>> tiles_array_;
        std::vector<std::shared_ptr<DynamicObject>> dynamic_object_ptrs_;
        std::vector<std::shared_ptr<DynamicObject>> selected_dynamic_object_ptrs_;

        void SetWidth(int width);
        void SetHeight(int height);
        void CreateTiles();
        void SetTile(int x, int y, Tile tile);
        void GetMapSlice(int x, int y, int width, int height);
        Tile* GetTile(int x, int y);
        void update(double dt);
        std::map<ResourceTypes, std::shared_ptr<Resource>> GetResourceSlice(Rect<double> rect);
        void SelectDynamicObject(double x, double y);
};

#endif // MAP_HPP