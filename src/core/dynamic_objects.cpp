#include "dynamic_objects.hpp"


const std::string to_string(WorkerStates p){
  switch(p)
  {
    case WorkerStates::DEAD: return "dead";
    case WorkerStates::IDLE: return "idle";
    case WorkerStates::EXECUTINGTASK: return "executing task";
    case WorkerStates::MOVING: return "moving";
    case WorkerStates::GATHERIDLE: return "gatheridle";
    case WorkerStates::GATHERING: return "gathering";
    case WorkerStates::CONSTRUCTING: return "constructing";

  }
  return ""; // or an empty string
}
const std::string to_full_string(WorkerStates p){return to_string(p);}
const std::string to_string(NeedsTypes p){
  switch(p)
  {
    case NeedsTypes::FOOD: return "food";
    case NeedsTypes::SLEEP: return "sleep";
    case NeedsTypes::WATER: return "water";

  }
  return ""; // or an empty string
}


/////////////////////////////////////// DynamicObject ///////////////////////////////////////
void DynamicObject::update(double dt){
    // update the dynamic object
    double dx = 0;
    double dy = 0;
    // if over the bitwise actions
    if (dynamic_object_actions_.HasFlag((EFlagValue)DynamicObjectActions::MOVE_UP)){
        dy += speed_;
    }
    if (dynamic_object_actions_.HasFlag((EFlagValue)DynamicObjectActions::MOVE_DOWN)){
        dy -= speed_;
    }
    if (dynamic_object_actions_.HasFlag((EFlagValue)DynamicObjectActions::MOVE_LEFT)){
        dx -= speed_;
    }
    if (dynamic_object_actions_.HasFlag((EFlagValue)DynamicObjectActions::MOVE_RIGHT)){
        dx += speed_;
    }
    // diagonal movement check
    if ((dynamic_object_actions_.HasFlag((EFlagValue)DynamicObjectActions::MOVE_UP) || dynamic_object_actions_.HasFlag((EFlagValue)DynamicObjectActions::MOVE_DOWN)) && (dynamic_object_actions_.HasFlag((EFlagValue)DynamicObjectActions::MOVE_LEFT) || dynamic_object_actions_.HasFlag((EFlagValue)DynamicObjectActions::MOVE_RIGHT))){
        dx /= 1.41;
        dy /= 1.41;
    }
    // update x & y
    MoveX(dx * dt);
    MoveY(dy * dt);
}
void DynamicObject::SetX(double x){
    footprint_.x_ = x;
}
void DynamicObject::SetY(double y){
    footprint_.y_ = y;
}
void DynamicObject::MoveX(double dx){
    footprint_.x_ += dx;
}
void DynamicObject::MoveY(double dy){
    footprint_.y_ += dy;
}
void DynamicObject::SetSpeed(double speed){
    speed_ = speed;
}
bool DynamicObject::PointInFootprint(double x, double y){
    // check if a point is in the footprint
    if (x >= footprint_.x_ && x <= footprint_.x_ + footprint_.width_ && y >= footprint_.y_ && y <= footprint_.y_ + footprint_.height_){
        return true;
    } else {
        return false;
    }
}
/////////////////////////////////////// End DynamicObject ///////////////////////////////////////

