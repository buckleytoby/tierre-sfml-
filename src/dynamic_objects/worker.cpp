#include "dynamic_objects.hpp"

/////////////////////////////////////// Worker ///////////////////////////////////////

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
    case WorkerStates::CONSTRUCTINGIDLE: return "constructingidle";
    case WorkerStates::CRAFTING: return "crafting";
    case WorkerStates::CRAFTINGIDLE: return "craftingidle";
    case WorkerStates::INFERRING: return "inferring";

  }
  return ""; // or an empty string
}
const std::string to_full_string(WorkerStates p){return to_string(p);}

std::string to_string(WorkerStats p){
    switch(p)
    {
        case WorkerStats::CURRCARRY: return "Current Carry";
        case WorkerStats::MAXCARRY: return "Max Carry";
    
    }
    return ""; // or an empty string
    
}

Worker::Worker(): DynamicObject("worker"){
    // ctor
    SetHandleInputCb(std::bind(&Worker::onHandleInput, this, std::placeholders::_1));
    //
    SetDynamicObjectType(DynamicObjectTypes::WORKER);
    is_male_ = std::rand() > RAND_MAX / 2 ? true : false;
    if (is_male_)
        LoadArt("worker_m");
    else
        LoadArt("worker_f");
    // set height and width
    bounds_m_.width = 1.0; // in meters
    bounds_m_.height = 1.5; // in meters

    // set maps
    needs_map_ = {
            {NeedsTypes::FOOD, FoodNeed()}
        };
    attribute_map_ = {
            {WorkerAttributeTypes::STRENGTH, Attribute(WorkerAttributeTypes::STRENGTH, 5.0)},
            {WorkerAttributeTypes::SPEED, Attribute(WorkerAttributeTypes::SPEED, 1.0)}
        };
    stats_map_ = {
            {WorkerStats::CURRCARRY, 0.0},
            {WorkerStats::MAXCARRY, 10.0}
        };

    // make skills
    // TODO: iterate over enum class properly
    for (int i=0; i<(int)SkillTypes::END_OF_ENUM_VAL; i++){
        skill_map_[(SkillTypes)i] = SkillFactory::CreateSkill((SkillTypes)i);
    }

    // 
    if (DEBUG){
        // give worker super stats
        skill_map_[SkillTypes::CONSTRUCTION]->SetLevel(100.0);
        skill_map_[SkillTypes::GATHERING]->SetLevel(100.0);
        skill_map_[SkillTypes::CRAFTING]->SetLevel(100.0);
    }

    // set state callback map
    m_state_callback_map[WorkerStates::DEAD] = [](double dt){return true;}; // do nothing, it's dead. Shouldn't ever get here though.
    m_state_callback_map[WorkerStates::IDLE] = [this](double dt){return IdleCB(dt);};
    m_state_callback_map[WorkerStates::INFERRING] = [this](double dt){return InferAction(dt);};
    m_state_callback_map[WorkerStates::EXECUTINGTASK] = [this](double dt){return ExecutingTaskCB(dt);};
    m_state_callback_map[WorkerStates::MOVING] = [this](double dt){return MovingCB(dt);};
    m_state_callback_map[WorkerStates::GATHERIDLE] = [this](double dt){return GatherCB(dt);};
    m_state_callback_map[WorkerStates::GATHERING] = [this](double dt){return GatherCB(dt);};
    m_state_callback_map[WorkerStates::CONSTRUCTINGIDLE] = [this](double dt){return ConstructingCB(dt);};
    m_state_callback_map[WorkerStates::CONSTRUCTING] = [this](double dt){return ConstructingCB(dt);};
    m_state_callback_map[WorkerStates::CRAFTINGIDLE] = [this](double dt){return CraftingIdleCB(dt);};
    m_state_callback_map[WorkerStates::CRAFTING] = [this](double dt){return CraftingCB(dt);};
}

bool Worker::ExecutingTaskCB(double dt)
{
    ExecuteTask(dt);

    return true;
}

bool Worker::MovingCB(double dt)
{
    actionMoveTowardsGoal();

    return true;
}

bool Worker::GatherCB(double dt)
{
    Gather(dt);

    return true;
}

bool Worker::ConstructingCB(double dt)
{
    Construct(dt);

    return true;
}

bool Worker::CraftingIdleCB(double dt)
{
    CraftingIdle(dt);

    return true;
}

