#include "widgets.hpp"

SelectedStatus::SelectedStatus(double x, double y, double w, double h, MapPtr map_ref, TaskManagerPtr task_mgr_ptr): Widget(x, y, w, h)
{
    map_ref_ = map_ref;
    task_manager_ptr_ = task_mgr_ptr;

    // border rectangle
    border_ = std::make_shared<RectWidget>(0, 0, w, h);
    AddChild(border_);

    text_ = std::make_shared<TextBox>(0, 0, "Status: ");
    AddChild(text_);


}
void SelectedStatus::onUpdate(double dt, double x, double y){
    // check if selected unit changed. If so, trigger a redraw
    auto unit = map_ref_->GetFirstSelectedObject();
    if (unit != selected_unit_ptr_){
        selected_unit_ptr_ = unit;
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
            str += "\nInventory: ";
            for (auto& item : worker->GetInventoryMap()){
                str += to_full_string(item.first);
                str += ": ";
                str += std::to_string(item.second->GetAmount());
                str += ", ";
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
            str += "\nNeeds: ";
            for (auto& need : worker->needs_map_){
                str += to_string(need.first);
                str += ": ";
                str += std::to_string(need.second.val_);
                str += ", ";
            }


        } else if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::GENERIC){
            str += "Generic";
        }
        // add position
        str += "\nPos: " + std::to_string(dynamic_object_ptr->footprint_.x_);
        str += ", ";
        str += std::to_string(dynamic_object_ptr->footprint_.y_);




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
        for (auto& resource : map_ref_->selected_tile_ptr_->resource_map_){
            str += to_full_string(resource.second->resource_type_);
            str += ": ";
            str += std::to_string(resource.second->GetTotal());
            str += ", ";
        }
        // add building
        if (map_ref_->selected_tile_ptr_->building_type_ != BuildingTypes::NONE){
            str += "\nBuilding: ";
            str += to_full_string(map_ref_->selected_tile_ptr_->building_type_);
            str += "\nBuilding Status: " + to_full_string(map_ref_->selected_tile_ptr_->building_ptr_->GetStatus());
            str += "\nInventory: ";
            for (auto& item : map_ref_->selected_tile_ptr_->building_ptr_->inventory_map_){
                str += to_full_string(item.first);
                str += ": ";
                str += std::to_string(item.second);
                str += ", ";
            }
            // switch over building status
            switch (map_ref_->selected_tile_ptr_->building_ptr_->GetStatus()){
                case BuildingStatus::PRECONSTRUCTION:
                    // iterate through item_reqs_map
                    str += "\nItem Reqs: ";
                    for (auto& item : map_ref_->selected_tile_ptr_->building_ptr_->item_reqs_map_){
                        str += to_full_string(item.first);
                        str += ": ";
                        str += std::to_string(item.second);
                        str += ", ";
                    }
                    break;
                case BuildingStatus::CONSTRUCTION:
                    str += "\nConstruction Effort: " + std::to_string(map_ref_->selected_tile_ptr_->building_ptr_->effort_val_);
                    str += " / " + std::to_string(map_ref_->selected_tile_ptr_->building_ptr_->construction_effort_req_);
                    break;
                case BuildingStatus::READY:
                    str += "\n Recipe " + to_full_string(map_ref_->selected_tile_ptr_->building_ptr_->active_recipe_) + " Reqs: ";
                    for (auto& item : map_ref_->selected_tile_ptr_->building_ptr_->GetRecipe(map_ref_->selected_tile_ptr_->building_ptr_->active_recipe_)->inputs_){
                        str += to_full_string(item.first);
                        str += ": ";
                        str += std::to_string(item.second);
                        str += ", ";
                    }
                    break;
                case BuildingStatus::OPERATING:
                    str += "\n Recipe Progress: " + std::to_string(map_ref_->selected_tile_ptr_->building_ptr_->effort_val_);
                    str += " / " + std::to_string(map_ref_->selected_tile_ptr_->building_ptr_->GetRecipe(map_ref_->selected_tile_ptr_->building_ptr_->active_recipe_)->effort_req_);
                    break;
            }

        }
    }
    text_ = std::make_shared<TextBox>(30, 30, str);
    
    // reset children
    children_.clear();
    AddChild(border_);
    AddChild(text_);
    AddChild(button_active_task_);
    AddChild(tasks_list_);
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
            tasks_list_ = std::make_shared<Dropdown>(200, 600, task_manager_ptr_->GetTaskNames());
            tasks_list_->SetOnClickCallback([this, worker](){
                // cb: set this worker's task to the selected idx from the dropdown menu
                worker->SetTask(task_manager_ptr_->GetTask(tasks_list_->GetClickedIdx()));
                reDraw();
            });
            tasks_list_->MakeInvisible();
            
            button_active_task_ = std::make_shared<Button>(50, 600, "Active Task: \n" + task_str);
            button_active_task_->SetOnClickCallback([this](){
                tasks_list_->ToggleVisibility();
            });

        }
    }

    // tile GUI
    if (map_ref_->selected_tile_ptr_ != nullptr){
        if (map_ref_->selected_tile_ptr_->building_type_ != BuildingTypes::NONE){
            // make building widget
            building_widget_ = std::make_shared<BuildingWidget>(200, 200, 400, 400, map_ref_->selected_tile_ptr_->building_ptr_);
            AddChild(building_widget_);
            building_widget_->MakeInvisible();

            // make building widget visibility button
            auto building_widget_button = std::make_shared<Button>(50, 400, "Building Widget");
            building_widget_button->SetOnClickCallback([this](){
                building_widget_->ToggleVisibility();
            });

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