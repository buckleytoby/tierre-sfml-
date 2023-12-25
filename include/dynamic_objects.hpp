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
        double goal_dist_threshold_{0.5}; // in meters, euclidean distance
        

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
    BUILDING,

};

class Worker : public DynamicObject
{
    private:
        using inherited = DynamicObject;
    public:
        WorkerStates worker_state_{WorkerStates::IDLE};
        std::map<ResourceTypes, ResourceItem> inventory_map_;
        std::map<SkillTypes, Skill> skill_map_;
        ImmediateSurroundings immediate_surroundings_;
        NearbySurroundings nearby_surroundings_;

        // medium lvl vars
        XY<double> goal_{0.0, 0.0};
        double gather_time_{0.0};
        std::shared_ptr<Resource> gather_resource_ptr_{nullptr};

        

        Worker(){SetDynamicObjectType(DynamicObjectTypes::WORKER);}
        void update(double dt);
        void AI(double dt);
        void SetGoal(double x, double y);
        void SetState(WorkerStates worker_state){worker_state_ = worker_state;}
        void AddToInventory(ResourceTypes itemType, double amount);
        void RemoveFromInventory(ResourceTypes itemType);
        Rect<double> GetImmediateSurroundingsRect();
        Rect<double> GetNearbySurroundingsRect();

        // medium lvl actions
        void MoveTowardsGoal();
        void Gather(double dt);
};

#endif // DYNAMIC_OBJECTS_HPP