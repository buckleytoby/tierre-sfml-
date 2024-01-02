#include "gameplay.hpp"





/////////////////////////////////////// GamePlay ///////////////////////////////////////
// write class methods
GamePlay::~GamePlay(){
    // destructor
}
void GamePlay::draw(sf::RenderWindow& window){
    hud_.Draw(window);
}
void GamePlay::update(double dt){
    //// meta updates
    // update the GUI
    hud_.Update(dt);

    //// update the gameplay
    // update the map
    map_.update(dt);

    // update the viewport
    viewport_.update(dt);
}
void GamePlay::load(sf::RenderWindow& window){
    // load the gameplay
    // set the viewport window reference
    viewport_.SetWindowHandle(&window);

}
void GamePlay::unload(){
    // unload the gameplay
}
/////////////////////////////////////// End GamePlay ///////////////////////////////////////

/////////////////////////////////////// Viewport ///////////////////////////////////////
void Viewport::SetX(double x){
    x_ = x;
}
void Viewport::SetY(double y){
    y_ = y;
}
void Viewport::SetWidth(double width){
    if (width < min_width_){
        width = min_width_;
    } else {
        width_ = width;
    }
}
void Viewport::SetHeight(double height){
    if (height < min_height_){
        height = min_height_;
    } else {
        height_ = height;
    }
}
void Viewport::SetWindowHandle(sf::RenderWindow* window){
    window_ptr_ = window;
}
int Viewport::GetIntX(){
    return floor(x_);
}
int Viewport::GetIntY(){
    return floor(y_);
}
double Viewport::GetX(){
    return x_;
}
double Viewport::GetY(){
    return y_;
}
int Viewport::GetIntWidth(){
    return (int)width_;
}
int Viewport::GetIntHeight(){
    return (int)height_;
}
void Viewport::update(double dt){
    // update the viewport
    double dx = 0;
    double dy = 0;
    double dz = 0;
    // if over the bitwise actions
    if (viewport_actions_.HasFlag((EFlagValue)ViewportActions::SCROLL_UP)){
        dy += scroll_sensitivity_;
    }
    if (viewport_actions_.HasFlag((EFlagValue)ViewportActions::SCROLL_DOWN)){
        dy -= scroll_sensitivity_;
    }
    if (viewport_actions_.HasFlag((EFlagValue)ViewportActions::SCROLL_LEFT)){
        dx -= scroll_sensitivity_;
    }
    if (viewport_actions_.HasFlag((EFlagValue)ViewportActions::SCROLL_RIGHT)){
        dx += scroll_sensitivity_;
    }
    if (viewport_actions_.HasFlag((EFlagValue)ViewportActions::ZOOM_IN_ONCE)){
        dz -= zoom_sensitivity_;
        viewport_actions_.UnsetFlag((EFlagValue)ViewportActions::ZOOM_IN_ONCE);
    }
    if (viewport_actions_.HasFlag((EFlagValue)ViewportActions::ZOOM_OUT_ONCE)){
        dz += zoom_sensitivity_;
        viewport_actions_.UnsetFlag((EFlagValue)ViewportActions::ZOOM_OUT_ONCE);
    }
    // calc center of viewport and aspect ratio
    double cx = x_ + width_ / 2;
    double cy = y_ + height_ / 2;
    double aspect_ratio = width_ / height_;

    // update width & height
    SetWidth(width_ + dz);
    SetHeight(height_ + dz / aspect_ratio);

    // new x & y
    double new_x = cx - width_ / 2;
    double new_y = cy - height_ / 2;

    // update x & y
    SetX(new_x + dx * dt);
    SetY(new_y + dy * dt);
}
void Viewport::UpdateMousePosition(){
    // update the mouse position
    auto mouse_pos = sf::Mouse::getPosition(*window_ptr_);
    mouse_x_ = mouse_pos.x;
    mouse_y_ = mouse_pos.y;
    map_mouse_x_ = ConvertPixelToMeterX(*window_ptr_, mouse_x_);
    map_mouse_y_ = ConvertPixelToMeterY(*window_ptr_, mouse_y_);
}
double Viewport::GetPixelsPerMeterX(sf::RenderWindow& window){
    // get pixels per meter x
    // TODO: cache this
    return (double)window.getSize().x / width_;
}
double Viewport::GetPixelsPerMeterY(sf::RenderWindow& window){
    // get pixels per meter y
    // TODO: cache this
    return(double)window.getSize().y / height_;
}

