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
    NESTED, // special enum type
    SelectClosestBuilding,
    SetGoalToSelectedBuilding,
    MoveTowardsGoal,
    TransferInventory,
    SetGoalToBuilding,
    TakeInventory,
    end
};
// define functions for each action type
typedef std::function<void()> DefaultActionFcn;
typedef std::function<void()> SelectClosestBuildingFcn;

// success callback
typedef std::function<bool()> SuccessFcn;

// to string
const std::string to_string(ActionTypes action_type_);

class Action
{
    public:
        ActionTypes action_type_{ActionTypes::Default};
        std::string name_;
        std::vector<int> universal_ids_; // integers used to identify action-specific objects in game
        bool completed_{false};

        Action(){};
        Action(ActionTypes action_type){action_type_ = action_type;}

        // virtuals
        virtual std::string GetName(){return to_string(action_type_);}
        virtual ActionTypes GetActiveActionType(){return action_type_;}
        virtual void update(bool is_complete);
        virtual bool IsComplete(){return completed_;}
};
typedef std::shared_ptr<Action> ActionPtr;

class SetGoalToBuilding: public Action
{
    public:

        SetGoalToBuilding(int building_id);
        virtual void update(bool is_complete);
};

// inherit from action so that tasks themselves can be added to actions_
class Task : public Action 
{
    public:
        std::vector<ActionPtr> actions_;
        int active_action_nb_{0};
        int prior_action_{0};

        Task(std::string name){name_ = name;}

        void IncrementActiveAction();
        void AddAction(ActionPtr action){actions_.push_back(action);}
        void RemoveAction(){actions_.pop_back();}
        void NewAndAddAction(ActionTypes type);
        void Reset(){active_action_nb_ = 0;}
        void Clear(){actions_.clear(); Reset();}
        std::vector<std::string> GetActionNames();
        std::shared_ptr<Task> copy();

        // virtuals
        virtual std::string GetName(){return name_;}
        virtual ActionTypes GetActiveActionType();
        virtual void update(bool is_complete);
        virtual bool IsComplete();
};
typedef std::shared_ptr<Task> TaskPtr;

// follow the pattern in this link: 
// reference: https://stackoverflow.com/questions/3113139/how-to-create-mapstring-classmethod-in-c-and-be-able-to-search-for-functi
// it's like a callback pattern
class TaskManager
{
    public:
        std::vector<TaskPtr> tasks_;
        TaskPtr active_task_{nullptr}; // the one being edited

        void AddTask(TaskPtr task){tasks_.push_back(task);}
        void NewTask();
        void RemoveTask(int idx);
        void EditTask(int idx);

        // getters
        TaskPtr GetTask(int idx);

        TaskManager(){}
        std::vector<std::string> GetTaskNames();
        std::vector<std::string> GetActiveTaskActionNames();
        std::vector<std::string> GetActionsAndTasks();
};
typedef std::shared_ptr<TaskManager> TaskManagerPtr;









#endif // TASKMANAGER_HPP