/////////////////////////////////////// Worker ///////////////////////////////////////
Worker::Worker(){
    SetDynamicObjectType(DynamicObjectTypes::WORKER);

    // make skills
    // TODO: iterate over enum class properly
    for (int i=0; i<(int)SkillTypes::END_OF_ENUM_VAL; i++){
        skill_map_[(SkillTypes)i] = Skill((SkillTypes)i);
    }
}
void Worker::Die(){
    dynamic_object_actions_.ClearFlag();
    SetState(WorkerStates::DEAD);
}
void Worker::update(double dt){
    // check if dead
    if (worker_state_ == WorkerStates::DEAD){
        return;
    }

    // update the worker (this is where movement actually happens)
    inherited::update(dt);

    // update needs
    for (auto& need : needs_map_){
        need.second.update(dt);
        
        // need-specific switch case
        switch (need.first) {
            case NeedsTypes::FOOD:
                if (need.second.IsZero()){
                    // ded
                    Die();
                }
        }
    }

    // AI
    AI(dt);
}
void Worker::AI(double dt){
    // AI for the worker
    // first check if any needs are critical
    for (auto& need : needs_map_){
        if (need.second.IsCritical()){
            // TODO
        }
    }
    // next check if any needs are auto-fulfillable
    for (auto& need : needs_map_){
        if (need.second.IsAutoFulfillable()){
            switch (need.second.need_type_){
                case NeedsTypes::FOOD:
                    Eat();
                    break;
                case NeedsTypes::SLEEP:
                    // TODO
                    break;
                case NeedsTypes::WATER:
                    // TODO
                    break;
            }
        }
    }

    // clear actions
	// search terms: fsm, finite state machine
    switch (worker_state_){
        case WorkerStates::DEAD:
            // do nothing, it's dead. Shouldn't ever get here though.
            break;
        case WorkerStates::IDLE:
            // Attempt to infer an action based on the attention
            Reset();
            InferAction(dt);
            break;
		case WorkerStates::INFERRING:
			InferAction(dt);
			break;
        case WorkerStates::EXECUTINGTASK:
            ExecuteTask(dt);
            break;
        case WorkerStates::MOVING:
            // move towards goal
            MoveTowardsGoal();
            break;
        case WorkerStates::GATHERIDLE:
            // gather resources
            Gather(dt);
            break;
        case WorkerStates::GATHERING:
            // gather resources
            Gather(dt);
            break;
        case WorkerStates::CONSTRUCTINGIDLE:
            // construct buildings
            Construct(dt);
            break;
        case WorkerStates::CONSTRUCTING:
            // constructing
            Construct(dt);
            break;
        case WorkerStates::CRAFTINGIDLE:
            // crafting
            Craft(dt);
            break;
        case WorkerStates::CRAFTING:
            // crafting
            Craft(dt);
            break;
    }
}
void Worker::Reset(){
	ResetDynamicObjectActions();
	ClearAttention();
	SetState(WorkerStates::IDLE);
}
void Worker::ClearAttention(){
	// clear the attention
	selected_building_ptr_ = nullptr;
	selected_resource_ptr_ = nullptr;
}
void Worker::SetGoal(double x, double y){
    // set the goal for the worker
    goal_.x_ = x;
    goal_.y_ = y;
}
Rect<double> Worker::GetImmediateSurroundingsRect(){
    // get the immediate surroundings rect
    auto local_rect = immediate_surroundings_.GetLocalRect();
	Rect<double> global_rect = local_rect;
	global_rect.x_ = local_rect.x_ + GetCenter().x_;
	global_rect.y_ = local_rect.y_ + GetCenter().y_;
	return global_rect;
}
Rect<double> Worker::GetNearbySurroundingsRect(){
    // get the nearby surroundings rect
    auto local_rect = nearby_surroundings_.GetLocalRect();
	Rect<double> global_rect = local_rect;
	global_rect.x_ = local_rect.x_ + GetCenter().x_;
	global_rect.y_ = local_rect.y_ + GetCenter().y_;
	return global_rect;
}
void Worker::SetState(WorkerStates worker_state) 
{
    // this is hacky, but only perform leaving state checks if not switching to EXECUTINGTASK
    // this won't work when the user right-clicks and then user presses e....., although maybe that's ok
    if (worker_state != WorkerStates::EXECUTINGTASK){
        // Check if leaving moving
        if (worker_state_ == WorkerStates::MOVING){
            // clear actions
            ResetDynamicObjectActions();
        }
    }

    // update state
    worker_state_ = worker_state;
}