double Viewport::ConvertPixelToMeterX(sf::RenderWindow& window, double pixel_x){
    // convert pixel x to meter x
    double pixelsPerMeterW = GetPixelsPerMeterX(window);
    double meter_x = pixel_x / pixelsPerMeterW + x_;
    return meter_x;
}
double Viewport::ConvertPixelToMeterY(sf::RenderWindow& window, double pixel_y){
    // convert pixel y to meter y
    double pixelsPerMeterH = GetPixelsPerMeterY(window);
    // transform from SFML window coordinate system (x-right, y-down) to gameplay coordinate system (x-right, y-up)
    double new_pixel_y = window.getSize().y - pixel_y;

    double meter_y = new_pixel_y / pixelsPerMeterH + y_;
    return meter_y;
}
double Viewport::ConvertMeterToPixelX(sf::RenderWindow& window, double meter_x){
    // convert meter x to pixel x
    double pixelsPerMeterW = GetPixelsPerMeterX(window);
    double pixel_x = (meter_x - x_) * pixelsPerMeterW;
    return pixel_x;
}
double Viewport::ConvertMeterToPixelY(sf::RenderWindow& window, double meter_y){
    // convert meter y to pixel y
    double pixelsPerMeterH = GetPixelsPerMeterY(window);
    double pixel_y = (meter_y - y_) * pixelsPerMeterH;

    // transform from gameplay coordinate system (x-right, y-up) to SFML window coordinate system (x-right, y-down)
    pixel_y = window.getSize().y - pixel_y;
    return pixel_y;
}
/////////////////////////////////////// End Viewport ///////////////////////////////////////

/////////////////////////////////////// Tile ///////////////////////////////////////
std::shared_ptr<Resource> Tile::GetResource(ResourceTypes resource_type){
    // get a resource from the tile
    // check if resource_type in resource_map_
    if (resource_map_.find(resource_type) != resource_map_.end()){
        return resource_map_[resource_type];
    } else {
        return nullptr;
    }
}
void Tile::AddResource(std::shared_ptr<Resource> resource_ptr){
    // add a resource to the tile
    // check if resource_type in resource_map_
    if (resource_map_.find(resource_ptr->resource_type_) != resource_map_.end()){
        // add to the resource
        resource_map_[resource_ptr->resource_type_]->AddTotal(resource_ptr->GetTotal());
    } else {
        // add the resource
        resource_map_[resource_ptr->resource_type_] = resource_ptr;
    }

}
/////////////////////////////////////// End Tile ///////////////////////////////////////

/////////////////////////////////////// Map ///////////////////////////////////////
void Map::SetWidth(int width){
    width_ = width;
}
void Map::SetHeight(int height){
    height_ = height;
}
void Map::CreateTiles(){
    // create the tiles
    for (int i=0; i<width_; i++){
        std::vector<std::shared_ptr<Tile>> tiles_array_row;
        for (int j=0; j<height_; j++){
            auto tile = std::make_shared<Tile>();
            tile->x_ = i;
            tile->y_ = j;
            tiles_array_row.push_back(tile);
        }
        tiles_array_.push_back(tiles_array_row);
    }
}
void Map::SetTile(int x, int y, std::shared_ptr<Tile> tile){
    // set a tile on the map
    tiles_array_[x][y] = tile;
    tiles_array_[x][y]->x_ = x;
    tiles_array_[x][y]->y_ = y;
}

std::vector<std::shared_ptr<Tile>> Map::GetTileSlice(Rect<double> rect){
    // get a slice of the map
    int min_x = floor(rect.x_);
    int min_y = floor(rect.y_);
    int max_x = ceil(rect.x_ + rect.width_);
    int max_y = ceil(rect.y_ + rect.height_);

    std::vector<std::shared_ptr<Tile>> tiles;

    // iterate from min to max x, y
    for(int x = min_x; x < max_x; x++){
        for(int y = min_y; y < max_y; y++){
            auto tile = GetTile(x, y);
            if (tile != nullptr){
                tiles.push_back(tile);
            }
        }
    }
    return tiles;
}
std::map<ResourceTypes, std::shared_ptr<Resource>> Map::GetResourceSlice(Rect<double> rect){
    std::map<ResourceTypes, std::shared_ptr<Resource>> resources;

    // get a slice of the map
    auto tiles = GetTileSlice(rect);

    // iterate over tiles
    for (auto& tile : tiles){
        // iterate through resources
        for(auto& resource : tile->resource_map_){
            ResourceTypes resource_type = resource.second->resource_type_;
            resources[resource_type] = resource.second;
        }
    }
    return resources;
}
std::map<BuildingTypes, std::shared_ptr<Building>> Map::GetBuildingSlice(Rect<double> rect){
    std::map<BuildingTypes, std::shared_ptr<Building>> buildings;

    // get a slice of the map
    auto tiles = GetTileSlice(rect);

    // iterate over tiles
    for (auto& tile : tiles){
        // iterate through resources
        if (tile->building_type_ != BuildingTypes::NONE){
            BuildingTypes building_type = tile->building_type_;
            buildings[building_type] = tile->building_ptr_;
        }
    }
    return buildings;
}

