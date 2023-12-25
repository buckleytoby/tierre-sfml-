#include "gameplay.hpp"

// Hard Coded Enum Names
const std::string to_string(ResourceTypes p){
  switch(p)
  {
    case ResourceTypes::LUMBER: return "L";
    case ResourceTypes::STONE: return "S";
    case ResourceTypes::WATER: return "W";
    case ResourceTypes::GRASS: return "G";

  }
  return ""; // or an empty string
}
const sf::Color to_color(ResourceTypes p){
  switch(p)
  {
    case ResourceTypes::LUMBER: return sf::Color::Red;
    case ResourceTypes::STONE: return sf::Color::Yellow;
    case ResourceTypes::WATER: return sf::Color::Blue;
    case ResourceTypes::GRASS: return sf::Color::Green;

  }
  return sf::Color::White;
}
const std::string to_string(WorkerStates p){
  switch(p)
  {
    case WorkerStates::IDLE: return "idle";
    case WorkerStates::MOVING: return "moving";
    case WorkerStates::GATHERIDLE: return "gatheridle";
    case WorkerStates::GATHERING: return "gathering";
    case WorkerStates::BUILDING: return "building";

  }
  return ""; // or an empty string
}

/////////////////////////////////////// GamePlay ///////////////////////////////////////
// write class methods
GamePlay::GamePlay(){
    // constructor

    // Make the map
    map_.SetWidth(100);
    map_.SetHeight(100);
    map_.CreateTiles();

    // Set the viewport
    viewport_.SetX(0);
    viewport_.SetY(0);
    viewport_.SetWidth(19);
    viewport_.SetHeight(10);

    // hard code some tiles
    Tile tile, tile2;
    Lumber* lumber = new Lumber(100.0); // kg
    Stone* stone = new Stone(100.0);
    std::shared_ptr<Resource> resource1(lumber);
    std::shared_ptr<Resource> resource2(stone);
    tile.AddResource(resource1);
    tile2.AddResource(resource2);
    map_.SetTile(5, 5, tile);
    map_.SetTile(5, 6, tile);
    map_.SetTile(5, 7, tile);
    
    map_.SetTile(6, 5, tile2);
    map_.SetTile(6, 6, tile2);
    map_.SetTile(6, 7, tile2);

    // print out the map
    for (int i=0; i<10; i++){
        for (int j=0; j<10; j++){
            // iterate through resources
            for (auto& resource : map_.GetTile(i, j)->resource_map_){
                std::cout << to_string(resource.second->resource_type_) << " total amount: " << resource.second->GetTotal();
            }
        }
        std::cout << std::endl;
    }

    // hard code add a worker
    Worker* worker = new Worker();
    worker->SetX(5);
    worker->SetY(5);
    worker->SetSpeed(1);
    map_.dynamic_object_ptrs_.push_back(std::unique_ptr<DynamicObject>((DynamicObject*)worker));

}
GamePlay::~GamePlay(){
    // destructor
}
void GamePlay::draw(sf::RenderWindow& window){
    // draw the gameplay
    sf::Font font;
    if (!font.loadFromFile("c:\\Windows\\Fonts\\arial.ttf"))
    {
        return;
    }

    // update the title screen
    sf::Text text;
    text.setFont(font); // font is a sf::Font
    text.setString("Press ESC to go back to Title Screen");
    text.setCharacterSize(24); // in pixels, not points!
    // text.setOrigin(0, -5); // moves the text down 5 pixels
    text.setPosition(10, 10);
    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    window.draw(text);

    // iterate through the viewport, size + 1 to draw the tiles on the edge
    for (int i=viewport_.GetIntX(); i<viewport_.GetIntWidth()+1; i++){
        for (int j=viewport_.GetIntY(); j<viewport_.GetIntHeight()+1; j++){
            // get the tile from map_
            auto tile = map_.GetTile(i, j);
            // skip if no tile (including OOB)
            if (tile == nullptr){
                continue;
            }       

            // get the string
            std::string str = "";
            sf::Color color(0, 0, 0);
            // iterate through resources
            for (auto& resource : map_.GetTile(i, j)->resource_map_){
                str += to_string(resource.second->resource_type_);
                color = to_color(resource.second->resource_type_);
            }
            if (str == ""){
                continue;
            }

            // calculate pixel position for i, j
            double pixelX = viewport_.ConvertMeterToPixelX(window, i);
            double pixelY = viewport_.ConvertMeterToPixelY(window, j);
            double pixelsPerMeterW = viewport_.GetPixelsPerMeterX(window);
            double pixelsPerMeterH = viewport_.GetPixelsPerMeterY(window);  

            // draw the tile
            sf::Text text;
            text.setFont(font);
            text.setString(str);
            text.setPosition(pixelX, pixelY);
            text.setOrigin(0, pixelsPerMeterH); // set origin to bottom left
            text.setCharacterSize(pixelsPerMeterH); // pixels
            // text.setScale(pixelsPerMeterW, pixelsPerMeterH); // isn't working well
            text.setFillColor(color);
            text.setStyle(sf::Text::Bold);
            window.draw(text);
        }
    }

    // iterate through dynamic game objects
    for (auto& dynamic_object_ptr : map_.dynamic_object_ptrs_){
        // calculate pixel position for i, j
        double pixelX = viewport_.ConvertMeterToPixelX(window, dynamic_object_ptr->footprint_.x_);
        double pixelY = viewport_.ConvertMeterToPixelY(window, dynamic_object_ptr->footprint_.y_);   
        double pixelsPerMeterH = viewport_.GetPixelsPerMeterY(window);         

        // draw the tile
        sf::Text text;
        text.setFont(font);
        text.setString("W");
        text.setPosition(pixelX, pixelY);
        text.setCharacterSize(pixelsPerMeterH); // pixels
        text.setOrigin(0, pixelsPerMeterH); // set origin to bottom left
        // text.setScale(pixelsPerMeterW, pixelsPerMeterH); // isn't working well
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
        window.draw(text);
    }

    // HUD
    // HUD outline
    sf::RectangleShape shape(sf::Vector2f(280, 130));
    shape.setPosition(15.0, window.getSize().y - 150);
    shape.setFillColor(sf::Color::Black);
    // set a 10-pixel wide orange outline
    shape.setOutlineThickness(10.f);
    shape.setOutlineColor(sf::Color::Cyan);
    window.draw(shape);
    // end HUD outline

    // make string for selected unit
    for (auto& dynamic_object_ptr : map_.selected_dynamic_object_ptrs_){
        std::string str = "Selected Unit: ";
        switch (dynamic_object_ptr->dynamic_object_type_){
            case DynamicObjectTypes::WORKER:
                str += "Worker";
                // add inventory
                str += "\nInventory: ";
                for (auto& resource : std::static_pointer_cast<Worker>(dynamic_object_ptr)->inventory_map_){
                    str += to_string(resource.first);
                    str += ": ";
                    str += std::to_string(resource.second.GetAmount());
                    str += ", ";
                }
                // add state
                str += "\nState: ";
                str += to_string(std::static_pointer_cast<Worker>(dynamic_object_ptr)->worker_state_);
                break;
            case DynamicObjectTypes::GENERIC:
                str += "Generic";
                break;
            default:
                break;
        }
        // add position
        str += "\nPos: " + std::to_string(dynamic_object_ptr->footprint_.x_);
        str += ", ";
        str += std::to_string(dynamic_object_ptr->footprint_.y_);
        // draw the string
        sf::Text text;
        text.setFont(font);
        text.setString(str);
        text.setPosition(20, window.getSize().y - 150 + 10);
        text.setCharacterSize(12); // pixels
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
        window.draw(text);
    }
    // end make string for selected unit

    

}
void GamePlay::update(double dt){
    // update the gameplay
    // update the map
    map_.update(dt);

    // update the viewport
    viewport_.update(dt);
}
BitFlag GamePlay::handleInput(sf::Event& event){
    BitFlag handleInputFlags;
    // handle input for the title screen
    switch (event.type){
        ///////////////// key pressed /////////////////
        case sf::Event::KeyPressed:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::Escape:
                    std::cout << "Change active gamescreen TitleScreen" << std::endl;
                    handleInputFlags.SetFlag((EFlagValue)HandleInputActions::CHANGEACTIVEGAMESCREENTITLESCREEN);
                    break;
                case sf::Keyboard::Scan::A:
                    std::cout << "Move viewport left" << std::endl;
                    viewport_.viewport_actions_.SetFlag((EFlagValue)ViewportActions::SCROLL_LEFT);
                    break;
                case sf::Keyboard::Scan::D:
                    std::cout << "Move viewport right" << std::endl;
                    viewport_.viewport_actions_.SetFlag((EFlagValue)ViewportActions::SCROLL_RIGHT);
                    break;
                case sf::Keyboard::Scan::W:
                    std::cout << "Move viewport up" << std::endl;
                    viewport_.viewport_actions_.SetFlag((EFlagValue)ViewportActions::SCROLL_UP);
                    break;
                case sf::Keyboard::Scan::S:
                    std::cout << "Move viewport down" << std::endl;
                    viewport_.viewport_actions_.SetFlag((EFlagValue)ViewportActions::SCROLL_DOWN);
                    break;
                default:
                    break;
            }
            break;
        ///////////////// key released /////////////////
        case sf::Event::KeyReleased:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::A:
                    std::cout << "Stop moving viewport left" << std::endl;
                    viewport_.viewport_actions_.UnsetFlag((EFlagValue)ViewportActions::SCROLL_LEFT);
                    break;
                case sf::Keyboard::Scan::D:
                    std::cout << "Stop moving viewport right" << std::endl;
                    viewport_.viewport_actions_.UnsetFlag((EFlagValue)ViewportActions::SCROLL_RIGHT);
                    break;
                case sf::Keyboard::Scan::W:
                    std::cout << "Stop moving viewport up" << std::endl;
                    viewport_.viewport_actions_.UnsetFlag((EFlagValue)ViewportActions::SCROLL_UP);
                    break;
                case sf::Keyboard::Scan::S:
                    std::cout << "Stop moving viewport down" << std::endl;
                    viewport_.viewport_actions_.UnsetFlag((EFlagValue)ViewportActions::SCROLL_DOWN);
                    break;
                case sf::Keyboard::Scan::G:
                    std::cout << "Toggle gathering" << std::endl;
                    for (auto& dynamic_object_ptr : map_.selected_dynamic_object_ptrs_){
                        if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::WORKER){
                            // set worker state to gathering
                            auto worker = std::static_pointer_cast<Worker>(dynamic_object_ptr);
                            worker->SetState(WorkerStates::GATHERIDLE);
                        }
                    }
                default:
                    break;
            }
            break;
        ///////////////// MouseWheelScrolled /////////////////
        case sf::Event::MouseWheelScrolled:
            std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
            std::cout << "mouse x: " << event.mouseWheelScroll.x << std::endl;
            std::cout << "mouse y: " << event.mouseWheelScroll.y << std::endl;
            if (event.mouseWheelScroll.delta > 0){
                std::cout << "Zoom in" << std::endl;
                viewport_.viewport_actions_.SetFlag((EFlagValue)ViewportActions::ZOOM_IN_ONCE);
            } else {
                std::cout << "Zoom out" << std::endl;
                viewport_.viewport_actions_.SetFlag((EFlagValue)ViewportActions::ZOOM_OUT_ONCE);
            }
            break;
        ///////////////// MouseClicked /////////////////
        case sf::Event::MouseButtonReleased:
            std::cout << "mouse button: " << event.mouseButton.button << std::endl;
            std::cout << "mouse x: " << event.mouseButton.x << std::endl;
            std::cout << "mouse y: " << event.mouseButton.y << std::endl;
            if (event.mouseButton.button == sf::Mouse::Left){
                std::cout << "Left mouse button released" << std::endl;
                // select a dynamic object
                map_.SelectDynamicObject(viewport_.ConvertPixelToMeterX(event.mouseButton.x), viewport_.ConvertPixelToMeterY(event.mouseButton.y));
                // print dynamic object details
                for (auto& dynamic_object_ptr : map_.selected_dynamic_object_ptrs_){
                    std::cout << "Selected dynamic object: " << dynamic_object_ptr->footprint_.x_ << ", " << dynamic_object_ptr->footprint_.y_ << std::endl;
                }
            }
            if (event.mouseButton.button == sf::Mouse::Right){
                std::cout << "Right mouse button released" << std::endl;
                // move the selected dynamic object
                for (auto& dynamic_object_ptr : map_.selected_dynamic_object_ptrs_){
                    if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::WORKER){
                        // set worker state to moving
                        auto worker = std::static_pointer_cast<Worker>(dynamic_object_ptr);
                        worker->SetState(WorkerStates::MOVING);
                        worker->SetGoal(viewport_.ConvertPixelToMeterX(event.mouseButton.x), viewport_.ConvertPixelToMeterY(event.mouseButton.y));
                    }
                }
            } 
            break;
        ///////////////// Default /////////////////
        default:
            break;
    } //////////////// END event.type switch /////////////////

    return handleInputFlags;
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
        std::vector<Tile> tiles_array_row;
        for (int j=0; j<height_; j++){
            Tile tile;
            tile.x_ = i;
            tile.y_ = j;
            tiles_array_row.push_back(tile);
        }
        tiles_array_.push_back(tiles_array_row);
    }
}
void Map::SetTile(int x, int y, Tile tile){
    // set a tile on the map
    tiles_array_[x][y] = tile;
    tiles_array_[x][y].x_ = x;
    tiles_array_[x][y].y_ = y;
}

