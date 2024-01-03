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
std::vector<ResourcePtr> Map::GetResourceSlice(Rect<double> rect){
    std::vector<ResourcePtr> resources;

    // get a slice of the map
    auto tiles = GetTileSlice(rect);

    // iterate over tiles
    for (auto& tile : tiles){
        // iterate through resources
        for(auto& resource : tile->resource_map_){
            ResourceTypes resource_type = resource.second->resource_type_;
            resources.push_back(resource.second);
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
            for (auto input: GetActiveRecipe()->inputs_){
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
                for (auto& input : GetActiveRecipe()->inputs_){
                    double reqd_amount = input.second;
                    // remove from inventory
                    AddToInventory(input.first, -reqd_amount);
                }
                // set building status to operating
                building_status_ = BuildingStatus::OPERATING;
            }
            break;}
        case BuildingStatus::OPERATING:
            if (effort_val_ > GetActiveRecipe()->effort_req_){
                // add outputs to inventory
                for (auto& output : GetActiveRecipe()->outputs_){
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