// include guards
#ifndef MAP_HPP
#define MAP_HPP

#include "tiles.hpp"
#include <vector>
#include <memory>
#include "dynamic_objects.hpp"

enum class MapInputs
{
    NONE,
    HANDLED,
};  
enum class MapActions
{
    Flag1 = 1 << 0, // 1
    Flag2 = 1 << 1, // 2
    Flag3 = 1 << 2, // 4
    Flag4 = 1 << 3, // 8
    Flag5 = 1 << 4, // 16
    Flag6 = 1 << 5, // 32
    Flag7 = 1 << 6, // 64
    Flag8 = 1 << 7  // 128
};

class Map {
    public:
        int width_, height_; // in meters
        double mouse_x_{0}, mouse_y_{0}; // in meters
        std::vector<std::vector<std::shared_ptr<Tile>>> tiles_array_;
        std::vector<std::shared_ptr<DynamicObject>> dynamic_object_ptrs_;
        std::vector<DynamicObjectPtr> selected_dynamic_object_ptrs_;
        std::shared_ptr<Tile> selected_tile_ptr_;

        void SetWidth(int width);
        void SetHeight(int height);
        void SetMousePosition(double x, double y){mouse_x_ = x; mouse_y_ = y;}
        void CreateTiles();
        void SetTile(int x, int y, std::shared_ptr<Tile> tile);
        std::vector<std::shared_ptr<Tile>> GetTileSlice(Rect<double> rect);
        std::shared_ptr<Tile> GetTile(int x, int y);
        void update(double dt);
        MapInputs HandleInput(sf::Event& event);
        std::map<ResourceTypes, std::shared_ptr<Resource>> GetResourceSlice(Rect<double> rect);
        std::map<BuildingTypes, std::shared_ptr<Building>> GetBuildingSlice(Rect<double> rect);
        void SelectObject(double x, double y);
        void SelectDynamicObject(double x, double y);
        void MakeBuilding(BuildingTypes building_type, double x, double y);
        void MakeWorker(double x, double y);
        std::vector<DynamicObjectPtr> GetSelectedObjects(){return selected_dynamic_object_ptrs_;}
        DynamicObjectPtr GetFirstSelectedObject(){
            if (selected_dynamic_object_ptrs_.size() > 0){
                return selected_dynamic_object_ptrs_[0];
            } else {
                return nullptr;
            }
        }


        // Passthroughs
        void SetAttention(DynamicObjectPtr ptr, double x, double y);
        void SetAttention(double x, double y);
        void SetGoal(DynamicObjectPtr ptr, double x, double y);
        void MoveTowardsGoal(DynamicObjectPtr ptr);
        void SetAttentionAndMove(DynamicObjectPtr ptr, double x, double y);
        void SetAttentionAndMove(double x, double y);
};
typedef std::shared_ptr<Map> MapPtr;

#endif // MAP_HPP