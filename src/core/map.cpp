#include "map.hpp"

/////////////////////////////////////// Map ///////////////////////////////////////
Map::Map(): Interactive()
{
    // ctor
    SetHandleInputCb(std::bind(&Map::onHandleInput, this, std::placeholders::_1));
    SetOnSelectCB(std::bind(&Map::SelectMouseObjects, this));
}
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
            auto tile = std::make_shared<Tile>(i, j);
            tiles_array_row.push_back(tile);
        }
        tiles_array_.push_back(tiles_array_row);
    }

    // iterate through tiles to generate the resources
    for (int i=0; i<width_; i++){
        for (int j=0; j<height_; j++){
            // get the neighbors
            auto neighbors = GetNeighbors(tiles_array_[i][j]);
            // generate resources
            tiles_array_[i][j]->SetResource(MapGen::GenerateResourceFromNeighbors(neighbors));
            tiles_array_[i][j]->GetResource()->SetPos(i, j);
        }
    }
}
void Map::SetTile(int x, int y, std::shared_ptr<Tile> tile){
    // set a tile on the map
    tile->SetPos(x, y);
    tiles_array_[x][y] = tile;

}
std::vector<XY<int>> Map::GetCoordsSlice(double x1, double y1, double x2, double y2){
    Rect<double> rect(x1, y1, x2-x1, y2-y1);
    return GetCoordsSlice(rect);
}
std::vector<XY<int>> Map::GetCoordsSlice(Rect<double> rect){
    // get a slice of the map
    int min_x = floor(rect.x_);
    int min_y = floor(rect.y_);
    int max_x = ceil(rect.x_ + rect.width_);
    int max_y = ceil(rect.y_ + rect.height_);

    std::vector<XY<int>> coords;

    // iterate from min to max x, y
    for(int x = min_x; x < max_x; x++){
        for(int y = min_y; y < max_y; y++){
            coords.push_back(XY<int>(x, y));
        }
    }
    return coords;
}

