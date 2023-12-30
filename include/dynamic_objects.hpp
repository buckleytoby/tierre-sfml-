// include guards
#ifndef DYNAMIC_OBJECTS_HPP
#define DYNAMIC_OBJECTS_HPP

#include "globals.hpp"
#include "BitWise.hpp"
#include <memory>
#include <map>
#include <functional>

#include "resources.hpp"
#include "skills.hpp"
#include "senses.hpp"
#include "map.hpp"
#include "taskmanager.hpp"

// following this post: https://stackoverflow.com/a/6515232
enum class DynamicObjectInputs
{
    NONE,
    HANDLED,
};

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
        void ResetDynamicObjectActions(){dynamic_object_actions_.ClearFlag();}
        bool IsType(DynamicObjectTypes dynamic_object_type){return dynamic_object_type_ == dynamic_object_type;}
};
typedef std::shared_ptr<DynamicObject> DynamicObjectPtr;

enum class WorkerInputs
{
    NONE,
    HANDLED,
};
enum class WorkerStates{
    DEAD,
    IDLE,
    ACTIVE,
    EXECUTINGTASK,
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
        void update(double dt){
            double new_val = std::max(0.0, val_ - deterioration_rate_ * dt);
            val_ = new_val;}
        bool IsCritical(){return val_ < danger_level_;}
        bool IsAutoFulfillable(){return val_ < auto_fulfill_level_;}
        bool IsZero(){return val_ <= 0.0;}
};

class FoodNeed : public Need
{
    public:
        FoodNeed(){need_type_ = NeedsTypes::FOOD; val_ = 100.0; deterioration_rate_ = 0.5; auto_fulfill_level_ = 50.0;}
};
class WaterNeed : public Need
{
    public:
        WaterNeed(){need_type_ = NeedsTypes::WATER; val_ = 100.0; deterioration_rate_ = 0.5; auto_fulfill_level_ = 50.0;}
};
class SleepNeed : public Need
{
    public:
        SleepNeed(){need_type_ = NeedsTypes::SLEEP; val_ = 100.0; deterioration_rate_ = 0.5; auto_fulfill_level_ = 50.0;}
};


class Worker : public DynamicObject
{
    private:
        using inherited = DynamicObject;
    public:
        WorkerStates worker_state_{WorkerStates::IDLE};
        Inventory inventory_;
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
        WorkerInputs HandleInput(sf::Event& event);
        void AI(double dt);
        void SetGoal(double x, double y);
        void SetState(WorkerStates worker_state);
        XY<double> GetCenter(){return XY<double>{footprint_.x_ + footprint_.width_ / 2.0, footprint_.y_ + footprint_.height_ / 2.0};}
        Rect<double> GetImmediateSurroundingsRect();
        Rect<double> GetNearbySurroundingsRect();
        void Die();
        bool CheckState(WorkerStates worker_state){return worker_state_ == worker_state;}
        void SetAttention(BuildingPtr building_ptr){selected_building_ptr_ = building_ptr;}
        bool HasAttention(){return selected_building_ptr_ != nullptr;}

        // Passthroughs
        void AddToInventory(ItemTypes type, double amount){inventory_.AddToInventory(type, amount);}
        ItemMap GetInventoryMap(){return inventory_.GetItemMap();}

        // Action Primitives
        void SelectBuilding(BuildingPtr building_ptr);
        DefaultActionFcn SelectClosestBuilding();
        DefaultActionFcn SetGoalToSelectedBuilding();
        DefaultActionFcn MoveTowardsGoal();
        void Gather(double dt);
        void Construct(double dt);
        void Craft(double dt);
        void TransferItem(ItemTypes item_type, double amount_request, BuildingPtr building_ptr);
        void TransferInventory();
        void TakeItem(ItemTypes item_type, double amount_request, BuildingPtr building_ptr);
        DefaultActionFcn TakeInventory();
        DefaultActionFcn Eat();
        void InferAction(double dt);
        void EngageWithBuilding(BuildingPtr building_ptr);
        // End Action Primitives


        // Action primitive map
        // reference: https://stackoverflow.com/questions/3113139/how-to-create-mapstring-classmethod-in-c-and-be-able-to-search-for-functi
        std::map<std::string, std::function<void()>> action_primitive_map_{
            {"SelectClosestBuilding", std::bind(&Worker::SelectClosestBuilding, this)},
            {"SetGoalToSelectedBuilding", std::bind(&Worker::SetGoalToSelectedBuilding, this)},
            {"MoveTowardsGoal", std::bind(&Worker::MoveTowardsGoal, this)},
            {"TransferInventory", std::bind(&Worker::TransferInventory, this)},
        };
        TaskManager task_manager_;
        void ExecuteTask(double dt);

        // Pre-built test tasks
        void MakeTask1();

        void GatherAndStoreLumber(double dt);
};

#endif // DYNAMIC_OBJECTS_HPP