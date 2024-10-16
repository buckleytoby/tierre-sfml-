#include "widgets.hpp"

SelectedStatus::SelectedStatus(double x, double y, double w, double h, MapPtr map_ref, TaskManagerPtr task_mgr_ptr): Widget(x, y, w, h)
{
    map_ref_ = map_ref;
    task_manager_ptr_ = task_mgr_ptr;

    // border rectangle
    border_ = std::make_shared<RectWidget>(0, 0, w, h);
    text_ = std::make_shared<TextBox>(0, 0, "Status: ");
    AddChild(border_);
    AddChild(text_);

    //// map-level actions
    //
    button_unit_selector = std::make_shared<Button>(50, 450, "Select Units");
    button_unit_selector->SetOnClickCallback([this](){
        map_ref_->SetOnSelectToSelectUnits();
        return true;
    });
    AddChild(button_unit_selector);


    //// core actions
    // make idx_to_action_type_map_
    for (int i=0; i < COREACTIONS.size(); i++){
        core_actions_.push_back(to_string(COREACTIONS[i]));
        idx_to_action_type_map_[i] = COREACTIONS[i];
    }
    button_core_actions = std::make_shared<Button>(50, 500, "Core Actions");
    list_core_actions = std::make_shared<Dropdown>(200, 500, core_actions_);
    list_core_actions->MakeInvisible();
    // button triggers visibility
    button_core_actions->SetOnClickCallback([this](){
        list_core_actions->ToggleVisibility();
        return true;
    });
    //
    list_core_actions->SetOnClickCallback([this](){
        auto idx = list_core_actions->GetClickedIdx();
        auto enumval = idx_to_action_type_map_[idx];
        map_ref_->SetOnSelectFromContext(enumval);
        list_core_actions->MakeInvisible();
        return true;
    });
    AddChild(button_core_actions);
    AddChild(list_core_actions);

    //// buildings
    // make idx_to_building_type_map_
    for (int i=0; i < BUILDINGTYPES.size(); i++){
        buildings_.push_back(to_string(BUILDINGTYPES[i]));
        idx_to_building_type_map_[i] = BUILDINGTYPES[i];
    }
    button_buildings = std::make_shared<Button>(50, 550, "Buildings");
    list_buildings = std::make_shared<Dropdown>(200, 550, buildings_);
    list_buildings->MakeInvisible();
    // button triggers visibility
    button_buildings->SetOnClickCallback([this](){
        list_buildings->ToggleVisibility();
        return true;
    });
    //
    list_buildings->SetOnClickCallback([this](){
        auto idx = list_buildings->GetClickedIdx();
        auto enumval = idx_to_building_type_map_[idx];
        map_ref_->MakeBuilding(enumval);
        list_buildings->MakeInvisible();
        return true;
    });
    AddChild(button_buildings);
    AddChild(list_buildings);

    // add all children
    AddChild(button_active_task_);
    AddChild(tasks_list_);
    AddChild(building_widget_);
    AddChild(button_building_widget_);
}
void SelectedStatus::onUpdate(double dt, double x, double y){
    // check if selected unit changed. If so, trigger a redraw
    auto unit = map_ref_->GetFirstSelectedObject();
    auto tile = map_ref_->selected_tile_ptr_;
    if (unit != selected_unit_ptr_){
        selected_unit_ptr_ = unit;
        reDraw();
    }
    if (tile != selected_tile_ptr_){
        selected_tile_ptr_ = tile;
        reDraw();
    }

    //// redraw all the real-time stuff
    std::string str = "";
    // make string for selected unit
    if (map_ref_->selected_dynamic_object_ptrs_.size() > 0){
        str = "Selected Object Status: \n";
        auto dynamic_object_ptr = map_ref_->selected_dynamic_object_ptrs_[0];

        str += "Selected Unit: ";
        if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::WORKER){
            auto worker = std::static_pointer_cast<Worker>(dynamic_object_ptr);
            str += "Worker";
            // add inventory
            str += "\nInventory: \n  ";
            for (auto& item : worker->GetInventoryMap()){
                str += to_full_string(item.first);
                str += ": ";
                str += std::to_string(item.second->GetAmount());
                str += "\n  ";
            }
            // add state
            str += "\nState: ";
            str += to_string(worker->worker_state_);
            // if gathering, add gather progress
            if (worker->worker_state_ == WorkerStates::GATHERING){
                str += "\nGathering " + to_full_string(worker->selected_resource_ptr_->resource_type_) + " Progress: ";
                str += std::to_string(worker->action_time_) + " / " + std::to_string(worker->selected_resource_ptr_->time_to_gather_);
            }
            // if executing task, add task progress
            else if (worker->worker_state_ == WorkerStates::EXECUTINGTASK){
                if (worker->task_ptr_ != nullptr){
                    str += "\nExecuting Task Progress: \n";
                    for (int i=0; i<worker->task_ptr_->actions_.size(); i++){
                        if (i == worker->task_ptr_->active_action_nb_){
                            str += ">> ";
                        }
                        str += worker->task_ptr_->actions_[i]->GetName() + "\n";
                    }
                }
            }
            // add needs
            str += "\nNeeds: \n  ";
            for (auto& need : worker->needs_map_){
                str += to_string(need.first);
                str += ": ";
                str += std::to_string(need.second.val_);
                str += "\n  ";
            }

            // add stats
            str += "\nStats: \n  ";
            for (auto& stat : worker->stats_map_){
                str += to_string(stat.first);
                str += ": ";
                str += std::to_string(stat.second);
                str += "\n  ";
            }


        } else if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::GENERIC){
            str += "Generic";
        }
        // add position
        str += "\nPos: " + std::to_string(dynamic_object_ptr->GetX());
        str += ", ";
        str += std::to_string(dynamic_object_ptr->GetY());




    }
    // end make string for selected unit
    
    // make string for selected tile
    if (map_ref_->selected_tile_ptr_ != nullptr){
        str += "Selected Tile: ";
        // add position
        str += "\nPos: " + std::to_string(map_ref_->selected_tile_ptr_->x_);
        str += ", ";
        str += std::to_string(map_ref_->selected_tile_ptr_->y_);
        // add resources
        str += "\nResources: ";
        if(map_ref_->selected_tile_ptr_->HasResource()){
            auto resource = map_ref_->selected_tile_ptr_->GetResource();
            str += to_full_string(resource->GetResourceType());
            str += ": ";
            str += std::to_string(resource->GetTotal());
            str += ", ";
        }
        // add building
        if (map_ref_->selected_tile_ptr_->building_type_ != BuildingTypes::NONE){
            str += "\nBuilding: ";
            str += to_string(map_ref_->selected_tile_ptr_->building_type_);
            str += "\nBuilding Status: " + to_full_string(map_ref_->selected_tile_ptr_->building_ptr_->GetStatus());
            str += "\nInventory: \n  ";
            for (auto& item : map_ref_->selected_tile_ptr_->building_ptr_->GetItemMap()){
                str += to_full_string(item.first);
                str += ": ";
                str += std::to_string(item.second->GetAmount());
                str += "\n  ";
            }
            // switch over building status
            switch (map_ref_->selected_tile_ptr_->building_ptr_->GetStatus()){
                case BuildingStatus::PRECONSTRUCTION:
                    // iterate through item_reqs_map
                    str += "\nItem Reqs: \n  ";
                    for (auto& item : map_ref_->selected_tile_ptr_->building_ptr_->item_reqs_map_){
                        str += to_full_string(item.first);
                        str += ": ";
                        str += std::to_string(item.second);
                        str += "\n  ";
                    }
                    break;
                case BuildingStatus::CONSTRUCTION:
                    str += "\nConstruction Effort: " + std::to_string(map_ref_->selected_tile_ptr_->building_ptr_->effort_val_);
                    str += " / " + std::to_string(map_ref_->selected_tile_ptr_->building_ptr_->construction_effort_req_);
                    break;
                case BuildingStatus::READY:
                    if (!(map_ref_->selected_tile_ptr_->building_ptr_->HasActiveRecipe()))
                        break;
                        
                    str += "\n  Recipe \n  " + to_full_string(map_ref_->selected_tile_ptr_->building_ptr_->GetActiveRecipe()->GetType()) + "\n    Reqs: ";
                    for (auto item : map_ref_->selected_tile_ptr_->building_ptr_->GetActiveRecipe()->inputs_){
                        str += to_full_string(item.first);
                        str += ": ";
                        str += std::to_string(item.second);
                        str += "\n  ";
                    }
                    break;
                case BuildingStatus::OPERATING:
                    str += "\n      Recipe Progress: " + std::to_string(map_ref_->selected_tile_ptr_->building_ptr_->effort_val_);
                    str += " / " + std::to_string(map_ref_->selected_tile_ptr_->building_ptr_->GetActiveRecipe()->effort_req_);
                    break;
            }

        }
    }
    RemoveChild(text_);
    text_ = std::make_shared<TextBox>(30, 30, str);
    AddChild(text_);
}
void SelectedStatus::reDraw(){
    // redraw widgets that require persistence but also sometimes change
    if (selected_unit_ptr_ != nullptr){
        if (selected_unit_ptr_->dynamic_object_type_ == DynamicObjectTypes::WORKER){
            auto worker = std::dynamic_pointer_cast<Worker>(selected_unit_ptr_);
            
            // Active Task Button
            std::string task_str = "No task";
            if (worker->task_ptr_ != nullptr){
                task_str = worker->task_ptr_->GetName();
            }

            // active tasks list
            RemoveChild(tasks_list_);
            tasks_list_ = std::make_shared<Dropdown>(200, 600, task_manager_ptr_->GetTaskNames());
            AddChild(tasks_list_);
            tasks_list_->SetOnClickCallback([this, worker](){
                // cb: set this worker's task to the selected idx from the dropdown menu
                worker->SetTask(task_manager_ptr_->CopyTask(tasks_list_->GetClickedIdx()));
                reDraw();
                return true;
            });
            tasks_list_->MakeInvisible();
            
            RemoveChild(button_active_task_);
            button_active_task_ = std::make_shared<Button>(50, 600, "Active Task: \n" + task_str);
            AddChild(button_active_task_);
            button_active_task_->SetOnClickCallback([this](){
                tasks_list_->ToggleVisibility();
                return true;
            });

        }
    } else {
        tasks_list_ = nullptr;
        button_active_task_ = nullptr;
    }

    // tile GUI
    if (selected_tile_ptr_ != nullptr){
        if (selected_tile_ptr_->building_type_ != BuildingTypes::NONE){
            // make building widget
            RemoveChild(building_widget_);
            building_widget_ = std::make_shared<BuildingWidget>(200, 200, 400, 400, selected_tile_ptr_->building_ptr_);
            AddChild(building_widget_);
            building_widget_->MakeInvisible();

            // make building widget visibility button
            RemoveChild(button_building_widget_);
            button_building_widget_ = std::make_shared<Button>(50, 400, "Building Widget");
            AddChild(button_building_widget_);
            button_building_widget_->SetOnClickCallback([this](){
                building_widget_->ToggleVisibility();
                return true;
            });
        } else {
            building_widget_ = nullptr;
            button_building_widget_ = nullptr;
        }
    } 

    // add children here?
}

// void SelectedStatus::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const {
//     // reset children
//     // children_.clear();
//     // AddChild(border_);
//     // AddChild(text_);
//     // AddChild(button_active_task_);
// }