/////////////////////////////////////// Worker Action Primitives ///////////////////////////////////////
void Worker::SelectBuilding(std::shared_ptr<Building> building_ptr){
    // select a building
    // auto success
    SetState(WorkerStates::IDLE);
    selected_building_ptr_ = building_ptr;
}
DefaultActionFcn Worker::SelectClosestBuilding(){
    // iterate over buildings in nearby surroundings
    double min_dist = 999.0;
    bool found = false;
    for (auto& building : nearby_surroundings_.buildings_){
        auto dist = eucl_dist<double>(GetCenter(), building->center_);
        if (dist < min_dist){
            min_dist = dist;
            found = true;
            SelectBuilding(building);
        }
    }
    if (found){
        SetState(WorkerStates::IDLE);
    }
    return 0;
}
DefaultActionFcn Worker::SetGoalToSelectedBuilding(){
    // set the goal to the selected building
    if (selected_building_ptr_ == nullptr){
        return 0;
    }
    // auto success
    SetState(WorkerStates::IDLE);
    goal_.x_ = selected_building_ptr_->center_.x_;
    goal_.y_ = selected_building_ptr_->center_.y_;
    return 0;
}

DefaultActionFcn Worker::MoveTowardsGoal(){
    SetState(WorkerStates::MOVING);
    // TODO: base off center of footprint
    bool done_x = false;
    bool done_y = false;

    // simple directional movement based off goal_
    if (abs(GetCenter().x_ - goal_.x_) < goal_dist_threshold_){
        done_x = true;
    } else {
        if (GetCenter().x_ < goal_.x_){
            dynamic_object_actions_.SetFlag((EFlagValue)DynamicObjectActions::MOVE_RIGHT);
        } else {
            dynamic_object_actions_.SetFlag((EFlagValue)DynamicObjectActions::MOVE_LEFT);
        }
    }

    if (abs(GetCenter().y_ - goal_.y_) < goal_dist_threshold_){
        done_y = true;
    } else {
        if (GetCenter().y_ < goal_.y_){
            dynamic_object_actions_.SetFlag((EFlagValue)DynamicObjectActions::MOVE_UP);
        } else {
            dynamic_object_actions_.SetFlag((EFlagValue)DynamicObjectActions::MOVE_DOWN);
        }
    }

    if (done_x && done_y){
        SetState(WorkerStates::IDLE);
    }
    return 0;
}
void Worker::Gather(double dt){
    // gather resources

    // if not gathering, decide which resource to gather and set action_time_ to zero
    // get first resource in immediatesurroundings
    if (worker_state_ == WorkerStates::GATHERIDLE){
        if (immediate_surroundings_.resources_.size() > 0){
            selected_resource_ptr_ = immediate_surroundings_.resources_[0];
            action_time_ = 0.0;
            SetState(WorkerStates::GATHERING);
        } else {
            // nothing close enough to gather
            std::cout << "Nothing close enough to gather" << std::endl;
            SetState(WorkerStates::IDLE);
            return;
        }
    }

    // update gather time
    action_time_ += dt;

    // check if gather time is greater than resource time_to_gather_
    if (action_time_ > selected_resource_ptr_->time_to_gather_){
        // convert resource to item type
        ResourceTypes resource_type = selected_resource_ptr_->resource_type_;
        ItemTypes item_type = (ItemTypes)resource_type;

        auto amount = selected_resource_ptr_->Extract();

        // add resource to inventory
        AddToInventory(item_type, amount);
        
        // set gather time to zero
        action_time_ = 0.0;

        // check if resource is exhausted
        if (selected_resource_ptr_->GetTotal() <= 0){
            // set state to GATHERIDLE
            SetState(WorkerStates::GATHERIDLE);
        }
    }
}
void Worker::Construct(double dt){
    // construct buildings

    // if not constructing, decide which building to construct and set action_time_ to zero
    // get first building in immediatesurroundings
    if (worker_state_ == WorkerStates::CONSTRUCTINGIDLE){
        // iterate over buildings in immediate surroundings
        for (auto& building : immediate_surroundings_.buildings_){
            // check if building is ready
            if (building->building_status_ == BuildingStatus::PRECONSTRUCTION || building->building_status_ == BuildingStatus::CONSTRUCTION){
                // set building to construct
                selected_building_ptr_ = building;
                action_time_ = 0.0;
                SetState(WorkerStates::CONSTRUCTING);
                break;
            }
        }

        if (worker_state_ == WorkerStates::CONSTRUCTINGIDLE){
            // nothing close enough to construct
            std::cout << "Nothing close enough to construct" << std::endl;
            SetState(WorkerStates::IDLE);
            return;
        }
    }
    EngageWithBuilding(selected_building_ptr_);
}
void Worker::EngageWithBuilding(BuildingPtr building_ptr){
    // switch case on building status
    switch (building_ptr->building_status_){
        case BuildingStatus::PRECONSTRUCTION:
            // transfer required items from inventory to building
            for (auto& item : building_ptr->item_reqs_map_){
                // check if item in inventory
                if (inventory_.find(item.first)){
                    // amount to transfer
                    auto amount = std::min(item.second, inventory_.GetItem(item.first)->GetAmount());

                    // remove from inventory
                    inventory_.GetItem(item.first)->RemoveAmount(amount);
                    // add to building
                    selected_building_ptr_->AddToInventory(item.first, item.second);
                }
            }
            break;
        case BuildingStatus::CONSTRUCTION:
            // update construction effort
            building_ptr->effort_val_ += skill_map_[SkillTypes::CONSTRUCTION].CalcEffortUnits();
            break;
        case BuildingStatus::OPERATING:
            // add to effort val
            selected_building_ptr_->effort_val_ += skill_map_[SkillTypes::CRAFTING].CalcEffortUnits();
        default:
            SetState(WorkerStates::CONSTRUCTINGIDLE);
            break;
    }
}
void Worker::Craft(double dt){
    // craft items

    // TODO: check if worker is close enough to selected_building_ptr_

    // TODO: move worker towards selected_building_ptr_ if not close enough
    
    // check if selected building is operating
    if (selected_building_ptr_->building_status_ != BuildingStatus::OPERATING){
        // set state to CRAFTINGIDLE
        SetState(WorkerStates::CRAFTINGIDLE);
        return;
    } else {
        // add to effort val
        selected_building_ptr_->effort_val_ += skill_map_[SkillTypes::CRAFTING].CalcEffortUnits();
    }
}
void Worker::TransferItem(ItemTypes item_type, double amount_request, std::shared_ptr<Building> building_ptr){
    // transfer item to building
    double amount_to_transfer = 0.0;
    // check if item in inventory
    if (inventory_.find(item_type)){
        // check if amount in inventory is greater than required
        amount_to_transfer = std::min(amount_request, inventory_.GetItem(item_type)->GetAmount());
    } else {
        return;
    }
    // add to building
    building_ptr->AddToInventory(item_type, amount_to_transfer);
    // remove from inventory
    inventory_.GetItem(item_type)->RemoveAmount(amount_to_transfer);
}
void Worker::TransferInventory(){
    // Smart transfer inventory to building based on context
    // check if building is nullptr

    if (selected_building_ptr_ == nullptr){
        return;
    }

    // check if selected building has a recipe
    if (selected_building_ptr_->active_recipe_idx_ != -1){
        // iterate over inputs
        for (auto& input : selected_building_ptr_->GetRecipe(selected_building_ptr_->active_recipe_idx_)->inputs_){
            // transfer item
            TransferItem(input.first, input.second, selected_building_ptr_);
        }
    } else {
        // iterate over inventory
        for (auto& item : inventory_.GetItemMap()){
            // transfer item
            TransferItem(item.first, item.second->GetAmount(), selected_building_ptr_);
        }
    }

    SetState(WorkerStates::IDLE);
}
void Worker::TakeItem(ItemTypes item_type, double amount_request, std::shared_ptr<Building> building_ptr){
    // transfer item to building
    double amount_to_transfer = 0.0;
    // check if item in building
    if (building_ptr->inventory_map_.find(item_type) != building_ptr->inventory_map_.end()){
        // check if amount in building is greater than required
        amount_to_transfer = std::min(amount_request, building_ptr->inventory_map_[item_type]);
    } else {
        return;
    }
    // add to inventory
    AddToInventory(item_type, amount_to_transfer);
    // remove from building
    building_ptr->inventory_map_[item_type] -= amount_to_transfer;
}
DefaultActionFcn Worker::TakeInventory(){
    if (selected_building_ptr_ == nullptr){
        return 0;
    }
    
    // get items to not transfer from the building's recipe
    std::vector<ItemTypes> items_to_avoid;
    if (selected_building_ptr_->active_recipe_idx_ != -1){
        for (auto& input : selected_building_ptr_->GetRecipe(selected_building_ptr_->active_recipe_idx_)->inputs_){
            items_to_avoid.push_back(input.first);
        }
    }

    // iterate over inventory
    for (auto& item : selected_building_ptr_->GetItemMap()){
        // check if item is in items_to_avoid
        if (std::find(items_to_avoid.begin(), items_to_avoid.end(), item.first) != items_to_avoid.end()){
            continue;
        }
        // transfer item
        TakeItem(item.first, item.second, selected_building_ptr_);
    }

    SetState(WorkerStates::IDLE);
    return 0;
}
DefaultActionFcn Worker::Eat(){
    // iterate over inventory, find first item with item_category_ == ItemCategories::FOOD, then remove it from inventory and increase need value
    for (auto& item : inventory_.GetItemMap()){
        if (item.second->IsItemCategory(ItemCategories::FOOD)){
            auto food = std::static_pointer_cast<FoodItem>(item.second);
            // get amount to eat, protection against negative value
            double amount_to_eat = std::min(1.0, item.second->GetAmount());
            needs_map_[NeedsTypes::FOOD].val_ += amount_to_eat * food->nutrient_amount_;
            // subtract from inventory
            item.second->RemoveAmount(amount_to_eat);
            return 0;
        }
    }
    return 0;
}
void Worker::InferActionWide(double dt){
    // infer simple actions based off NearbySurroundings
    
    // buildings have priority 
    // iterate through nearby surroundings
	double dist = 99;
	BuildingPtr new_building_ptr = nullptr;

    for (auto& building : nearby_surroundings_.buildings_){
        // check if building is ready
        if (building->building_status_ == BuildingStatus::PRECONSTRUCTION || building->building_status_ == BuildingStatus::CONSTRUCTION){
			// dist
			auto new_dist = eucl_dist<double>(GetCenter(), building->center_);
			if (new_dist < dist){
				dist = new_dist;
				new_building_ptr = building;
			}
        }
    }
	if (new_building_ptr != nullptr){
		// set building to construct
		selected_building_ptr_ = new_building_ptr;
		SetState(WorkerStates::INFERRING);
	}
}
void Worker::InferAction(double dt){
    // Infer an action based on the attention (selected_building_ptr_)
    if (HasAttention()){
		// check if close enough for interaction
		if (eucl_dist<double>(GetCenter(), selected_building_ptr_->center_) < interaction_dist_){
			SetState(WorkerStates::CONSTRUCTING);
			Construct(dt);
		} else {
			// set goal to selected building
			SetGoalToSelectedBuilding();
			SetState(WorkerStates::MOVING);
		}

    } else {
        InferActionWide(dt);
    }
}
void Worker::ExecuteTask(double dt){
    // Execute a task
    // TODO: move this to dynamic object class
    if (task_ptr_ == nullptr){
        std::cout << "No active task." << std::endl;
        SetState(WorkerStates::IDLE);
        return;
    }
    
    ResetDynamicObjectActions();
            
    auto action_type = task_ptr_->GetActiveActionType(); // recurse get active action type
    action_primitive_map_[action_type]();

    SuccessFcn success_fcn = [this](){
            if (CheckState(WorkerStates::IDLE)){
                return true;
            } else {
                return false;
            }
        };

    auto is_complete = success_fcn();
    task_ptr_->update(is_complete); // recurse update off completion of action

    // Revert to executing task. Bit of a hack.
    SetState(WorkerStates::EXECUTINGTASK);
}

/////////////////////////////////////// End Worker ///////////////////////////////////////