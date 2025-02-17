// include guards
#ifndef MAP_HPP
#define MAP_HPP

#include "tiles.hpp"
#include <vector>
#include <memory>
#include "dynamic_objects.hpp"
#include "worker.hpp"
#include "mapgen.hpp"
#include "utils.hpp"
#include "interactive.hpp"
#include "taskmanager.hpp"
#include "event_manager.hpp"

// class stubs
class DynamicObject; typedef std::shared_ptr<DynamicObject> DynamicObjectPtr;
class Task; typedef std::shared_ptr<Task> TaskPtr;
class TaskManager; typedef std::shared_ptr<TaskManager> TaskManagerPtr;
class Worker; typedef std::shared_ptr<Worker> WorkerPtr;
enum class WorkerStates;
enum class ActionTypes;
class Action; typedef std::shared_ptr<Action> ActionPtr;

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

class Map: public Interactive
{
    public:
        int width_;
        int height_; // in meters
        double mouse_x_{0};
        double mouse_y_{0}; // in meters
        std::vector<std::vector<std::shared_ptr<Tile>>> tiles_array_;
        std::vector<std::shared_ptr<DynamicObject>> dynamic_object_ptrs_;
        std::vector<DynamicObjectPtr> selected_dynamic_object_ptrs_;
        std::shared_ptr<Tile> selected_tile_ptr_;
        bool b_append_actions;

        // task mgr ref
        TaskManagerPtr task_manager_ptr_;
        void SetTaskManager(TaskManagerPtr ptr){task_manager_ptr_ = ptr;}
        bool HasTaskManager(){return task_manager_ptr_ != nullptr;}

        // ctor
        Map();

        void SetWidth(int width);
        void SetHeight(int height);
        void SetMousePosition(double x, double y){mouse_x_ = x; mouse_y_ = y;}
        void CreateTiles();
        void SetTile(int x, int y, std::shared_ptr<Tile> tile);
        std::vector<XY<int>> GetCoordsSlice(double x1, double y1, double x2, double y2);
        std::vector<XY<int>> GetCoordsSlice(Rect<double> rect);
        std::vector<std::shared_ptr<Tile>> GetTileSlice(Rect<double> rect);
        std::shared_ptr<Tile> GetTile(int x, int y);
        std::shared_ptr<Tile> GetTile(double x, double y);
        std::shared_ptr<Tile> GetTile();
        void update(double dt);
        virtual HandleInputNS::InputResult onHandleInput(sf::Event& event);
        ResourcePtr GetResource(double x, double y);
        BuildingPtr GetBuilding(double x, double y);
        std::vector<ResourcePtr> GetResourceSlice(Rect<double> rect);
        std::vector<BuildingPtr> GetBuildingSlice(Rect<double> rect);
        std::vector<DynamicObjectPtr> GetDynamicObjectSlice(Rect<double> bounds);
        void SelectObject(double x, double y);
        Rect<double> GetMouseRect();
        void SelectMouseObjects();
        void SelectDynamicObject(double x, double y);
        void MakeBuilding(BuildingTypes building_type);
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
        std::vector<TilePtr> GetEuclNeighbors(TilePtr tile_ptr);

        std::vector<TilePtr> GetNeighbors(TilePtr tile_ptr);

        // Passthroughs
        void SetAttention(DynamicObjectPtr ptr, double x, double y);
        void ClearAttention(DynamicObjectPtr ptr);
        void SetAttention(double x, double y);
        void SetGoal(DynamicObjectPtr ptr, double x, double y);
        void MoveTowardsGoal(DynamicObjectPtr ptr);
        void ClearState(DynamicObjectPtr ptr);
        void SetAttentionAndMove(DynamicObjectPtr ptr, double x, double y);
        void SetAttentionAndMove(double x, double y);
        void SetTask(WorkerPtr ptr, TaskPtr task_ptr);
        void SetState(WorkerPtr ptr, WorkerStates state);

        // Apply-to-all methods
        void SetTasks(TaskPtr task_ptr);
        void SetOnSelectToSelectUnits();
        void SetOnSelectToSetTask(ActionTypes action_type);
        void SetOnSelectToAddAction(ActionTypes action_type);
        void SetOnSelectFromContext(ActionTypes action_type);
        void SetStates(WorkerStates state);

        ActionPtr MakeAction(ActionTypes action_type);

        // selection & action members
        std::function<void()> on_select_cb_;
        double mouse_down_x_{0}, mouse_down_y_{0}; // in meters
        void SetOnSelectCB(std::function<void()> cb){on_select_cb_ = cb;}
        void SetMouseDownPosition(double x, double y){mouse_down_x_ = x; mouse_down_y_ = y;}
        void SetMouseDownPosition(){mouse_down_x_ = mouse_x_; mouse_down_y_ = mouse_y_;}
        void onSelect(){if (on_select_cb_ != nullptr){on_select_cb_();}}
        std::shared_ptr<Rect<double>> GetSelectRect();

        bool Atleast1WorkerSelected();

        void InferAction();
        void SubscriberSetSelectedWorkersState(WorkerStates new_state);
        void InferAction(DynamicObjectPtr ptr, double x, double y);
};
typedef std::shared_ptr<Map> MapPtr;

#endif // MAP_HPP