std::vector<std::shared_ptr<Tile>> Map::GetTileSlice(Rect<double> rect){
    // get a slice of the map
    std::vector<std::shared_ptr<Tile>> tiles;
    auto coords = GetCoordsSlice(rect);

    // iterate through coords
    for (auto& coord : coords){
        // get the tile
        int x = coord.x_;
        int y = coord.y_;
        auto tile = GetTile(x, y);
        if (tile != nullptr){
            tiles.push_back(tile);
        }
    }
    return tiles;
}
ResourcePtr Map::GetResource(double x, double y){
    // get a resource from the map
    return GetTile(x, y)->GetResource();
}
BuildingPtr Map::GetBuilding(double x, double y){
    // get a building from the map
    return GetTile(x, y)->GetBuildingPtr();
}
std::vector<ResourcePtr> Map::GetResourceSlice(Rect<double> rect){
    std::vector<ResourcePtr> resources;

    // get a slice of the map
    auto tiles = GetTileSlice(rect);

    // iterate over tiles
    for (auto& tile : tiles){
        // iterate through resources
        if (tile->HasResource()){
            resources.push_back(tile->GetResource());
        }
    }
    return resources;
}
std::vector<BuildingPtr> Map::GetBuildingSlice(Rect<double> rect){
    std::vector<BuildingPtr> buildings;

    // get a slice of the map
    auto tiles = GetTileSlice(rect);

    // iterate over tiles
    for (auto& tile : tiles){
        // iterate through resources
        if (tile->building_type_ != BuildingTypes::NONE){
            BuildingTypes building_type = tile->building_type_;
            buildings.push_back(tile->building_ptr_);
        }
    }
    return buildings;
}
std::vector<DynamicObjectPtr> Map::GetDynamicObjectSlice(Rect<double> in_bounds){
    std::vector<DynamicObjectPtr> dynamic_objects;

    // convert Rect to sf::Rect
    sf::Rect<double> bounds(in_bounds.x_, in_bounds.y_, in_bounds.width_, in_bounds.height_);

    // iterate through dynamic objects
    for (auto& dynamic_object_ptr : dynamic_object_ptrs_){        
        if (bounds.intersects(dynamic_object_ptr->GetBoundingRect())){
            // select the dynamic object
            dynamic_objects.push_back(dynamic_object_ptr);
        }
    }
    return dynamic_objects;
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
std::shared_ptr<Tile> Map::GetTile(double x, double y){
    int tile_x = floor(x);
    int tile_y = floor(y);
    return GetTile(tile_x, tile_y);
}
std::shared_ptr<Tile> Map::GetTile(){
    // get the tile at the mouse position
    return GetTile(mouse_x_, mouse_y_);
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

    // iterate through dynamic objects
    for (auto& dynamic_object_ptr : dynamic_object_ptrs_){
        // update dynamic objects
        dynamic_object_ptr->Update(dt);

        // if worker
        if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::WORKER){
            // cast to worker
            auto worker = std::static_pointer_cast<Worker>(dynamic_object_ptr);

            // update senses
            auto resources = GetResourceSlice(worker->GetImmediateSurroundingsRect());
            auto buildings = GetBuildingSlice(worker->GetImmediateSurroundingsRect());
            auto dynamos = GetDynamicObjectSlice(worker->GetImmediateSurroundingsRect());
            worker->immediate_surroundings_->SetResource(resources);
            worker->immediate_surroundings_->SetBuilding(buildings);
            worker->immediate_surroundings_->SetDynamicObjects(dynamos);
            resources = GetResourceSlice(worker->GetNearbySurroundingsRect());
            buildings = GetBuildingSlice(worker->GetNearbySurroundingsRect());
            dynamos = GetDynamicObjectSlice(worker->GetNearbySurroundingsRect());
            worker->nearby_surroundings_->SetResource(resources);
            worker->nearby_surroundings_->SetBuilding(buildings);
            worker->nearby_surroundings_->SetDynamicObjects(dynamos);
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
        std::cout << "Selected dynamic object: " << dynamic_object_ptr->GetX() << ", " << dynamic_object_ptr->GetY() << std::endl;
    }
}
Rect<double> Map::GetMouseRect(){
    // get the mouse rect
    Rect<double> rect;
    rect.x_ = mouse_down_x_;
    rect.y_ = mouse_down_y_;
    rect.width_ = mouse_x_ - mouse_down_x_;
    rect.height_ = mouse_y_ - mouse_down_y_;
    return rect;
}
void Map::SelectMouseObjects(){
    // clear current selection
    selected_dynamic_object_ptrs_.clear();
    selected_tile_ptr_ = nullptr;

    auto bounds = GetMouseRect();
    selected_dynamic_object_ptrs_ = GetDynamicObjectSlice(bounds);
    if (selected_dynamic_object_ptrs_.size() == 0){
        SelectObject(bounds.x_ + bounds.width_, bounds.y_ + bounds.height_);
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
        if (dynamic_object_ptr->PointInBounds(x, y)){
            // select the dynamic object
            selected_dynamic_object_ptrs_.push_back(dynamic_object_ptr);
        }
    }
}
void Map::MakeBuilding(BuildingTypes building_type){
    // set the onSelect cb
    SetOnSelectCB([this, building_type](){
        auto coords = GetSelectRect()->GetCoords();
        for (auto& coord : coords){
            MakeBuilding(building_type, coord.x_, coord.y_);
        }
        // reset the onSelect cb
        SetOnSelectCB(std::bind(&Map::SelectMouseObjects, this));
    });
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
    tile->building_ptr_->SetPos(tile_x, tile_y);
    // set the footprint x, y
    tile->building_ptr_->footprint_.x_ = tile_x;
    tile->building_ptr_->footprint_.y_ = tile_y;
    tile->building_ptr_->center_.x_ = tile_x + 0.5;
    tile->building_ptr_->center_.y_ = tile_y + 0.5;
    
    // Debug printout
    // std::cout << "Building created: " << to_string(building_type) << " at: " << tile_x << ", " << tile_y << std::endl;
}
void Map::MakeWorker(double x, double y){
    // make worker
    auto worker = std::make_shared<Worker>();
    // set the map ref
    worker->immediate_surroundings_->map_ref_ = this; // hack
    worker->nearby_surroundings_->map_ref_ = this; // hack
    worker->SetX(x);
    worker->SetY(y);
    worker->SetSpeed(1);
    worker->needs_map_[NeedsTypes::FOOD].val_ = 100.0;
    worker->AddToInventory(ItemTypes::CORN, 2);
    // worker->AddToInventory(ItemTypes::PLANTBASKET, 1);
    dynamic_object_ptrs_.push_back(worker);
}
void Map::SetAttention(DynamicObjectPtr ptr, double x, double y){
    // first clear attention
    ptr->ClearAttention();

    // get tile int x, y
    auto tile = GetTile(x, y);

    // check if tile is nullptr
    if (tile == nullptr)
        return;

    // set selected tile ptr
    ptr->SetAttention(tile);

    if (tile->HasBuildingPtr())
    {
        ptr->SetAttention(tile->GetBuildingPtr());
    } 
    else if (tile->HasResource())
    {
        ptr->SetAttention(tile->GetResource());
    }
}
void Map::SetAttention(double x, double y){
    // Implication: for all dynamic objects
    for (auto &dynamic_object_ptr : selected_dynamic_object_ptrs_)
    {
        ClearAttention(dynamic_object_ptr);
        SetAttention(dynamic_object_ptr, x, y);
    }
}
void Map::ClearAttention(DynamicObjectPtr ptr){
    ptr->ResetDynamicObjectActions();
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
void Map::ClearState(DynamicObjectPtr ptr){
    if (ptr->IsType(DynamicObjectTypes::WORKER)){
        std::static_pointer_cast<Worker>(ptr)->ClearState();
    }
}
void Map::SetAttentionAndMove(DynamicObjectPtr ptr, double x, double y){
    // set attention and move
    ClearState(ptr);
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
std::vector<TilePtr> Map::GetEuclNeighbors(TilePtr tile_ptr){
    // get euclidean neighbors
    std::vector<TilePtr> neighbors;
    int x = tile_ptr->x_;
    int y = tile_ptr->y_;
    
    // get the four euclidean neighbors
    auto tile = GetTile(x-1, y);
    if (tile != nullptr){
        neighbors.push_back(tile);
    }
    tile = GetTile(x+1, y);
    if (tile != nullptr){
        neighbors.push_back(tile);
    }
    tile = GetTile(x, y-1);
    if (tile != nullptr){
        neighbors.push_back(tile);
    }
    tile = GetTile(x, y+1);
    if (tile != nullptr){
        neighbors.push_back(tile);
    }
    return neighbors;
}
std::vector<TilePtr> Map::GetNeighbors(TilePtr tile_ptr){
    // get neighbors
    std::vector<TilePtr> neighbors;
    int x = tile_ptr->x_;
    int y = tile_ptr->y_;
    
    // iterate -1 to 1
    for (int i=-1; i<2; i++){
        for (int j=-1; j<2; j++){
            // skip if i, j is 0, 0
            if (i == 0 && j == 0){
                continue;
            }
            // get the tile
            auto tile = GetTile(x+i, y+j);
            if (tile != nullptr){
                neighbors.push_back(tile);
            }
        }
    }
    return neighbors;
}
void Map::SetTask(WorkerPtr ptr, TaskPtr task_ptr){
    ptr->SetTask(task_ptr);
};
void Map::SetState(WorkerPtr ptr, WorkerStates state){
    ptr->SetState(state);
};

void Map::SetTasks(TaskPtr task_ptr){
    // set tasks for all selected dynamic objects
    for (auto &dynamic_object_ptr : selected_dynamic_object_ptrs_)
    {
        // check if worker and cast to worker
        if (dynamic_object_ptr->dynamic_object_type_ != DynamicObjectTypes::WORKER){
            continue;
        }
        auto worker_ptr = std::static_pointer_cast<Worker>(dynamic_object_ptr);
        SetTask(worker_ptr, task_ptr);
    }
}
void Map::SetStates(WorkerStates state){
    // set states for all selected dynamic objects
    for (auto &dynamic_object_ptr : selected_dynamic_object_ptrs_)
    {
        // check if worker and cast to worker
        if (dynamic_object_ptr->dynamic_object_type_ != DynamicObjectTypes::WORKER){
            continue;
        }
        auto worker_ptr = std::static_pointer_cast<Worker>(dynamic_object_ptr);
        SetState(worker_ptr, state);
    }
}
ActionPtr Map::MakeAction(ActionTypes action_type){
    auto action = std::make_shared<Action>(action_type, GetSelectRect());
    // add the tile at mouse position
    action->SetTile(GetTile());
    return action;
}

        // // make the task
        // auto task_ptr = std::make_shared<Task>(to_string(action_type) + " task");
        // // set the tasks
        // SetTasks(task_ptr);
        // // set states
        // SetStates(WorkerStates::EXECUTINGTASK);
void Map::SetOnSelectToSelectUnits(){
    // setup onSelect cb
    SetOnSelectCB([this](){
        // select the mouse objects
        SelectMouseObjects();
    });
}
void Map::SetOnSelectToSetTask(ActionTypes action_type){
    // setup onSelect cb
    SetOnSelectCB([this, action_type](){
        // make the task
        auto task_ptr = std::make_shared<Task>(to_string(action_type) + " task");
        // add to task mgr easy edit task
        task_ptr->AddAction(MakeAction(action_type));
        // set the tasks
        SetTasks(task_ptr);
        // set states
        SetStates(WorkerStates::EXECUTINGTASK);
        // reset the onSelect cb
        SetOnSelectCB(std::bind(&Map::SelectMouseObjects, this));
    });
}
void Map::SetOnSelectToAddAction(ActionTypes action_type){
    // setup onSelect cb
    SetOnSelectCB([this, action_type](){
        // add to task mgr easy edit task
        task_manager_ptr_->easy_edit_task_->AddAction(MakeAction(action_type));
    });
}
void Map::SetOnSelectFromContext(ActionTypes action_type){
    if (task_manager_ptr_->in_easy_edit_mode_){
        SetOnSelectToAddAction(action_type);
    } else {
        SetOnSelectToSetTask(action_type);
    }
}
std::shared_ptr<Rect<double>> Map::GetSelectRect(){
    auto x = mouse_down_x_;
    auto y = mouse_down_y_;
    auto width = mouse_x_ - mouse_down_x_;
    auto height = mouse_y_ - mouse_down_y_;
    if (width < 0){
        width = -width;
        x = mouse_x_;
    }
    if (height < 0){
        height = -height;
        y = mouse_y_;
    }

    return std::make_shared<Rect<double>>(x, y, width, height);
}
void Map::InferAction(){
    if (task_manager_ptr_->in_easy_edit_mode_){
        // SetOnSelectToAddAction(); // right now this is done in the task manager handle input fcn
    } else {
        // SetAttentionAndMove(mouse_x_, mouse_y_);

        // loop through each selected dynamic object
        for (auto &dynamic_object_ptr : selected_dynamic_object_ptrs_)
        {
            // infer action for each dynamic object
            InferAction(dynamic_object_ptr, mouse_x_, mouse_y_);
        }
    }
}
void Map::InferAction(DynamicObjectPtr ptr, double x, double y){
    // gives this dynamic object a reference to the tile at that location
    SetAttention(ptr, x, y); 

    // now, we can infer action
    ptr->InferAction(b_append_actions);
}
    /////////////////////////////////////// End Map ///////////////////////////////////////