#include "taskmanager.hpp"


////////////////////////////// Action //////////////////////////////
void Action::update(double dt){
    // call the callback
    update_callback_();
}
const std::string Action::to_string(){
    switch (action_type_){
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
////////////////////////////// End Task //////////////////////////////
////////////////////////////// TaskManager //////////////////////////////
void TaskManager::Execute(double dt){
    // execute the active task
    if (active_task_ != nullptr){
        active_task_->update(dt);
    }
}
////////////////////////////// End TaskManager //////////////////////////////