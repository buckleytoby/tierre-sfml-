#include "taskmanager.hpp"


////////////////////////////// Action //////////////////////////////
void Action::update(double dt){
    // call the callback
    update_callback_();
}
const std::string Action::to_string(){
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
////////////////////////////// End Action //////////////////////////////
////////////////////////////// Task //////////////////////////////
void Task::update(double dt){
    // update the active action
    actions_[active_action_]->update(dt);
    // test success criteria
    if(actions_[active_action_]->IsComplete()){
        IncrementActiveAction();
    }
}
void Task::IncrementActiveAction(){
    active_action_ = (active_action_ + 1) % actions_.size();
}
std::vector<std::string> Task::GetActionNames(){
    std::vector<std::string> action_names;
    for (auto action: actions_){
        action_names.push_back(action->to_string());
    }
    return action_names;
}
////////////////////////////// End Task //////////////////////////////
////////////////////////////// TaskManager //////////////////////////////
std::vector<std::string> TaskManager::GetTaskNames(){
    std::vector<std::string> task_names;
    for (auto task: tasks_){
        task_names.push_back(task->task_name_);
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
////////////////////////////// End TaskManager //////////////////////////////