// include guards
#ifndef DYNAMIC_OBJECTS_HPP
#define DYNAMIC_OBJECTS_HPP

#include "globals.hpp"
#include "BitWise.hpp"
#include <memory>
#include <map>
#include "resources.hpp"
#include "skills.hpp"
#include "senses.hpp"
#include "map.hpp"

// following this post: https://stackoverflow.com/a/6515232

// Dynamic Object Superclass
enum class DynamicObjectActions
{
    NONE = 0,
    MOVE_LEFT = 1 << 0, // 1
    MOVE_RIGHT = 1 << 1, // 2
    MOVE_UP = 1 << 2, // 4
    MOVE_DOWN = 1 << 3, // 8
    Flag5 = 1 << 4, // 16
    Flag6 = 1 << 5, // 32
    Flag7 = 1 << 6, // 64
    Flag8 = 1 << 7  // 128
};
enum class DynamicObjectTypes
{
    GENERIC,
    WORKER,
};

class DynamicObject
{
    public:
        Rect<double> footprint_{0, 0, 1, 1};
        double speed_{0}; // in meters per second
        BitFlag dynamic_object_actions_{0};
        DynamicObjectTypes dynamic_object_type_{DynamicObjectTypes::GENERIC};

        // params
        double goal_dist_threshold_{0.25}; // in meters, euclidean distance
        

        // virtual functions
        virtual void update(double dt);
        virtual void AI(double dt){};

        void SetX(double x);
        void SetY(double y);
        void MoveX(double dx);
        void MoveY(double dy);
        void SetSpeed(double speed);
        void SetDynamicObjectType(DynamicObjectTypes dynamic_object_type){dynamic_object_type_ = dynamic_object_type;}
        bool PointInFootprint(double x, double y);
};

enum class WorkerStates{
    IDLE,
    MOVING,
    GATHERING,
    GATHERIDLE,
    CONSTRUCTING,
    CONSTRUCTINGIDLE,
    CRAFTING,
    CRAFTINGIDLE,
};
enum class NeedsTypes{
    FOOD,
    SLEEP,
    WATER,
};

class Need
{
    public:
        NeedsTypes need_type_{NeedsTypes::FOOD};
        double val_{0.0};
        double max_{100.0};
        double deterioration_rate_{0.1}; // per second
        double danger_level_{10.0}; // at this level the worker will prioritize this need over all else
        double auto_fulfill_level_{25.0}; // at this level the worker will automatically fulfill this need if they can
        void update(double dt){val_ -= deterioration_rate_ * dt;}
        bool IsCritical(){return val_ < danger_level_;}
        bool IsAutoFulfillable(){return val_ < auto_fulfill_level_;}
};

class FoodNeed : public Need
{
    public:
        FoodNeed(){need_type_ = NeedsTypes::FOOD; val_ = 100.0; deterioration_rate_ = 5.0; auto_fulfill_level_ = 50.0;}
};

class Worker : public DynamicObject
{
    private:
        using inherited = DynamicObject;
    public:
        WorkerStates worker_state_{WorkerStates::IDLE};
        std::map<ItemTypes, Item> inventory_map_;
        std::map<SkillTypes, Skill> skill_map_;
        std::map<NeedsTypes, Need> needs_map_{
            {NeedsTypes::FOOD, FoodNeed()}
        };
        ImmediateSurroundings immediate_surroundings_;
        NearbySurroundings nearby_surroundings_;

        // medium lvl vars
        XY<double> goal_{0.0, 0.0};
        double action_time_{0.0};
        std::shared_ptr<Resource> selected_resource_ptr_{nullptr};
        std::shared_ptr<Building> selected_building_ptr_{nullptr};
        

        Worker();
        void update(double dt);
        void AI(double dt);
        void SetGoal(double x, double y);
        void SetState(WorkerStates worker_state){worker_state_ = worker_state;}
        void AddToInventory(ItemTypes itemType, double amount);
        void RemoveFromInventory(ItemTypes itemType);
        Rect<double> GetImmediateSurroundingsRect();
        Rect<double> GetNearbySurroundingsRect();

        // medium lvl actions
        void MoveTowardsGoal();
        void Gather(double dt);
        void Construct(double dt);
        void Craft(double dt);
        void TransferItem(ItemTypes item_type, double amount_request, std::shared_ptr<Building> building_ptr);
        void TransferInventory();
        void TakeItem(ItemTypes item_type, double amount_request, std::shared_ptr<Building> building_ptr);
        void TakeInventory();
        void Eat();
};

#endif // DYNAMIC_OBJECTS_HPP