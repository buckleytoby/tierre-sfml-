#include "taskmanager.hpp"


const std::string to_string(ActionTypes action_type_){
    switch (action_type_){
        case ActionTypes::Default:
            return "Default";
        case ActionTypes::NESTED:
            return "NESTED";
        case ActionTypes::SelectClosestBuilding:
            return "SelectClosestBuilding";
        case ActionTypes::SelectBuilding:
            return "SelectBuilding";
        case ActionTypes::SelectResource:
            return "SelectResource";
        case ActionTypes::MoveTowardsGoal:
            return "MoveTowardsGoal";
        case ActionTypes::TransferInventory:
            return "TransferInventory";
        case ActionTypes::SetGoalToBuilding:
            return "SetGoalToBuilding";
        case ActionTypes::SetGoalToSelectedBuilding:
            return "SetGoalToSelectedBuilding";
        case ActionTypes::SetGoalToSelectedResource:
            return "SetGoalToSelectedResource";
        case ActionTypes::TakeInventory:
            return "TakeInventory";
        case ActionTypes::TakeItems:
            return "TakeItems";
        case ActionTypes::Construct:
            return "Construct";
        case ActionTypes::Gather:
            return "Gather";
        case ActionTypes::GatherArea:
            return "GatherArea";
        case ActionTypes::Craft:
            return "Craft";
        case ActionTypes::TransferItems:
            return "TransferItems";
        case ActionTypes::Infer:
            return "Infer";
        default:
            return "UNKNOWN";
    }
}
std::vector<ActionTypes> COREACTIONS = {
    ActionTypes::Gather,
    ActionTypes::GatherArea,
    ActionTypes::Construct,
    ActionTypes::Craft,
    ActionTypes::Infer,
};
////////////////////////////// Action //////////////////////////////
Action::Action(ActionTypes action_type, int x, int y){
    action_type_ = action_type;
    AddID(x);
    AddID(y);
}
Action::Action(ActionTypes action_type, std::shared_ptr<Rect<double>> rect){
    action_type_ = action_type;
    identifier_rects_.push_back(rect);
}

