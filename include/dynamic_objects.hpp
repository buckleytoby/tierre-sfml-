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
#include "taskmanager.hpp"
#include "utils.hpp"
#include "gameobject.hpp"

// forward decls
class Task; typedef std::shared_ptr<Task> TaskPtr;
class Surroundings;
class ImmediateSurroundings;
class NearbySurroundings;

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

class DynamicObject: public GameObject
{
    public:
        double speed_{0}; // in meters per second
        BitFlag dynamic_object_actions_{0};
        DynamicObjectTypes dynamic_object_type_{DynamicObjectTypes::GENERIC};

        // params
        double goal_dist_threshold_{0.1}; // in meters, euclidean distance
        double interaction_dist_{0.5}; // in meters, euclidean distance

        // vars
        double action_time_{0.0};
        XY<double> goal_{0.0, 0.0};

        // attention (by priority)
        GameObjectPtr selected_gameobject_;
        std::shared_ptr<Building> selected_building_ptr_{nullptr};
        ResourcePtr selected_resource_ptr_{nullptr};
        TilePtr selected_tile_ptr_{nullptr};
        std::shared_ptr<Rect<double>> attention_rect_;

        // senses
        std::shared_ptr<ImmediateSurroundings> immediate_surroundings_;
        std::shared_ptr<NearbySurroundings> nearby_surroundings_;
        
        // Constructor
        DynamicObject(std::string name);

        // virtual functions
        virtual void selfUpdate(double dt);
        virtual void AI(double dt){};

        void SetSpeed(double speed);
        void SetDynamicObjectType(DynamicObjectTypes dynamic_object_type){dynamic_object_type_ = dynamic_object_type;}
        void ResetDynamicObjectActions(){dynamic_object_actions_.ClearFlag();}
        bool IsType(DynamicObjectTypes dynamic_object_type){return dynamic_object_type_ == dynamic_object_type;}

        // attention methods
        void ClearAttention();
        void AttentionClear(){selected_gameobject_ = nullptr;}
        void AttentionClearResource(){selected_resource_ptr_ = nullptr;}
        void AttentionClearBuilding(){selected_building_ptr_ = nullptr;}
        void AttentionChanged(){action_time_ = 0.0;}
        void SetAttention(BuildingPtr building_ptr);
        void SetAttention(ResourcePtr ptr);
        void SetAttention(TilePtr ptr);
        void SetAttention(std::shared_ptr<Rect<double>> rect){attention_rect_ = rect;}
        void SetAttention(TaskPtr ptr);

        bool HasAttention();
        bool AttentionHasObject(){return selected_gameobject_ != nullptr;}
        bool AttentionHasRect(){return attention_rect_ != nullptr;}
        bool AttentionHasBuilding(){return selected_building_ptr_ != nullptr;}
        bool AttentionHasResource(){return selected_resource_ptr_ != nullptr;}
        bool AttentionHasTile(){return selected_tile_ptr_ != nullptr;}
        
        void SetGoalToSelectedGameObject();
        void SetGoalToSelectedBuilding();
        void SetGoalToSelectedResource();


        std::shared_ptr<Rect<double>> GetAttentionRect(){return attention_rect_;}

        sf::Rect<double> GetBoundingRect(){return sf::Rect<double>(bounds_m_.x, bounds_m_.y, bounds_m_.width, bounds_m_.height);}

        
        void SelectBuilding(BuildingPtr building_ptr);
        void SelectResource(ResourcePtr resource_ptr);
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
    INFERRING,
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
const std::string to_string(WorkerStates p);
const std::string to_full_string(WorkerStates p);
enum class NeedsTypes{
    FOOD,
    SLEEP,
    WATER,
};

const std::string to_string(NeedsTypes p);

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

enum class WorkerAttributeTypes{
    NONE,
    STRENGTH,
    SPEED,
};

enum class WorkerStats{
    CURRCARRY,
    MAXCARRY,
};
std::string to_string(WorkerStats p);


extern std::map<ItemTypes, WorkerAttributeTypes> ITEM_TO_WORKER_ATTRIBUTE_MAP;

class Attribute
{
    public:
        double val_{0.0};
        WorkerAttributeTypes attribute_type_{WorkerAttributeTypes::NONE};

        Attribute(){}
        Attribute(WorkerAttributeTypes type, double val){
            attribute_type_ = type;
            val_ = val;
        }
        WorkerAttributeTypes GetType(){return attribute_type_;}
        double GetVal(){return val_;}
        void SetVal(double val){val_ = val;}
};


class Worker : public DynamicObject
{
    private:
        using inherited = DynamicObject;
    public:
        WorkerStates worker_state_{WorkerStates::IDLE};
        WorkerStates task_worker_state_{WorkerStates::IDLE};
        Inventory inventory_;
        std::map<SkillTypes, SkillPtr> skill_map_;
        std::map<NeedsTypes, Need> needs_map_;
        std::map<WorkerAttributeTypes, Attribute> attribute_map_;
        std::map<WorkerStats, double> stats_map_;

        // vars
        bool is_male_;
        bool changed_state_{false};

