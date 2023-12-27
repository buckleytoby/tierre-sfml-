#include "gameplay.hpp"




// Hard Coded Enum Names
const std::string to_full_name(ResourceTypes p){
    switch(p)
    {
        case ResourceTypes::LUMBER: return "Lumber";
        case ResourceTypes::STONE: return "Stone";
        case ResourceTypes::WATER: return "Water";
        case ResourceTypes::GRASS: return "Grass";
        case ResourceTypes::CORN: return "Corn";
    
    }
    return ""; // or an empty string
    
}
const std::string to_string(ResourceTypes p){
  switch(p)
  {
    case ResourceTypes::LUMBER: return "L";
    case ResourceTypes::STONE: return "S";
    case ResourceTypes::WATER: return "W";
    case ResourceTypes::GRASS: return "G";
    case ResourceTypes::CORN: return "C";

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
const std::string to_string(ItemTypes p){
    switch(p)
    {
        case ItemTypes::NONE: return "";
    }
    return ""; // or an empty string
}
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
const std::string to_string(BuildingTypes p){
  switch(p)
  {
    case BuildingTypes::NONE: return "";
    case BuildingTypes::WORKSPACE: return "Ws";
    case BuildingTypes::FARM: return "Fa";
  }
  return ""; // or an empty string
}
const std::string to_full_string(BuildingTypes p){
  switch(p)
  {
    case BuildingTypes::NONE: return "";
    case BuildingTypes::WORKSPACE: return "Workspace";
    case BuildingTypes::FARM: return "Farm";
  }
  return ""; // or an empty string
}
const std::string to_full_string(BuildingStatus p){
    switch(p)
    {
        case BuildingStatus::PRECONSTRUCTION: return "Preconstruction";
        case BuildingStatus::CONSTRUCTION: return "Construction";
        case BuildingStatus::READY: return "Ready";
        case BuildingStatus::OPERATING: return "Operating";
    }
    return ""; // or an empty string
}
const std::string to_full_string(ItemTypes p){
    switch(p)
    {
        case ItemTypes::NONE: return "";
        case ItemTypes::CORNSEED: return "Corn Seed";
        case ItemTypes::LUMBER: return "Lumber";
        case ItemTypes::STONE: return "Stone";
        case ItemTypes::WATER: return "Water";
        case ItemTypes::GRASS: return "Grass";
        case ItemTypes::CORN: return "Corn";
    }
    return ""; // or an empty string
}
const std::string to_full_string(RecipeTypes p){
    switch(p)
    {
        case RecipeTypes::NONE: return "";
        case RecipeTypes::PROCESSCORNSTALK: return "Process Cornstalk";
        case RecipeTypes::FARMCORN: return "Farm Corn";
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
    auto tile = std::make_shared<Tile>();
    auto tile2 = std::make_shared<Tile>();
    auto tile3 = std::make_shared<Tile>();
    auto lumber = std::make_shared<Lumber>(100.0); 
    auto stone = std::make_shared<Stone>(100.0);
    auto corn = std::make_shared<CornResource>(100.0);
    std::shared_ptr<Resource> resource1(lumber);
    std::shared_ptr<Resource> resource2(stone);
    tile->AddResource(resource1);
    tile2->AddResource(resource2);
    tile3->AddResource(std::shared_ptr<Resource>(corn));
    map_.SetTile(4, 4, tile3);

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
    worker->needs_map_[NeedsTypes::FOOD].val_ = 100.0;
    // give worker starting inventory of corn
    worker->AddToInventory(ItemTypes::CORN, 2);

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

            // check for building
            if (tile->building_type_ != BuildingTypes::NONE){
                str += to_string(tile->building_type_);
                color = sf::Color::White;
            } else {
                // iterate through resources
                for (auto& resource : map_.GetTile(i, j)->resource_map_){
                    str += to_string(resource.second->resource_type_);
                    color = to_color(resource.second->resource_type_);
                }
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
    sf::RectangleShape shape(sf::Vector2f(300, window.getSize().y - 100));
    shape.setPosition(15.0, 110);
    shape.setFillColor(sf::Color::Black);
    // set a 10-pixel wide orange outline
    shape.setOutlineThickness(10.f);
    shape.setOutlineColor(sf::Color::Cyan);
    window.draw(shape);
    // end HUD outline

    // make string for selected unit
    std::string str = "";
    for (auto& dynamic_object_ptr : map_.selected_dynamic_object_ptrs_){
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
                str += "\nGathering " + to_full_name(worker->selected_resource_ptr_->resource_type_) + " Progress: ";
                str += std::to_string(worker->action_time_) + " / " + std::to_string(worker->selected_resource_ptr_->time_to_gather_);
            }
            // if executing task, add task progress
            else if (worker->worker_state_ == WorkerStates::EXECUTINGTASK){
                str += "\nExecuting Task Progress: \n";
                for (int i=0; i<worker->task_steps_.size(); i++){
                    if (i == worker->task_current_step_){
                        str += ">> ";
                    }
                    str += worker->task_steps_[i] + "\n";
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
            break;
        }
        // add position
        str += "\nPos: " + std::to_string(dynamic_object_ptr->footprint_.x_);
        str += ", ";
        str += std::to_string(dynamic_object_ptr->footprint_.y_);
    }
    // end make string for selected unit

    // make string for selected tile
    if (map_.selected_tile_ptr_ != nullptr){
        str += "Selected Tile: ";
        // add position
        str += "\nPos: " + std::to_string(map_.selected_tile_ptr_->x_);
        str += ", ";
        str += std::to_string(map_.selected_tile_ptr_->y_);
        // add resources
        str += "\nResources: ";
        for (auto& resource : map_.selected_tile_ptr_->resource_map_){
            str += to_full_name(resource.second->resource_type_);
            str += ": ";
            str += std::to_string(resource.second->GetTotal());
            str += ", ";
        }
        // add building
        if (map_.selected_tile_ptr_->building_type_ != BuildingTypes::NONE){
            str += "\nBuilding: ";
            str += to_full_string(map_.selected_tile_ptr_->building_type_);
            str += "\nBuilding Status: " + to_full_string(map_.selected_tile_ptr_->building_ptr_->GetStatus());
            str += "\nInventory: ";
            for (auto& item : map_.selected_tile_ptr_->building_ptr_->inventory_map_){
                str += to_full_string(item.first);
                str += ": ";
                str += std::to_string(item.second);
                str += ", ";
            }
            // switch over building status
            switch (map_.selected_tile_ptr_->building_ptr_->GetStatus()){
                case BuildingStatus::PRECONSTRUCTION:
                    // iterate through item_reqs_map
                    str += "\nItem Reqs: ";
                    for (auto& item : map_.selected_tile_ptr_->building_ptr_->item_reqs_map_){
                        str += to_full_string(item.first);
                        str += ": ";
                        str += std::to_string(item.second);
                        str += ", ";
                    }
                    break;
                case BuildingStatus::CONSTRUCTION:
                    str += "\nConstruction Effort: " + std::to_string(map_.selected_tile_ptr_->building_ptr_->effort_val_);
                    str += " / " + std::to_string(map_.selected_tile_ptr_->building_ptr_->construction_effort_req_);
                    break;
                case BuildingStatus::READY:
                    str += "\n Recipe " + to_full_string(map_.selected_tile_ptr_->building_ptr_->active_recipe_) + " Reqs: ";
                    for (auto& item : map_.selected_tile_ptr_->building_ptr_->recipes_[map_.selected_tile_ptr_->building_ptr_->active_recipe_].inputs_){
                        str += to_full_string(item.first);
                        str += ": ";
                        str += std::to_string(item.second);
                        str += ", ";
                    }
                    break;
                case BuildingStatus::OPERATING:
                    str += "\n Recipe Progress: " + std::to_string(map_.selected_tile_ptr_->building_ptr_->effort_val_);
                    str += " / " + std::to_string(map_.selected_tile_ptr_->building_ptr_->recipes_[map_.selected_tile_ptr_->building_ptr_->active_recipe_].effort_req_);
                    break;
            }

        }
    }
    
    // draw the string
    text.setFont(font);
    text.setString(str);
    text.setPosition(20, 130);
    text.setCharacterSize(12); // pixels
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    window.draw(text);

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
                    break;
                case sf::Keyboard::Scan::C:
                    std::cout << "Toggle constructing" << std::endl;
                    for (auto& dynamic_object_ptr : map_.selected_dynamic_object_ptrs_){
                        if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::WORKER){
                            // set worker state to constructing
                            auto worker = std::static_pointer_cast<Worker>(dynamic_object_ptr);
                            worker->SetState(WorkerStates::CONSTRUCTINGIDLE);
                        }
                    }
                    break;
                case sf::Keyboard::Scan::R:
                    std::cout << "Toggle crafting" << std::endl;
                    for (auto& dynamic_object_ptr : map_.selected_dynamic_object_ptrs_){
                        if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::WORKER){
                            // set worker state to crafting
                            auto worker = std::static_pointer_cast<Worker>(dynamic_object_ptr);
                            worker->SetState(WorkerStates::CRAFTINGIDLE);
                        }
                    }
                    break;
                case sf::Keyboard::Scan::T:
                    std::cout << "Transfer Inventory" << std::endl;
                    for (auto& dynamic_object_ptr : map_.selected_dynamic_object_ptrs_){
                        if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::WORKER){
                            auto worker = std::static_pointer_cast<Worker>(dynamic_object_ptr);
                            worker->TransferInventory();
                        }
                    }
                    break;
                case sf::Keyboard::Scan::E:
                    std::cout <<"Execute Task" << std::endl;
                    for (auto& dynamic_object_ptr : map_.selected_dynamic_object_ptrs_){
                        if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::WORKER){
                            auto worker = std::static_pointer_cast<Worker>(dynamic_object_ptr);
                            worker->SetState(WorkerStates::EXECUTINGTASK);
                        }
                    }
                    break;
                case sf::Keyboard::Scan::Num1:{
                    std::cout << "Build workspace" << std::endl;
                    // get mouse position
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(*viewport_.window_ptr_);
                    // create a workspace
                    map_.MakeBuilding(BuildingTypes::WORKSPACE, viewport_.ConvertPixelToMeterX(mousePosition.x), viewport_.ConvertPixelToMeterY(mousePosition.y));
                    break;}
                case sf::Keyboard::Scan::Num2:{
                    std::cout << "Build farm" << std::endl;
                    // get mouse position
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(*viewport_.window_ptr_);
                    // create a farm
                    map_.MakeBuilding(BuildingTypes::FARM, viewport_.ConvertPixelToMeterX(mousePosition.x), viewport_.ConvertPixelToMeterY(mousePosition.y));
                    break;}
                case sf::Keyboard::Scan::Num0:{
                    std::cout << "Make worker" << std::endl;
                    // get mouse position
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(*viewport_.window_ptr_);
                    // create a worker
                    map_.MakeWorker(viewport_.ConvertPixelToMeterX(mousePosition.x), viewport_.ConvertPixelToMeterY(mousePosition.y));
                    break;}
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
                map_.SelectObject(viewport_.ConvertPixelToMeterX(event.mouseButton.x), viewport_.ConvertPixelToMeterY(event.mouseButton.y));
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
            // do nothing
            break;
        case WorkerStates::EXECUTINGTASK:
            active_script_fcn_(dt);
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
void Worker::SelectClosestBuilding(){
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
}
void Worker::SetGoalToSelectedBuilding(){
    // set the goal to the selected building
    if (selected_building_ptr_ == nullptr){
        return;
    }
    // auto success
    SetState(WorkerStates::IDLE);
    goal_.x_ = selected_building_ptr_->center_.x_;
    goal_.y_ = selected_building_ptr_->center_.y_;
}

void Worker::MoveTowardsGoal(){
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

    // switch case on building status
    switch (selected_building_ptr_->building_status_){
        case BuildingStatus::PRECONSTRUCTION:
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
            break;
        case BuildingStatus::CONSTRUCTION:
            // update construction effort
            selected_building_ptr_->effort_val_ += skill_map_[SkillTypes::CONSTRUCTION].CalcEffortUnits();
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
void Worker::TransferInventoryToClosestBuilding(double dt){
    // This is the very first "script" function.
    // It is supposed to be called from the UPDATE function, so at FRAMERATE times per second.
    // Therefore, any function which has a success criteria (e.g. MoveTowardsGoal) cannot be blocking.
    // Must maintain a semi-permanent meta-structure which tracks the steps in the task,
    // sets the worker state, increments through the task steps, and finally restarts

    // EXECUTINGTASK should be a separate worker state, then, which will be called by the UPDATE function
    // and ultimately enters here. The task manager should then take ownership, set the appropriate worker state
    // so that actions can proceed as normal, and before the function returns revert the state back to EXECUTINGTASK
    // to set up for the next loop.

    // User will have to press a dedicated 'execute script' button and can break out of it at anytime by providing a different command
    // ex: right clicking to move

    std::function<void()> fcn;
    // reset low-level actions
    ResetDynamicObjectActions();

    int i = task_current_step_;

    auto step = task_steps_[i];
    auto start_state = task_start_states[i];

    SetState(start_state);
    fcn = action_primitive_map_[step];
    fcn();

    // check if success
    if (CheckState(WorkerStates::IDLE)){
        // increment task_current_step_
        task_current_step_ = (task_current_step_ + 1) % task_steps_.size();
    }

    // Revert to executing task. Bit of a hack.
    SetState(WorkerStates::EXECUTINGTASK);
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