std::shared_ptr<Tile> Map::GetTile(int x, int y){
    // get a tile from the map
    // check x, y in bounds
    if (x < 0 || x >= width_ || y < 0 || y >= height_){
        return nullptr;
    } else {
        return tiles_array_[x][y];
    }
}
void Map::update(double dt){
    // update the map
    // update the buildings
    for (int i=0; i<width_; i++){
        for (int j=0; j<height_; j++){
            // check if building
            if (tiles_array_[i][j]->building_type_ != BuildingTypes::NONE){
                // update the building
                tiles_array_[i][j]->building_ptr_->update(dt);
            }
        }
    }

    // update dynamic objects
    for (auto& dynamic_object_ptr : dynamic_object_ptrs_){
        dynamic_object_ptr->update(dt);

        // if worker
        if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::WORKER){
            auto worker = std::static_pointer_cast<Worker>(dynamic_object_ptr);
            // update senses
            auto resources = GetResourceSlice(worker->GetImmediateSurroundingsRect());
            auto buildings = GetBuildingSlice(worker->GetImmediateSurroundingsRect());
            worker->immediate_surroundings_.SetResource(resources);
            worker->immediate_surroundings_.SetBuilding(buildings);
            resources = GetResourceSlice(worker->GetNearbySurroundingsRect());
            buildings = GetBuildingSlice(worker->GetNearbySurroundingsRect());
            worker->nearby_surroundings_.SetResource(resources);
            worker->nearby_surroundings_.SetBuilding(buildings);
        }
    }
}
void Map::SelectObject(double x, double y){
    // keywords: selection, selectobject
    // clear selected objects
    selected_dynamic_object_ptrs_.clear();
    selected_tile_ptr_ = nullptr;

    // select an object
    SelectDynamicObject(x, y);

    // if no dynamic objects, get the tile
    if (selected_dynamic_object_ptrs_.size() == 0){
        // get tile int x, y
        int tile_x = floor(x);
        int tile_y = floor(y);
        auto tile = GetTile(tile_x, tile_y);
        // check if tile is nullptr
        if (tile == nullptr){
            return;
        } else {
            selected_tile_ptr_ = std::shared_ptr<Tile>(tile);
        }
    }

    // print dynamic object details
    for (auto &dynamic_object_ptr : selected_dynamic_object_ptrs_){
        std::cout << "Selected dynamic object: " << dynamic_object_ptr->footprint_.x_ << ", " << dynamic_object_ptr->footprint_.y_ << std::endl;
    }
}
void Map::SelectDynamicObject(double x, double y){
    // keywords: selection, selectobject
    // select a dynamic object
    // TODO: put dynamic objects into a quadtree
    // clear selected dynamic objects
    selected_dynamic_object_ptrs_.clear();
    // iterate through dynamic objects
    for (auto& dynamic_object_ptr : dynamic_object_ptrs_){
        // check if x, y is near the dynamic object
        if (dynamic_object_ptr->PointInFootprint(x, y)){
            // select the dynamic object
            selected_dynamic_object_ptrs_.push_back(dynamic_object_ptr);
        }
    }
}
void Map::MakeBuilding(BuildingTypes building_type, double x, double y){
    // make a building
    // get tile int x, y
    int tile_x = floor(x);
    int tile_y = floor(y);
    auto tile = GetTile(tile_x, tile_y);
    // check if tile is nullptr
    if (tile == nullptr){
        return;
    }
    // check if tile has building
    if (tile->building_type_ != BuildingTypes::NONE){
        return;
    }

    // make building using the factory
    tile->building_type_ = building_type;
    tile->building_ptr_ = BuildingFactory::MakeBuilding(building_type);
    // set the footprint x, y
    tile->building_ptr_->footprint_.x_ = tile_x;
    tile->building_ptr_->footprint_.y_ = tile_y;
    tile->building_ptr_->center_.x_ = tile_x + 0.5;
    tile->building_ptr_->center_.y_ = tile_y + 0.5;
    
    // Debug printout
    std::cout << "Building created: " << to_string(building_type) << " at: " << tile_x << ", " << tile_y << std::endl;
}
void Map::MakeWorker(double x, double y){
    // make worker
    auto worker = std::make_shared<Worker>();
    worker->footprint_.x_ = x;
    worker->footprint_.y_ = y;
    worker->SetSpeed(1);
    worker->needs_map_[NeedsTypes::FOOD].val_ = 100.0;
    dynamic_object_ptrs_.push_back(worker);
}
void Map::SetAttention(DynamicObjectPtr ptr, double x, double y){
    // get tile int x, y
    int tile_x = floor(x);
    int tile_y = floor(y);
    auto tile = GetTile(tile_x, tile_y);
    BuildingPtr building_ptr{nullptr};
    if (tile->HasBuildingPtr()){
        building_ptr = tile->building_ptr_;
    }
    if (ptr->IsType(DynamicObjectTypes::WORKER)){
        auto worker = std::static_pointer_cast<Worker>(ptr);
        worker->SetAttention(building_ptr);
    }
}
void Map::SetAttention(double x, double y){
    // Implication: for all dynamic objects
    for (auto &dynamic_object_ptr : selected_dynamic_object_ptrs_)
    {
        SetAttention(dynamic_object_ptr, x, y);
    }
}
void Map::SetGoal(DynamicObjectPtr ptr, double x, double y){
    if (ptr->IsType(DynamicObjectTypes::WORKER)){
        std::static_pointer_cast<Worker>(ptr)->SetGoal(x, y);
    }
}
void Map::MoveTowardsGoal(DynamicObjectPtr ptr){
    if (ptr->IsType(DynamicObjectTypes::WORKER)){
        std::static_pointer_cast<Worker>(ptr)->MoveTowardsGoal();
    }
}
void Map::SetAttentionAndMove(DynamicObjectPtr ptr, double x, double y){
    // set attention and move
    SetAttention(ptr, x, y);
    SetGoal(ptr, x, y);
    MoveTowardsGoal(ptr);
}
void Map::SetAttentionAndMove(double x, double y){
    // Implication: for all dynamic objects
    for (auto &dynamic_object_ptr : selected_dynamic_object_ptrs_)
    {
        SetAttentionAndMove(dynamic_object_ptr, x, y);
    }
}
/////////////////////////////////////// End Map ///////////////////////////////////////

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
    switch (worker_state_){
        case WorkerStates::DEAD:
            // do nothing, it's dead. Shouldn't ever get here though.
            break;
        case WorkerStates::IDLE:
            // Attempt to infer an action based on the attention
            ResetDynamicObjectActions();
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
void Worker::SetGoal(double x, double y){
    // set the goal for the worker
    goal_.x_ = x;
    goal_.y_ = y;
}
Rect<double> Worker::GetImmediateSurroundingsRect(){
    // get the immediate surroundings rect
    auto local_rect = immediate_surroundings_.GetLocalRect();
    local_rect.x_ += footprint_.x_;
    local_rect.y_ += footprint_.y_;
    return local_rect;
}
Rect<double> Worker::GetNearbySurroundingsRect(){
    // get the nearby surroundings rect
    auto local_rect = nearby_surroundings_.GetLocalRect();
    local_rect.x_ += footprint_.x_;
    local_rect.y_ += footprint_.y_;
    return local_rect;
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
        auto dist = eucl_dist<double>(GetCenter(), building.second->center_);
        if (dist < min_dist){
            min_dist = dist;
            found = true;
            SelectBuilding(building.second);
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
    if (abs(footprint_.x_ - goal_.x_) < goal_dist_threshold_){
        done_x = true;
    } else {
        if (footprint_.x_ < goal_.x_){
            dynamic_object_actions_.SetFlag((EFlagValue)DynamicObjectActions::MOVE_RIGHT);
        } else {
            dynamic_object_actions_.SetFlag((EFlagValue)DynamicObjectActions::MOVE_LEFT);
        }
    }

    if (abs(footprint_.y_ - goal_.y_) < goal_dist_threshold_){
        done_y = true;
    } else {
        if (footprint_.y_ < goal_.y_){
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
            selected_resource_ptr_ = immediate_surroundings_.resources_.begin()->second;
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
            if (building.second->building_status_ == BuildingStatus::PRECONSTRUCTION || building.second->building_status_ == BuildingStatus::CONSTRUCTION){
                // set building to construct
                selected_building_ptr_ = building.second;
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
    if (selected_building_ptr_->active_recipe_ != RecipeTypes::NONE){
        // iterate over inputs
        for (auto& input : selected_building_ptr_->recipes_[selected_building_ptr_->active_recipe_].inputs_){
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
void Worker::InferAction(double dt){
    // Infer an action based on the attention (selected_building_ptr_)
    if (HasAttention()){
        SetState(WorkerStates::CONSTRUCTING);
        EngageWithBuilding(selected_building_ptr_);
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
void Worker::MakeTask1(){
    // This is the very first "task" function.
    // It is supposed to be called from the UPDATE function, so at FRAMERATE times per second.
    // Therefore, any function which has a success criteria (e.g. MoveTowardsGoal) cannot be blocking.
    // Must maintain a semi-permanent meta-structure which tracks the actions in the task,
    // sets the worker state, increments through the task actions, and finally restarts

    // EXECUTINGTASK should be a separate worker state, then, which will be called by the UPDATE function
    // and ultimately enters here. The task manager should then take ownership, set the appropriate worker state
    // so that actions can proceed as normal, and before the function returns revert the state back to EXECUTINGTASK
    // to set up for the next loop.

    // User will have to press a dedicated 'execute task' button and can break out of it at anytime by providing a different command
    // ex: right clicking to move

    std::vector<std::string> task_actions_ = {"SelectClosestBuilding", "SetGoalToSelectedBuilding", "MoveTowardsGoal", "TransferInventory"};
    std::vector<WorkerStates> task_start_states = {WorkerStates::ACTIVE, WorkerStates::ACTIVE, WorkerStates::MOVING, WorkerStates::ACTIVE};
    
    TaskPtr task = std::make_shared<Task>("Example Task Worker");
    for (int i=0; i<task_actions_.size(); i++){
        ActionPtr action = std::make_shared<Action>();
        task->AddAction(action);
    }
}
/////////////////////////////////////// End Worker ///////////////////////////////////////

/////////////////////////////////////// Building ///////////////////////////////////////
void Building::update(double dt){
    // update the building
    // update the building status
    switch (building_status_){
        case BuildingStatus::PRECONSTRUCTION:{
            // iterate through item_reqs_map_
            bool all_reqs_fulfilled = true;
            for (auto& item: item_reqs_map_){
                double reqd_amount = item.second;
                // check if item in inventory
                if (inventory_map_.find(item.first) != inventory_map_.end()){
                    // check if amount in inventory is greater than required
                    if (inventory_map_[item.first] < reqd_amount){
                        all_reqs_fulfilled = false;
                    }
                } else {
                    all_reqs_fulfilled = false;
                }
            }
            
            if (all_reqs_fulfilled){
                // remote required items from inventory
                for (auto& item : item_reqs_map_){
                    double reqd_amount = item.second;
                    // remove from inventory
                    AddToInventory(item.first, -reqd_amount);
                }
                // set building status to construction
                building_status_ = BuildingStatus::CONSTRUCTION;
            }
            break;}
        case BuildingStatus::CONSTRUCTION:
            // check if construction effort is greater than construction time
            if (effort_val_ > construction_effort_req_){
                // set building status to complete
                building_status_ = BuildingStatus::READY;
                effort_val_ = 0.0;
            }
            break;
        case BuildingStatus::READY:{
            // check if all items of the active recipe are fulfilled
            bool all_reqs_fulfilled = true;
            for (auto& input: recipes_[active_recipe_].inputs_){
                double reqd_amount = input.second;
                // check if item in inventory
                if (inventory_map_.find(input.first) != inventory_map_.end()){
                    // check if amount in inventory is greater than required
                    if (inventory_map_[input.first] < reqd_amount){
                        all_reqs_fulfilled = false;
                    }
                } else {
                    all_reqs_fulfilled = false;
                }
            }
            if (all_reqs_fulfilled){
                // remove required items from inventory
                for (auto& input : recipes_[active_recipe_].inputs_){
                    double reqd_amount = input.second;
                    // remove from inventory
                    AddToInventory(input.first, -reqd_amount);
                }
                // set building status to operating
                building_status_ = BuildingStatus::OPERATING;
            }
            break;}
        case BuildingStatus::OPERATING:
            if (effort_val_ > recipes_[active_recipe_].effort_req_){
                // add outputs to inventory
                for (auto& output : recipes_[active_recipe_].outputs_){
                    // add to inventory
                    AddToInventory(output.first, output.second);
                }
                // set effort val to zero
                effort_val_ = 0.0;
                building_status_ = BuildingStatus::READY;
            }
        default:
            break;
    }
}
void Building::AddToInventory(ItemTypes item_type, double amount){
    // add to the storage
    // check if item_type in inventory_map_
    if (inventory_map_.find(item_type) != inventory_map_.end()){
        // add to the resource
        inventory_map_[item_type] += amount;
    } else {
        // add the resource
        inventory_map_[item_type] = amount;
    }
}
/////////////////////////////////////// End Building ///////////////////////////////////////