bool Worker::CraftingCB(double dt)
{
    Craft(dt);

    return true;
}

bool Worker::CheckState(WorkerStates state){
    // two modes, either executing task or not
    if (worker_state_ == WorkerStates::EXECUTINGTASK){
        return task_worker_state_ == state;
    }
    // if not executing task, check state like normal
    else {
        return worker_state_ == state;
    }
}
void Worker::Die(){
    dynamic_object_actions_.ClearFlag();
    SetState(WorkerStates::DEAD);
}

/// @brief Updates that don't depend on time. Used to "make decisions", instant actions, etc.
bool Worker::InstantUpdate()
{
    // check if dead
    if (worker_state_ == WorkerStates::DEAD){
        // TODO: add event to remove this game object from the update list?

        return false;
    }

    //// process "thoughts" queue
    // process desired state
    if (!desired_states_.empty())
    {
        // whether to pop the next desired state
        bool can_pop = false;

        //// PoC, hard coded for now
        switch (desired_states_.front())
        {
            case (WorkerStates::MOVING):
            {
                // if we want to move, then pop
                can_pop = true;
                break;
            }

            case (WorkerStates::GATHERING):
            {
                // check if we're allowed to transition
                if (CanGatherNow())
                {
                    can_pop = true;
                }
                // else, move towards attention without reseting desired state
                else
                {
                    // set goal to selected resource
                    SetGoalToSelectedResource(); // TODO: change to SetGoalToAttention
                    SetState(WorkerStates::MOVING);
                }
                break;
            }
        }

        // if we can pop, then set the state
        if (can_pop)
        {
            // set state
            SetState(desired_states_.front());

            // remove front state
            desired_states_.pop();
        }

    }

    return true;
}

bool Worker::UpdateNeeds(double dt)
{
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

    return true;
}

bool Worker::TemporalUpdate(double dt)
{
    // update the worker (this is where movement actually happens)
    // call the inherited update
    inherited::onUpdate(dt);

    // update our needs
    UpdateNeeds(dt);

    // check triggers
    if (must_update_stats_){
        UpdateStats();
        must_update_stats_ = false;
    }

    // AI
    AI(dt);

    return true;
}

/// @brief virtual onUpdate fcn. Class specific updates
/// @param dt 
void Worker::onUpdate(double dt){
    // instant updates
    if (!InstantUpdate())
        return;

    // temporal updates
    if (!TemporalUpdate(dt))
        return;
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
    ResetDynamicObjectActions(); // here?
    
    // Call the current state callback
    m_state_callback_map[worker_state_](dt);

    // Update our Plan
    /*
    update
    update
    update
    */

}
void Worker::Reset(){
	ResetDynamicObjectActions();
	ClearAttention();
	SetState(WorkerStates::IDLE);
}
Rect<double> Worker::GetImmediateSurroundingsRect(){
    // get the immediate surroundings rect
    auto local_rect = immediate_surroundings_->GetLocalRect();
	Rect<double> global_rect = local_rect;
	global_rect.x_ = local_rect.x_ + GetCenter().x_;
	global_rect.y_ = local_rect.y_ + GetCenter().y_;
	return global_rect;
}
Rect<double> Worker::GetNearbySurroundingsRect(){
    // get the nearby surroundings rect
    auto local_rect = nearby_surroundings_->GetLocalRect();
	Rect<double> global_rect = local_rect;
	global_rect.x_ = local_rect.x_ + GetCenter().x_;
	global_rect.y_ = local_rect.y_ + GetCenter().y_;
	return global_rect;
}
void Worker::SetState(WorkerStates worker_state) 
{
    // two states, either executing task or not
    if (worker_state_ == WorkerStates::EXECUTINGTASK){
        task_worker_state_ = worker_state;
    }
    // update state
    else {
        // check if entering execute task
        if (worker_state == WorkerStates::EXECUTINGTASK){
            task_ptr_->onFirst();
        }
        worker_state_ = worker_state;
    }
}
void Worker::ClearState(){
    // clear the state and the task state
    worker_state_ = WorkerStates::IDLE;
    task_worker_state_ = WorkerStates::IDLE;
}

