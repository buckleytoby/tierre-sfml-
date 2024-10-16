// include guard
#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include <memory>
#include <algorithm>
#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <SFML/Graphics.hpp>



// define functions for each action type
typedef std::function<void()> DefaultActionFcn;
typedef std::function<void()> SelectClosestBuildingFcn;

// success callback
typedef std::function<bool()> SuccessFcn;

enum class ActionTypes {
    START,
    Default,
    NESTED, // special enum type
    SelectClosestBuilding,
    SelectBuilding,
    SelectResource,
    SetGoalToSelectedBuilding,
    SetGoalToSelectedResource,
    MoveTowardsGoal,
    TransferInventory, // this is the entire inventory
    TransferItems, // specific items
    SetGoalToBuilding,
    TakeInventory,
    TakeItems, // specific items
    Construct,
    Gather,
    GatherArea,
    Craft,
    Infer,
    END
};
const std::string to_string(ActionTypes action_type_);
extern std::vector<ActionTypes> COREACTIONS;

#include "map.hpp"
#include "gui.hpp"
#include "dynamic_objects.hpp"
#include "interactive.hpp"

// class stubs
class Map;
enum class WorkerStates;
class GUI;


class Action
{
    public:
        ActionTypes action_type_{ActionTypes::Default};
        std::string name_;
        std::vector<int> universal_ids_; // integers used to identify action-specific objects in game
        std::vector<std::shared_ptr<Rect<double>>> identifier_rects_; // rectangles used to identify action-specific areas in game
        std::shared_ptr<Building> building_ptr_{nullptr};
        std::shared_ptr<Resource> resource_ptr_{nullptr};
        std::shared_ptr<Tile> tile_ptr_{nullptr};
        bool completed_{false};

        // cbs
        std::function<void()> on_enter_cb_{nullptr};
        std::function<void()> on_complete_cb_{nullptr};

        Action(){};
        Action(ActionTypes action_type){action_type_ = action_type;}
        Action(ActionTypes action_type, int x, int y);
        Action(ActionTypes action_type, std::shared_ptr<Rect<double>> rect);
        void AddID(int id){universal_ids_.push_back(id);}
        void SetIDs(std::vector<int> ids){universal_ids_ = ids;}
        void SetBuilding(BuildingPtr ptr){building_ptr_ = ptr;}
        void SetResource(ResourcePtr ptr){resource_ptr_ = ptr;}
        void SetTile(TilePtr ptr);
        void onEnter(){if (on_enter_cb_ != nullptr){on_enter_cb_();}}
        void onComplete(){if (on_complete_cb_ != nullptr){on_complete_cb_();}}

        // virtuals
        virtual std::string GetName(){return to_string(action_type_);}
        virtual ActionTypes GetActiveActionType(){return action_type_;}
        virtual Action* GetActiveAction();
        virtual void update(bool is_complete);
        virtual bool IsComplete(){return completed_;}
};
typedef std::shared_ptr<Action> ActionPtr;

class SetGoalToBuilding: public Action
{
    public:
        SetGoalToBuilding(int x, int y);
};
class SelectBuilding: public Action
{
    public:
        SelectBuilding(BuildingPtr ptr);
};
class SelectResource: public Action
{
    public:
        SelectResource(ResourcePtr ptr);
};

// inherit from action so that tasks themselves can be added to actions_
class Task : public Action 
{
    public:
        std::vector<ActionPtr> actions_;
        std::function<void()> on_increment_cb_{nullptr};
        int active_action_nb_{0};
        int prior_action_{0};
        bool first_{true};
        bool repeat_{true};

        Task(std::string name);

        bool ValidTask();

        void onFirst();

        void IncrementActiveAction();
        void AddAction(ActionPtr action){actions_.push_back(action);}
        void AddAction(ActionTypes type){actions_.push_back(std::make_shared<Action>(type));}
        void RemoveAction(){actions_.pop_back();}
        void NewAndAddAction(ActionTypes type);
        void Reset(){active_action_nb_ = 0;}
        void Clear(){actions_.clear(); Reset();}
        std::vector<std::string> GetActionNames();
        std::shared_ptr<Task> copy();
        void SetOnIncrementCB(std::function<void()> cb){on_increment_cb_ = cb;}
        void onIncrement();
        bool IsFirst(){return first_;}

        // virtuals
        virtual std::string GetName(){return name_;}
        virtual ActionTypes GetActiveActionType();
        virtual Action* GetActiveAction();
        virtual void update(bool is_complete);
        virtual bool IsComplete();
};
typedef std::shared_ptr<Task> TaskPtr;

// follow the pattern in this link: 
// reference: https://stackoverflow.com/questions/3113139/how-to-create-mapstring-classmethod-in-c-and-be-able-to-search-for-functi
// it's like a callback pattern
class TaskManager: public Interactive
{
    public:
        std::vector<TaskPtr> tasks_; // tasks created in the UI
        TaskPtr active_task_{nullptr}; // the one being edited in the UI
        bool in_easy_edit_mode_{false};
        TaskPtr easy_edit_task_{nullptr}; // the one being map-edited
        double mouse_x_, mouse_y_;

        // map handle
        std::shared_ptr<Map> map_ptr_{nullptr};

        // gui handle
        std::shared_ptr<GUI> gui_ptr_{nullptr};

        // widgets
        std::vector<WidgetPtr> widgets_;

        void AddTask(TaskPtr task){tasks_.push_back(task);}
        void NewTask();
        void RemoveTask(int idx);
        void EditTask(int idx);

        // getters
        TaskPtr CopyTask(int idx);
        TaskPtr GetTask(){return active_task_;}

        // ctor
        TaskManager(std::shared_ptr<Map> map_ptr, std::shared_ptr<GUI> gui);
        std::vector<std::string> GetTaskNames();
        std::vector<std::string> GetActiveTaskActionNames();
        std::vector<std::string> GetActionsAndTasks();
        void EnterEditMode();
        void ExitEditMode();
        void Draw(sf::RenderWindow& window);

        // handle inputs here?
        virtual HandleInputNS::InputResult onHandleInput(sf::Event event);
        void InferAction();

        void Update();
};
typedef std::shared_ptr<TaskManager> TaskManagerPtr;









#endif // TASKMANAGER_HPP