        // triggers
        bool must_update_stats_{true};
        bool infer_actions_{false};
        
        // Core Methods
        Worker();
        void selfUpdate(double dt);
        virtual HandleInputNS::InputResult onHandleInput(sf::Event& event);
        void AI(double dt);
        void Reset();
        void SetGoal(double x, double y);
        void SetState(WorkerStates worker_state);
        void ClearState();
        Rect<double> GetImmediateSurroundingsRect();
        Rect<double> GetNearbySurroundingsRect();
        void Die();
        bool CheckState(WorkerStates worker_state);
        void UpdateStats();
        bool CanGather(ResourcePtr ptr);
        bool InventoryHasRoom(ResourcePtr ptr);

        // Virtuals
        

        // Passthroughs
        bool AddToInventory(ItemTypes type, double yield);
        ItemMap GetInventoryMap(){return inventory_.GetItemMap();}

        bool SelectClosestBuilding();
        bool MoveTowardsGoal();
        bool CheckandMoveTowardsGoal();
        void TakeInventory();
        void TakeNonRecipeInventory();
        void Eat();
        void BeginGather();
        void BeginGather(double dt);
        bool FindGatherResource();
        void GatherArea(double dt);
        void BeginConstruct();
        bool CanConstruct(BuildingPtr ptr);
        bool FindConstructBuilding();
        void Gather(double dt);
        void GatherCore(double dt);
        void ExecuteCoreAction(double dt, std::function<bool()> FindFcn, std::function<void(double)> CoreFcn, std::function<bool()> CheckFcn);
        void Construct(double dt);
        void ConstructCore(BuildingPtr building_ptr, double dt);
        bool FindCraftBuilding();
        void Craft(double dt);
        void TransferItem(ItemTypes item_type, double amount_request, BuildingPtr building_ptr);
        void TransferItemCore(ItemTypes item_type, double amount_request, std::shared_ptr<Building> building_ptr);
        void TransferItems();
        bool TransferInventory();
        void TransferInventoryCore();
        bool CanTakeItems();
        void TakeItems();
        void TakeItemCore(ItemTypes item_type, double amount_request, BuildingPtr building_ptr);

        // Action Primitives
        void actionSelectClosestBuilding();
        void actionTransferInventory();
        void actionMoveTowardsGoal();
        void actionSetGoalToSelectedResource();
        void actionSetGoalToSelectedBuilding();
        void actionSelectBuilding();
        void actionSelectResource();
        void actionGather();
        void actionGatherArea();
        void actionConstruct();
        void actionTakeInventory();
        void actionTakeItems();
        void actionTransferItems();
        void actionCraft();
        void actionInfer();
        // End Action Primitives

        // Action Inference
        bool InferActionWide(double dt);
        bool InferAction(double dt);
        bool InferAction(double dt, BuildingPtr ptr);
        bool InferAction(double dt, ResourcePtr ptr);
        bool InferAction(double dt, TilePtr ptr);
        bool IsCloseEnough(GameObjectPtr ptr);

        void CraftingIdle(double dt);

        bool CanCraft(BuildingPtr ptr);

        void CraftCore(double dt);

        // Action primitive map
        // reference: https://stackoverflow.com/questions/3113139/how-to-create-mapstring-classmethod-in-c-and-be-able-to-search-for-functi
        std::map<ActionTypes, std::function<void()>> action_primitive_map_
        {
            {ActionTypes::SelectClosestBuilding, std::bind(&Worker::actionSelectClosestBuilding, this)},
            {ActionTypes::SetGoalToSelectedBuilding, std::bind(&Worker::actionSetGoalToSelectedBuilding, this)},
            {ActionTypes::SetGoalToSelectedResource, std::bind(&Worker::actionSetGoalToSelectedResource, this)},
            {ActionTypes::MoveTowardsGoal, std::bind(&Worker::actionMoveTowardsGoal, this)},
            {ActionTypes::TransferInventory, std::bind(&Worker::actionTransferInventory, this)},
            {ActionTypes::TransferItems, std::bind(&Worker::actionTransferItems, this)},
            {ActionTypes::TakeItems, std::bind(&Worker::actionTakeItems, this)},
            {ActionTypes::TakeInventory, std::bind(&Worker::actionTakeInventory, this)},
            {ActionTypes::SelectBuilding, std::bind(&Worker::actionSelectBuilding, this)},
            {ActionTypes::SelectResource, std::bind(&Worker::actionSelectResource, this)},
            {ActionTypes::Construct, std::bind(&Worker::actionConstruct, this)},
            {ActionTypes::Gather, std::bind(&Worker::actionGather, this)},
            {ActionTypes::GatherArea, std::bind(&Worker::actionGatherArea, this)},
            {ActionTypes::Craft, std::bind(&Worker::actionCraft, this)},
            {ActionTypes::Infer, std::bind(&Worker::actionInfer, this)},
        };
        TaskPtr task_ptr_{nullptr};
        void SetTask(TaskPtr ptr);
        void ExecuteTask(double dt);

};
typedef std::shared_ptr<Worker> WorkerPtr;

#endif // DYNAMIC_OBJECTS_HPP