/////////////////////////////////////// Worker Action Primitives ///////////////////////////////////////
bool Worker::SelectClosestBuilding(){
    // iterate over buildings in nearby surroundings
    double min_dist = 999.0;
    bool found = false;
    for (auto& building : nearby_surroundings_->buildings_){
        auto dist = eucl_dist<double>(GetCenter(), building->center_);
        if (dist < min_dist){
            min_dist = dist;
            found = true;
            SelectBuilding(building);
        }
    }
    if (found){
        return true;
    } else {
        return false;
    }
}

bool Worker::MoveTowardsGoal(){
    SetState(WorkerStates::MOVING);
    // TODO: base off center of bounds
    bool done_x = false;
    bool done_y = false;

    // simple directional movement based off goal_
    if (abs(GetCenter().x_ - goal_.x_) < goal_dist_threshold_){
        done_x = true;
    } else {
        if (GetCenter().x_ < goal_.x_){
            dynamic_object_actions_.SetFlag((BitEnum)DynamicObjectActions::MOVE_RIGHT);
        } else {
            dynamic_object_actions_.SetFlag((BitEnum)DynamicObjectActions::MOVE_LEFT);
        }
    }

    if (abs(GetCenter().y_ - goal_.y_) < goal_dist_threshold_){
        done_y = true;
    } else {
        if (GetCenter().y_ < goal_.y_){
            dynamic_object_actions_.SetFlag((BitEnum)DynamicObjectActions::MOVE_UP);
        } else {
            dynamic_object_actions_.SetFlag((BitEnum)DynamicObjectActions::MOVE_DOWN);
        }
    }

    if (done_x && done_y){
        return true;
    } else
        return false;
}
bool Worker::CheckandMoveTowardsGoal(){
    // check close enough
    if (IsCloseEnough(selected_gameobject_))
        return true;
    else
        return MoveTowardsGoal();
}
bool Worker::InventoryHasRoom(ResourcePtr ptr){
    return (stats_map_[WorkerStats::CURRCARRY]+ptr->GetYieldPerGather()) < stats_map_[WorkerStats::MAXCARRY];
}

/// @brief True if it is possible for the resource to be gathered from, doesn't check distance
/// @param ptr 
/// @return 
bool Worker::CanGather(ResourcePtr ptr){
    if (ptr == nullptr)
        return false;
    // check if selected resource is gatherable and has resources and we have inventory space
    return ptr->CanGather() && InventoryHasRoom(ptr);
}

bool Worker::CanGather()
{
    return CanGather(selected_resource_ptr_);
}

bool Worker::CanGatherNow()
{
    return CanGather() && IsCloseEnough(selected_resource_ptr_);
}

bool Worker::CanMove()
{

    return true;
}

void Worker::BeginGather(){BeginGather(0.0);}
void Worker::BeginGather(double dt){
    // begin gather
    if (CanGather(selected_resource_ptr_)){
        // if entering gathering, then set action_time_ to zero
        if (!CheckState(WorkerStates::GATHERING))
            action_time_ = 0.0;
            SetState(WorkerStates::GATHERING);
    } else {
        SetState(WorkerStates::GATHERIDLE);
        return;
    }
}
bool Worker::FindGatherResource(){
    // if not gathering, decide which resource to gather and set action_time_ to zero
    // get first resource in immediatesurroundings
    if (CheckState(WorkerStates::GATHERIDLE)){
        AttentionClearResource();
        // check if we have an attention rect
        if (AttentionHasRect()){
            XY<int> center = XY<int>(GetCenter().x_, GetCenter().y_);
            for (auto coord: GetAttentionRect()->GetCoordsByEuclDist(center)){
                auto resource = nearby_surroundings_->GetResource(coord.x_, coord.y_);
                if (CanGather(resource)){
                    // set resource to gather
                    SetAttention(resource);
                    SetGoalToSelectedResource();
                    break;
                }
            }
        }
    }

    // final sanity check
    if (!CanGather(selected_resource_ptr_)){
        return false;
    }
    return true;
}
void Worker::GatherArea(double dt){
    // gather resources in area
    ExecuteCoreAction(dt, 
        [this](){return FindGatherResource();}, 
        [this](double dt){GatherCore(dt);}, 
        [this](){
            if (!CanGather(selected_resource_ptr_)){
                SetState(WorkerStates::GATHERIDLE);
                return false;
            } else {
                return true;
            }
            });
}
void Worker::Gather(double dt){
    ExecuteCoreAction(dt, 
        [this](){return FindGatherResource();}, 
        [this](double dt){GatherCore(dt);}, 
        [this](){
            if (!CanGather(selected_resource_ptr_)){
                SetState(WorkerStates::IDLE);
                return false;
            } else {
                return true;
            }
            });
}
void Worker::GatherCore(double dt){
    // doesn't check validity
    // update gather time
    action_time_ += skill_map_[SkillTypes::GATHERING]->CalcEffortUnits() * dt;

    // check if gather time is greater than resource time_to_gather_
    if (action_time_ > selected_resource_ptr_->time_to_gather_){
        // convert resource to item type, slightly hacky
        ResourceTypes resource_type = selected_resource_ptr_->resource_type_;
        ItemTypes item_type = (ItemTypes)resource_type;

        auto yield = selected_resource_ptr_->Extract(); // kg

        // add resource to inventory
        AddToInventory(item_type, yield);
        
        // set gather time to zero
        action_time_ = 0.0;
    }
}
void Worker::ExecuteCoreAction(double dt, std::function<bool()> FindFcn, std::function<void(double)> CoreFcn, std::function<bool()> CheckFcn){
    // find a valid game object to interact with
    if (!FindFcn()){
        SetState(WorkerStates::IDLE);
        return;
    }

    // ensure we're close enough
    if (!CheckandMoveTowardsGoal()){
        return;
    }

    // interact with the object
    CoreFcn(dt);
    
    // check if done with this object
    if (!CheckFcn()){
        AttentionClear();
    }
}