void Map::GetMapSlice(int x, int y, int width, int height){
    // get a slice of the map
    // TODO
}
std::map<ResourceTypes, std::shared_ptr<Resource>> Map::GetResourceSlice(Rect<double> rect){
    // get a slice of the map
    int min_x = floor(rect.x_);
    int min_y = floor(rect.y_);
    int max_x = ceil(rect.x_ + rect.width_);
    int max_y = ceil(rect.y_ + rect.height_);

    std::map<ResourceTypes, std::shared_ptr<Resource>> resources;

    // iterate from min to max x, y
    for(int x = min_x; x < max_x; x++){
        for(int y = min_y; y < max_y; y++){
            Tile* tile;
            tile = GetTile(x, y);
            if (tile != nullptr){
                // iterate through resources
                for(auto& resource : tile->resource_map_){
                    ResourceTypes resource_type = resource.second->resource_type_;
                    resources[resource_type] = resource.second;
                }
            }
        }
    }
    return resources;
}

Tile* Map::GetTile(int x, int y){
    // get a tile from the map
    // check x, y in bounds
    if (x < 0 || x >= width_ || y < 0 || y >= height_){
        return nullptr;
    } else {
        return &tiles_array_[x][y];
    }
}
void Map::update(double dt){
    // update the map
    // update dynamic objects
    for (auto& dynamic_object_ptr : dynamic_object_ptrs_){
        dynamic_object_ptr->update(dt);

        // if worker
        if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::WORKER){
            auto worker = std::static_pointer_cast<Worker>(dynamic_object_ptr);
            // update senses
            auto resources = GetResourceSlice(worker->GetImmediateSurroundingsRect());
            worker->immediate_surroundings_.SetResource(resources);
            resources = GetResourceSlice(worker->GetNearbySurroundingsRect());
            worker->nearby_surroundings_.SetResource(resources);
        }
    }
}
void Map::SelectDynamicObject(double x, double y){
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
void Worker::update(double dt){
    // update the worker
    inherited::update(dt);

    // AI
    AI(dt);
}
void Worker::AI(double dt){
    // AI for the worker
    // clear actions
    switch (worker_state_){
        case WorkerStates::IDLE:
            // do nothing
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
        case WorkerStates::BUILDING:
            // build
            break;
    }
}
void Worker::SetGoal(double x, double y){
    // set the goal for the worker
    goal_.x_ = x;
    goal_.y_ = y;
}
void Worker::AddToInventory(ResourceTypes itemType, double amount){
    // add to the inventory
    // check if itemType in inventory_map_
    if (inventory_map_.find(itemType) != inventory_map_.end()){
        // add to the resource
        inventory_map_[itemType].AddAmount(amount);
    } else {
        // add the resource
        inventory_map_[itemType] = ResourceItem(itemType, amount);
    }
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
void Worker::MoveTowardsGoal(){
    dynamic_object_actions_.ClearFlag();
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
        worker_state_ = WorkerStates::IDLE;
    }
}
void Worker::Gather(double dt){
    // gather resources

    // if not gathering, decide which resource to gather and set gather_time_ to zero
    // get first resource in immediatesurroundings
    if (worker_state_ == WorkerStates::GATHERIDLE){
        if (immediate_surroundings_.resources_.size() > 0){
            gather_resource_ptr_ = immediate_surroundings_.resources_.begin()->second;
            gather_time_ = 0.0;
            SetState(WorkerStates::GATHERING);
        } else {
            // nothing close enough to gather
            std::cout << "Nothing close enough to gather" << std::endl;
            SetState(WorkerStates::IDLE);
            return;
        }
    }

    // update gather time
    gather_time_ += dt;

    // check if gather time is greater than resource time_to_gather_
    if (gather_time_ > gather_resource_ptr_->time_to_gather_){
        // get min of GetTotal & yield_per_gather_
        double amount = std::min(gather_resource_ptr_->GetTotal(), gather_resource_ptr_->yield_per_gather_);

        // add resource to inventory
        AddToInventory(gather_resource_ptr_->resource_type_, amount);
        // remove resource from tile
        gather_resource_ptr_->RemoveTotal(amount);
        // check if resource is empty
        if (gather_resource_ptr_->GetTotal() <= 0){
            // remove resource from tile
            gather_resource_ptr_->RemoveTotal(gather_resource_ptr_->GetTotal());
            // remove resource from immediate surroundings
            immediate_surroundings_.resources_.erase(gather_resource_ptr_->resource_type_);
        }
        // set gather time to zero
        gather_time_ = 0.0;

        // check if resource is exhausted
        if (gather_resource_ptr_->GetTotal() <= 0){
            // set state to GATHERIDLE
            SetState(WorkerStates::GATHERIDLE);
        }
    }



}
/////////////////////////////////////// End Worker ///////////////////////////////////////