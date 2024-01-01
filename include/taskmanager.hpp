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
    SETGOALTOBUILDING,
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
        DefaultActionFcn update_callback_;
        SuccessFcn success_callback_;
        std::string name_;
        std::vector<int> universal_ids_; // integers used to identify action-specific objects in game

        virtual void update(double dt);
        virtual bool IsComplete(){return success_callback_();}
        void SetUpdateCallback(DefaultActionFcn update_callback){update_callback_ = update_callback;}
        void SetSuccessCallback(SuccessFcn success_callback){success_callback_ = success_callback;}

        const std::string to_string();
        virtual std::string GetName(){return to_string();}
};
typedef std::shared_ptr<Action> ActionPtr;

class SetGoalToBuilding: public Action
{
    public:

        SetGoalToBuilding(int building_id);
        virtual void update(double dt);
};

// inherit from action so that tasks themselves can be added to actions_
class Task : public Action 
{
    public:
        std::vector<ActionPtr> actions_;
        int active_action_{0};
        int prior_action_{0};

        Task(std::string name){name_ = name;}

        virtual void update(double dt);
        virtual bool IsComplete();
        void IncrementActiveAction();
        void AddAction(ActionPtr action){actions_.push_back(action);}
        void RemoveAction(){actions_.pop_back();}
        void NewAndAddAction(ActionTypes type);
        void Reset(){active_action_ = 0;}
        void Clear(){actions_.clear(); Reset();}
        std::vector<std::string> GetActionNames();
        virtual std::string GetName(){return name_;}
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
        TaskPtr GetTask(int idx){return tasks_[idx];}

        TaskManager(){}
        std::vector<std::string> GetTaskNames();
        std::vector<std::string> GetActiveTaskActionNames();
        std::vector<std::string> GetActionsAndTasks();
};
typedef std::shared_ptr<TaskManager> TaskManagerPtr;









#endif // TASKMANAGER_HPP