void Action::update(bool is_complete){
    completed_ = is_complete;
}
Action* Action::GetActiveAction(){
    return this;
}
SetGoalToBuilding::SetGoalToBuilding(int x, int y): Action(ActionTypes::SetGoalToBuilding){
    AddID(x);
    AddID(y);
}
// void SetGoalToBuilding::update(bool is_complete){
//     // get building ptr from id
//     // auto building_ptr = GetBuildingPtrFromId(universal_ids_[0]);
//     // // set goal to building center
//     // SetGoal(building_ptr->GetCenter().x_, building_ptr->GetCenter().y_);
// }
SelectBuilding::SelectBuilding(BuildingPtr ptr): Action(ActionTypes::SelectBuilding){
    SetBuilding(ptr);
}
SelectResource::SelectResource(ResourcePtr ptr): Action(ActionTypes::SelectResource){
    SetResource(ptr);
}
void Action::SetTile(TilePtr ptr){
    tile_ptr_ = ptr;
    if (tile_ptr_ != nullptr){
        SetBuilding(tile_ptr_->GetBuildingPtr());
        SetResource(tile_ptr_->GetResource());
    }
}
////////////////////////////// End Action //////////////////////////////
////////////////////////////// Task //////////////////////////////
Task::Task(std::string name){
    name_ = name;
}
bool Task::ValidTask(){
    return actions_.size() > 0;
}
void Task::onFirst(){
    if (ValidTask()){
        // special check for the very first time the task is run
        if (IsFirst()){
            GetActiveAction()->onEnter();
            first_ = false;
        }
    }
}
void Task::update(bool is_complete){
    // update the active action, recursion if action is a task
    GetActiveAction()->update(is_complete);
    // test success criteria
    if(GetActiveAction()->IsComplete()){
        GetActiveAction()->onComplete();
        IncrementActiveAction();
        onIncrement();
        GetActiveAction()->onEnter(); // new task
    }
}
void Task::onIncrement(){
    if (on_increment_cb_ != nullptr)
        on_increment_cb_();
}
bool Task::IsComplete(){
    // no actions -> complete
    // all actions complete -> complete
    // else, not complete
    bool complete = true;
    for (auto action: actions_){
        complete = complete && action->IsComplete();
    }
    return complete;
}
ActionTypes Task::GetActiveActionType(){
    // recurse
    return actions_[active_action_nb_]->GetActiveActionType();
}
Action* Task::GetActiveAction(){
    // recurse
    return actions_[active_action_nb_]->GetActiveAction();
}
void Task::IncrementActiveAction(){
    prior_action_ = active_action_nb_;
    active_action_nb_ = (active_action_nb_ + 1) % actions_.size();
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
TaskPtr Task::copy(){
    TaskPtr task_ptr = std::make_shared<Task>(name_);
    for (auto action: actions_){
        task_ptr->AddAction(action);
    }
    return task_ptr;
}
////////////////////////////// End Task //////////////////////////////
////////////////////////////// TaskManager //////////////////////////////
TaskManager::TaskManager(std::shared_ptr<Map> map_ptr, std::shared_ptr<GUI> gui): Interactive()
{
    // ctor
    SetHandleInputCb(std::bind(&TaskManager::onHandleInput, this, std::placeholders::_1));
    map_ptr_ = map_ptr;
    gui_ptr_ = gui;
}
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
    for (int i=0; i<static_cast<int>(ActionTypes::END); i++){
        out.push_back(to_string(static_cast<ActionTypes>(i)));
    }

    // iterate through all custom tasks
    for (auto& taskptr: tasks_){
        out.push_back(taskptr->GetName());
    }
    return out;
}
TaskPtr TaskManager::CopyTask(int idx){
    // value protection
    if (idx < 0 || idx >= tasks_.size()){
        return nullptr;
    }
    return tasks_[idx]->copy();
}
void TaskManager::EnterEditMode(){
    if (!in_easy_edit_mode_){
        in_easy_edit_mode_ = true;
        // reset the easy edit task
        easy_edit_task_ = std::make_shared<Task>("Easy Edit Task");
        std::cout<<"EnterEditMode"<<std::endl;
    }
}
void TaskManager::ExitEditMode(){
    if (in_easy_edit_mode_){
        in_easy_edit_mode_ = false;
        // give the easy edit task to the selected dynamic objects
        map_ptr_->SetTasks(easy_edit_task_);
        // set state to EXECUTINGTASK
        map_ptr_->SetStates(WorkerStates::EXECUTINGTASK);
        // clear widgets
        widgets_.clear();
    }
}
void TaskManager::InferAction(){
    // add action to easy edit task
    auto tile = map_ptr_->GetTile();

    // check if tile is nullptr
    if (tile == nullptr)
        return;

    if (tile->HasBuildingPtr()){
        // check if building can be constructed
        if (tile->CanConstruct()){
            easy_edit_task_->AddAction(map_ptr_->MakeAction(ActionTypes::Construct));
        }
        // if building can be crafted
        else if (tile->CanCraft()){
            easy_edit_task_->AddAction(map_ptr_->MakeAction(ActionTypes::Craft));
        }
        // if building is storage
        else if (tile->GetBuildingPtr()->IsCategory(BuildingCategory::STORAGE)){
            // make selectitems widget
            auto ptr = std::make_shared<SelectItems>(400, 400, 400, 400);
            ptr->SetOnClickCallback([this, ptr, tile](){
                // cb: set this worker's task to the selected idxs from the dropdown menu
                auto items = ptr->GetSelectedItems();
                // convert items to ints
                std::vector<int> items_int;
                for (auto item: items){
                    items_int.push_back(static_cast<int>(item));
                }
                if (ptr->taking){
                    auto ptr = std::make_shared<Action>(ActionTypes::TakeItems);
                    ptr->SetBuilding(tile->GetBuildingPtr());
                    ptr->SetIDs(items_int);
                    easy_edit_task_->AddAction(ptr);
                } else {
                    auto ptr = std::make_shared<Action>(ActionTypes::TransferItems);
                    ptr->SetBuilding(tile->GetBuildingPtr());
                    ptr->SetIDs(items_int);
                    easy_edit_task_->AddAction(ptr);
                }
                return true;
            });

            // add selectitems widget to widgets_
            widgets_.push_back(ptr);
        }
            
    } 
    else if (tile->HasResource()){
        // if can gather
        if (tile->CanGather()){
            easy_edit_task_->AddAction(map_ptr_->MakeAction(ActionTypes::Gather));
        }
    }
}
void TaskManager::Update(){
    // update all widgets
    for (auto& widget: widgets_){
        widget->Update(DELTATIME, mouse_x_, mouse_y_);
    }
}
void TaskManager::Draw(sf::RenderWindow& window){
    // draw the task manager
    if (in_easy_edit_mode_){
        // iterate through widgets_ and draw
        for (auto& widget: widgets_){
            widget->draw(window, sf::Transform::Identity);
        }
    }
}  
////////////////////////////// End TaskManager //////////////////////////////