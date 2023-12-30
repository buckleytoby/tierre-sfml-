// include guard
#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include <memory>
#include <algorithm>
#include <vector>
#include <string>
#include <functional>
#include <iostream>


enum class ActionTypes {
    Default,
    SelectClosestBuilding,
    SetGoalToSelectedBuilding,
    MoveTowardsGoal,
    TransferInventory,
};
// define functions for each action type
typedef std::function<void()> DefaultActionFcn;
typedef std::function<void()> SelectClosestBuildingFcn;

// success callback
typedef std::function<bool()> SuccessFcn;

class Action
{
    public:
        ActionTypes action_type_{ActionTypes::Default};
        DefaultActionFcn update_callback_;
        SuccessFcn success_callback_;

        void update(double dt);
        bool IsComplete(){return success_callback_();}
        void SetUpdateCallback(DefaultActionFcn update_callback){update_callback_ = update_callback;}
        void SetSuccessCallback(SuccessFcn success_callback){success_callback_ = success_callback;}

        const std::string to_string();
};
typedef std::shared_ptr<Action> ActionPtr;

class Task
{
    public:
        std::vector<ActionPtr> actions_;
        int active_action_{0};
        std::string task_name_;

        void update(double dt);
        void IncrementActiveAction();
        void AddAction(ActionPtr action){actions_.push_back(action);}
        void Reset(){active_action_ = 0;}
        void Clear(){actions_.clear(); Reset();}
};
typedef std::shared_ptr<Task> TaskPtr;

// follow the pattern in this link: 
// reference: https://stackoverflow.com/questions/3113139/how-to-create-mapstring-classmethod-in-c-and-be-able-to-search-for-functi
// it's like a callback pattern
class TaskManager
{
    public:
        std::vector<TaskPtr> tasks_;
        TaskPtr active_task_{nullptr};

        void Execute(double dt);
        void AddTask(TaskPtr task){tasks_.push_back(task);}
        
};









#endif // TASKMANAGER_HPP