bool Worker::AddToInventory(ItemTypes type, double yield){
    // check if there's room
    if (stats_map_[WorkerStats::CURRCARRY] + yield > stats_map_[WorkerStats::MAXCARRY]){
        return false;
    }
    inventory_.AddTo(type, yield);
    must_update_stats_ = true;
    return true;
}
void Worker::BeginConstruct(){
    // begin construct
    SetState(WorkerStates::CONSTRUCTINGIDLE);
}
bool Worker::CanConstruct(BuildingPtr ptr){
    // check if selected building is constructable and we have the required items
    if (ptr == nullptr)
        return false;
    return ptr->CanConstruct();
}
bool Worker::FindConstructBuilding(){
    if (!AttentionHasBuilding()){
        // check if we have an attention rect
        if (AttentionHasRect()){
            XY<int> center = XY<int>(GetCenter().x_, GetCenter().y_);
            for (auto coord: GetAttentionRect()->GetCoordsByEuclDist(center)){
                auto building = nearby_surroundings_->GetBuilding(coord.x_, coord.y_);
                if (CanConstruct(building)){
                    // set resource to gather
                    SetAttention(building);
                    SetGoalToSelectedBuilding();
                    break;
                }
            }
        }

        if (!AttentionHasBuilding()){
            return false;
        }
    }
    return true;
}
void Worker::Construct(double dt){
    ExecuteCoreAction(dt, 
        [this](){return FindConstructBuilding();}, 
        [this](double dt){ConstructCore(selected_building_ptr_, dt);}, 
        [this](){
            if (!CanConstruct(selected_building_ptr_)){
                SetState(WorkerStates::IDLE);
                return false;
            } else {
                return true;
            }
            });
}
void Worker::ConstructCore(BuildingPtr building_ptr, double dt){
    // switch case on building status
    switch (building_ptr->building_status_){
        case BuildingStatus::PRECONSTRUCTION:
            TransferInventory();
            // if (!selected_building_ptr_->CheckItemReqs())
            //     SetState(WorkerStates::IDLE);
            break;
        case BuildingStatus::CONSTRUCTION:
            // update construction effort
            building_ptr->effort_val_ += skill_map_[SkillTypes::CONSTRUCTION]->CalcEffortUnits() * dt;
            break;
        default:
            break;
    }
}
bool Worker::FindCraftBuilding(){
    if (!AttentionHasBuilding()){
        // check if we have an attention rect
        if (AttentionHasRect()){
            XY<int> center = XY<int>(GetCenter().x_, GetCenter().y_);
            for (auto coord: GetAttentionRect()->GetCoordsByEuclDist(center)){
                auto building = nearby_surroundings_->GetBuilding(coord.x_, coord.y_);
                if (CanCraft(building)){
                    // set resource to gather
                    SetAttention(building);
                    SetGoalToSelectedBuilding();
                    break;
                }
            }
        }

        if (!AttentionHasBuilding()){
            return false;
        }
    }
    return true;
}
void Worker::CraftingIdle(double dt){
    if (!AttentionHasBuilding()){
        SetState(WorkerStates::IDLE);
        return;
    }
    if (selected_building_ptr_->building_status_ == BuildingStatus::READY){
        TransferInventory();
    }
}
bool Worker::CanCraft(BuildingPtr ptr){
    // selected building is not null
    if (ptr == nullptr)
        return false;
    // if the building has non-recipe inventory, we're good and can go pick it up
    if (ptr->GetNonRecipeItems().size() > 0){
        return true;
    }

    // if the building's recipe is fulfilled, we're good
    if (ptr->IsRecipeFulfilled()){
        return true;
    }

    // if in READY, I must be able to fulfill the recipe
    if (ptr->building_status_ == BuildingStatus::READY){
        // check if selected building has a recipe
        if (ptr->active_recipe_idx_ != -1){
            return ptr->GetActiveRecipe()->CanFulfill(&inventory_);
        } else {
            return false;
        }
    // if in OPERATING, we're good
    } else if (ptr->building_status_ == BuildingStatus::OPERATING){
        return true;
    } else {
        return false;
    }
}
void Worker::Craft(double dt){
    ExecuteCoreAction(dt, 
        [this](){return selected_building_ptr_ != nullptr;}, 
        [this](double dt){CraftCore(dt);}, 
        [this](){
            if (!CanCraft(selected_building_ptr_)){
                SetState(WorkerStates::IDLE);
                    return false;
            } else {
                return true;
            }
            });
}
void Worker::CraftCore(double dt){
    // craft items

    // check if must transfer inventory
    if (selected_building_ptr_->building_status_ == BuildingStatus::READY){
        TransferInventoryCore();
    }
    // check if selected building is operating
    else if (selected_building_ptr_->building_status_ == BuildingStatus::OPERATING){
        selected_building_ptr_->effort_val_ += dt * skill_map_[SkillTypes::CRAFTING]->CalcEffortUnits();
    }
    
    //
    TakeNonRecipeInventory(); 
}
void Worker::TransferItem(ItemTypes item_type, double amount_request, std::shared_ptr<Building> building_ptr){
    ExecuteCoreAction(DELTATIME, 
        [this](){return selected_building_ptr_ != nullptr;}, 
        [this, item_type, amount_request](double dt){TransferItemCore(item_type, amount_request, selected_building_ptr_);}, 
        [this, item_type](){
            if (inventory_.HasNoneOf(item_type)){
                SetState(WorkerStates::IDLE);
                return true;
            } else {
                return false;
            }
        });
}
void Worker::TransferItemCore(ItemTypes item_type, double amount_request, std::shared_ptr<Building> building_ptr){
    // transfer item to building
    // if amount_request is negative, assume all
    double amount_to_transfer = 0.0;
    // check if item in inventory
    if (inventory_.find(item_type)){
        if (amount_request < 0.0)
            amount_to_transfer = inventory_.GetItem(item_type)->GetAmount();
        else
            // check if amount in inventory is greater than required
            amount_to_transfer = std::min(amount_request, inventory_.GetItem(item_type)->GetAmount());
    } else {
        return;
    }
    // add to building
    building_ptr->AddToInventory(item_type, amount_to_transfer);
    // remove from inventory
    inventory_.GetItem(item_type)->RemoveAmount(amount_to_transfer);
    must_update_stats_ = true;
}
void Worker::TransferItems(){
    ExecuteCoreAction(DELTATIME, 
        [this](){return selected_building_ptr_ != nullptr;}, 
        [this](double dt){
            auto ptr = task_ptr_->GetActiveAction();
            for (auto id: ptr->universal_ids_){
                TransferItemCore((ItemTypes)id, -1.0, ptr->building_ptr_);
            }
        }, 
        [this](){
            if (inventory_.HasNoneOf(task_ptr_->GetActiveAction()->universal_ids_)){
                SetState(WorkerStates::IDLE);
                return true;
            } else {
                return false;
            }
        });
}
bool Worker::TransferInventory(){
    ExecuteCoreAction(DELTATIME, 
        [this](){return selected_building_ptr_ != nullptr;}, 
        [this](double dt){TransferInventoryCore();}, 
        [this](){return false;}); // always done
    return true;
}
void Worker::TransferInventoryCore(){
    // Smart transfer inventory to building based on context
    // check if building is nullptr

    if (selected_building_ptr_->building_status_ == BuildingStatus::PRECONSTRUCTION){
        // transfer required items from inventory to building
        for (auto& item : selected_building_ptr_->item_reqs_map_){
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
    }

    if (selected_building_ptr_->building_status_ == BuildingStatus::READY){
        // check if selected building has a recipe
        if (selected_building_ptr_->active_recipe_idx_ != -1){
            // iterate over inputs
            for (auto& input : selected_building_ptr_->GetRecipe(selected_building_ptr_->active_recipe_idx_)->inputs_){
                // transfer item
                TransferItem(input.first, input.second, selected_building_ptr_);
            }
        } else {
            // // iterate over inventory
            // for (auto& item : inventory_.GetItemMap()){
            //     // transfer item
            //     TransferItem(item.first, item.second->GetAmount(), selected_building_ptr_);
            // }
        }
    }
}
bool Worker::CanTakeItems(){
    // check if I have room in my weight
    if (stats_map_[WorkerStats::CURRCARRY] >= stats_map_[WorkerStats::MAXCARRY]){
        return false;
    }
    return selected_building_ptr_->inventory_.HasNoneOf(task_ptr_->GetActiveAction()->universal_ids_);
}
void Worker::TakeItems(){
    ExecuteCoreAction(DELTATIME, 
        [this](){return selected_building_ptr_ != nullptr;}, 
        [this](double dt){
            auto ptr = task_ptr_->GetActiveAction();
            for (auto id: ptr->universal_ids_){
                TakeItemCore((ItemTypes)id, -1.0, ptr->building_ptr_);
            }
        }, 
        [this](){
            if (!CanTakeItems()){
                SetState(WorkerStates::IDLE);
                return true;
            } else {
                return false;
            }
        });
}
void Worker::TakeItemCore(ItemTypes item_type, double amount_request, std::shared_ptr<Building> building_ptr){
    // transfer item to building
    double amount_to_transfer = 0.0;
    // check if item in building
    if (building_ptr->inventory_.find(item_type)){
        // get max available
        if (amount_request < 0.0) {
            amount_to_transfer = building_ptr->inventory_.GetItem(item_type)->GetAmount();
        } else {
            // check if amount in building is greater than required
            amount_to_transfer = std::min(amount_request, building_ptr->inventory_.GetItem(item_type)->GetAmount());
        }
        // calculate max allowable
        auto max_allowable = (stats_map_[WorkerStats::MAXCARRY] - stats_map_[WorkerStats::CURRCARRY]) / building_ptr->inventory_.GetItem(item_type)->density_;
        amount_to_transfer = std::min(amount_to_transfer, max_allowable);
    } else {
        return;
    }
    // add to inventory
    if (AddToInventory(item_type, amount_to_transfer))
        // remove from building
        building_ptr->inventory_.GetItem(item_type)->RemoveAmount(amount_to_transfer);
}
void Worker::TakeInventory(){
    // stub
}
void Worker::TakeNonRecipeInventory(){
    // will only take items if the building is in the ready or operating state
    if (selected_building_ptr_ == nullptr){
        return;
    }

    // check if selected building is in the ready state
    if (!(selected_building_ptr_->CheckState(BuildingStatus::READY) || selected_building_ptr_->CheckState(BuildingStatus::OPERATING)))
        return;

    for (auto& item: selected_building_ptr_->GetNonRecipeItems()){
        // transfer item
        TakeItemCore(item->item_type_, item->GetAmount(), selected_building_ptr_);
    }
}
void Worker::Eat(){
    // iterate over inventory, find first item with item_category_ == ItemCategories::FOOD, then remove it from inventory and increase need value
    for (auto& item : inventory_.GetItemMap()){
        if (item.second->IsItemCategory(ItemCategories::FOOD)){
            auto food = std::static_pointer_cast<FoodItem>(item.second);
            // get amount to eat, protection against negative value
            double amount_to_eat = std::min(1.0, item.second->GetAmount());
            needs_map_[NeedsTypes::FOOD].val_ += amount_to_eat * food->nutrient_amount_;
            // subtract from inventory
            item.second->RemoveAmount(amount_to_eat);
            return;
        }
    }
}
bool Worker::InferActionWide(double dt){
    // infer simple actions based off NearbySurroundings
    
    // buildings have priority 
    // iterate through nearby surroundings
	double dist = 99;
	BuildingPtr new_building_ptr = nullptr;

    for (auto& building : nearby_surroundings_->buildings_){
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
        return true;
	}
    return false;
}

void Worker::SetDesiredState(WorkerStates state)
{
    // reset
    ResetDesiredState();

    // add
    AddDesiredState(state);
}

/// @brief Infer action
/// @return 
bool Worker::InferAction(bool append_action)
{
    auto state = WorkerStates::UNDEFINED;

    // if we have attention, then figure out what we can do
    if (HasAttention())
    {
        // default, move
        // BUG: if you right click a building, then cangather is false, so then the state becomes moving, but the target was never updated from 0, 0 so then the worker begins moving towards the origin.
        state = WorkerStates::MOVING;

        // gather resource
        if (CanGather())
        {
            state = WorkerStates::GATHERING;
        }
    }
    
    // if we've set a state, then either add or set
    if (state != WorkerStates::UNDEFINED)
    {
        // check if we're appending or setting
        if (append_action)
        {
            // append state
            AddDesiredState(state);
        }
        else
        {
            // set state
            SetDesiredState(state);
        }

        // successfully set
        return true;
    }

    // default return
    return false;
}
bool Worker::InferAction()
{
    // by default set, not append actions
    return InferAction(false);
}

bool Worker::InferAction(double dt){
    // Infer an action based on the attention (selected_building_ptr_)
    if (HasAttention())
    {
        if (InferAction(dt, selected_building_ptr_)) // building first priority
            return true;
        if (InferAction(dt, selected_resource_ptr_)) // resource second priority
            return true;

    } 
    else
    {
        if (InferActionWide(dt))
            return true;
    }
    return false;
}
bool Worker::InferAction(double dt, BuildingPtr ptr){
    if (ptr == nullptr)
        return false;

    // check if close enough for interaction
    if (eucl_dist<double>(GetCenter(), selected_building_ptr_->center_) < interaction_dist_){
        SetState(WorkerStates::CONSTRUCTING);
        Construct(dt);
    } else {
        // set goal to selected building
        SetGoalToSelectedBuilding();
        SetState(WorkerStates::MOVING);
    }
    return true;
}
bool Worker::InferAction(double dt, ResourcePtr ptr){
    if (ptr == nullptr)
        return false;

    // see if selected resource is gatherable and has resources
    if (!CanGather(ptr)){
        return false;
    }

    // check if close enough for interaction
    if (eucl_dist<double>(GetCenter(), ptr->GetCenter()) < interaction_dist_){
        SetState(WorkerStates::GATHERING);
        BeginGather(dt);
    } else {
        // set goal to selected resource
        SetGoalToSelectedResource();
        SetState(WorkerStates::MOVING);
    }
    return true;
}
bool Worker::IsCloseEnough(GameObjectPtr ptr){
    // check if close enough to interact with resource
    if (ptr == nullptr)
        return false;
    return eucl_dist<double>(GetCenter(), ptr->GetCenter()) < interaction_dist_;

}
void Worker::SetTask(TaskPtr task_ptr){
    // set the task
    task_ptr_ = task_ptr->copy();

    // hack: for now hard code, in the future abstract it
    // iterate through actions
    for (auto& action: task_ptr_->actions_){
        // switch on action type
        switch (action->GetActiveActionType()){
            case ActionTypes::Gather:
                action->on_enter_cb_ = [this](){
                    SetAttention(task_ptr_);
                    SetGoalToSelectedResource();
                    SetState(WorkerStates::GATHERING);
                    };
                break;
            case ActionTypes::GatherArea:
                action->on_enter_cb_ = [this](){
                    SetAttention(task_ptr_);
                    SetState(WorkerStates::GATHERIDLE);
                    };
                break;
            case ActionTypes::Construct:
                action->on_enter_cb_ = [this](){
                    SetAttention(task_ptr_);
                    SetGoalToSelectedBuilding();
                    SetState(WorkerStates::CONSTRUCTING);
                    };
                break;
            case ActionTypes::Craft:
                action->on_enter_cb_ = [this](){
                    SetAttention(task_ptr_);
                    SetGoalToSelectedBuilding();
                    SetState(WorkerStates::CRAFTING);
                    };
                break;
            case ActionTypes::TransferItems:
                action->on_enter_cb_ = [this](){
                    SetAttention(task_ptr_);
                    SetState(WorkerStates::IDLE);
                    };
                break;
            case ActionTypes::TakeItems:
                action->on_enter_cb_ = [this](){
                    SetAttention(task_ptr_);
                    SetState(WorkerStates::IDLE);
                    };
                break;

        }
    }
}
void Worker::ExecuteTask(double dt){
    // Execute a task
    // TODO: move this to dynamic object class
    if (task_ptr_ == nullptr || task_ptr_->actions_.size() == 0){
        std::cout << "No active task or no actions in task." << std::endl;
        SetState(WorkerStates::IDLE);
        return;
    }
    
    // call the continuous function
    auto action_type = task_ptr_->GetActiveActionType(); // recurse get active action type
    action_primitive_map_[action_type]();
    // end

    task_ptr_->update(CheckState(WorkerStates::IDLE)); // recurse update off completion of action
}
void Worker::UpdateStats(){
    // iterate through stats and re-calculate them all
    // for now, hard code
    // CURRCARRY
    stats_map_[WorkerStats::CURRCARRY] = inventory_.CalcTotalMass();

    // MAXCARRY
    stats_map_[WorkerStats::MAXCARRY] = attribute_map_[WorkerAttributeTypes::STRENGTH].GetVal(); // base value
    for (auto& item: inventory_.GetItemMap()){
        // try to cast to ToolType
        if (item.second->item_category_ == ItemCategories::TOOL){
            auto tool = std::static_pointer_cast<ToolItem>(item.second);
            // check if the itemtype is in ITEM_TO_WORKER_ATTRIBUTE_MAP
            if (ITEM_TO_WORKER_ATTRIBUTE_MAP.find(item.first) != ITEM_TO_WORKER_ATTRIBUTE_MAP.end()){
                // check if the attribute is STRENGTH
                if (ITEM_TO_WORKER_ATTRIBUTE_MAP[item.first] == WorkerAttributeTypes::STRENGTH){
                    // add to maxcarry
                    stats_map_[WorkerStats::MAXCARRY] += tool->additive_mod_;
                    stats_map_[WorkerStats::MAXCARRY] *= tool->mult_mod_;
                }
            }
        }
    }
}

bool Worker::IdleCB(double dt)
{
    // Reset stuff, then begin to infer the next action
    if (infer_actions_ && !InferAction(dt))
        // no actions to infer from current attention
        ClearAttention();
    else
        ClearAttention();

    return true;
}

/*
Actions.
These are the entry point for actions from tasks. Since they're called within the FSM, they can set the state when complete.
*/
void Worker::actionSelectClosestBuilding(){
    if (SelectClosestBuilding())
        SetState(WorkerStates::IDLE);
}
void Worker::actionMoveTowardsGoal(){
    if (MoveTowardsGoal()){
        SetState(WorkerStates::IDLE);
    }
}
void Worker::actionSetGoalToSelectedResource(){
    SetGoalToSelectedResource();
    SetState(WorkerStates::IDLE); // auto success
    }
void Worker::actionSetGoalToSelectedBuilding(){
    SetGoalToSelectedBuilding();
    SetState(WorkerStates::IDLE); // auto success
}
void Worker::actionSelectBuilding(){
    auto ptr = task_ptr_->GetActiveAction();
    SelectBuilding(ptr->building_ptr_);
    SetState(WorkerStates::IDLE); // autocomplete
}
void Worker::actionSelectResource(){
    auto ptr = task_ptr_->GetActiveAction();
    SelectResource(ptr->resource_ptr_);
    SetState(WorkerStates::IDLE); // autocomplete
}
void Worker::actionGather(){
    Gather(DELTATIME);
}
void Worker::actionGatherArea(){
    GatherArea(DELTATIME);
}
void Worker::actionConstruct(){
    Construct(DELTATIME);
}
void Worker::actionTakeInventory(){
    TakeInventory();
    SetState(WorkerStates::IDLE);
}
void Worker::actionTakeItems(){
    TakeItems();
}
void Worker::actionTransferItems(){
    TransferItems();
}
void Worker::actionTransferInventory(){
    TransferInventory();
    SetState(WorkerStates::IDLE);
}
void Worker::actionCraft(){
    Craft(DELTATIME);
}
void Worker::actionInfer(){
    SetAttention(task_ptr_);
    SetState(WorkerStates::INFERRING);
    InferAction(DELTATIME);
}
/////////////////////////////////////// End Worker ///////////////////////////////////////