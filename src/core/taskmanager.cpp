#include "taskmanager.hpp"


const std::string to_string(ActionTypes action_type_){
    switch (action_type_){
        case ActionTypes::Default:
            return "Default";
        case ActionTypes::SelectClosestBuilding:
            return "SelectClosestBuilding";
        case ActionTypes::SetGoalToSelectedBuilding:
            return "SetGoalToSelectedBuilding";
        case ActionTypes::MoveTowardsGoal:
            return "MoveTowardsGoal";
        case ActionTypes::TransferInventory:
            return "TransferInventory";
        default:
            return "Unknown";
    }
}
////////////////////////////// Action //////////////////////////////
void Action::update(double dt){
    // call the callback
    update_callback_();
}
const std::string Action::to_string(){
    return to_string(action_type_);
}
////////////////////////////// End Action //////////////////////////////
SetGoalToBuilding::SetGoalToBuilding(int building_id){
    universal_ids_.push_back(building_id);
}
////////////////////////////// Task //////////////////////////////
void Task::update(double dt){
    // update the active action, recursion if action is a task
    actions_[active_action_]->update(dt);
    // test success criteria
    if(actions_[active_action_]->IsComplete()){
        IncrementActiveAction();
    }
}
bool Task::IsComplete(){
    // Method called in the update recursion.

    if (active_action_ == 0 && active_action_ != prior_action_){
        return true;
    } else {
        return false;
    }
}
void Task::IncrementActiveAction(){
    prior_action_ = active_action_;
    active_action_ = (active_action_ + 1) % actions_.size();
}
std::vector<std::string> Task::GetActionNames(){
    std::vector<std::string> action_names;
    for (auto action: actions_){
        action_names.push_back(action->GetName());
    }
    return action_names;
}
void Task::NewAndAddAction(ActionTypes type){
    auto action = std::make_shared<Action>(type);
    AddAction(action);
}
////////////////////////////// End Task //////////////////////////////
////////////////////////////// TaskManager //////////////////////////////
std::vector<std::string> TaskManager::GetTaskNames(){
    std::vector<std::string> task_names;
    for (auto task: tasks_){
        task_names.push_back(task->GetName());
    }
    return task_names;
}
std::vector<std::string> TaskManager::GetActiveTaskActionNames(){
    if (active_task_ == nullptr){
        return {};
    }
    return active_task_->GetActionNames();
}
void TaskManager::NewTask(){
    int idx = tasks_.size();
    std::string name = "New Task " + std::to_string(idx);
    tasks_.push_back(std::make_shared<Task>(name));
    active_task_ = tasks_.back();
}
void TaskManager::RemoveTask(int idx){
    // value protection
    if (idx < 0 || idx >= tasks_.size()){
        return;
    }
    tasks_.erase(tasks_.begin() + idx);
}
void TaskManager::EditTask(int idx){
    // value protection
    if (idx < 0 || idx >= tasks_.size()){
        return;
    }
    active_task_ = tasks_[idx];
}
std::vector<std::string> TaskManager::GetActionsAndTasks(){
    std::vector<std::string> out;

    // iterate through all ActionTypes enum
    for (auto type: ActionTypes){
        out.push_back(to_string(type));
    }

    // iterate through all custom tasks
    for (auto& taskptr: tasks_){
        out.push_back(taskptr->GetName());
    }
    return out;
}
////////////////////////////// End TaskManager //////////////////////////////