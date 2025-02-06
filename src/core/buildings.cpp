#include "buildings.hpp"
int Building::id_counter_ = 0;

const std::string to_string(BuildingTypes p){
  switch(p)
  {
    case BuildingTypes::NONE: return "";
    case BuildingTypes::WORKSPACE: return "Workspace";
    case BuildingTypes::STORAGESPACE: return "Storagespace";
    case BuildingTypes::CHEST: return "Chest";
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
const sf::Color to_color(BuildingPtr p){
    switch(p->building_status_)
    {
        case BuildingStatus::PRECONSTRUCTION: return sf::Color::Red;
        case BuildingStatus::CONSTRUCTION: return sf::Color::Yellow;
        case BuildingStatus::READY: return sf::Color::Green;
        case BuildingStatus::OPERATING: return sf::Color::Blue;
    }
    return sf::Color::White;
} 
std::vector<BuildingTypes> BUILDINGTYPES = {
    BuildingTypes::WORKSPACE,
    BuildingTypes::STORAGESPACE,
    BuildingTypes::CHEST,
    BuildingTypes::FARM,
};











//////////////////////////////////// Building ///////////////////////////////////////
void Building::LoadArt(){
}

std::vector<std::string> Building::GetRecipeNames(){
    std::vector<std::string> out;
    for (auto& recipe : recipes_){
        out.push_back(to_full_string(recipe->GetType()));
    }
    return out;
}

void Building::Operating(){
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
}

bool Building::Preconstruction(double dt)
{
    auto all_reqs_fulfilled = CheckItemReqs();

    if (all_reqs_fulfilled)
    {
        // consume the required items and remove them from my inventory
        for (auto &item : item_reqs_map_)
        {
            //
            auto reqd_amount = item.second;

            // remove from inventory
            inventory_.RemoveFrom(item.first, reqd_amount);
        }
        // set building status to construction
        building_status_ = BuildingStatus::CONSTRUCTION;
    }

    // done
    return true;
}

/////////////////////////////////////// Building ///////////////////////////////////////
void Building::update(double dt){
    // update the building
    // update the building status
    // fsm, finite state machine
    switch (building_status_){
        case BuildingStatus::PRECONSTRUCTION:{
            Preconstruction(dt);
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
            auto all_reqs_fulfilled = IsRecipeFulfilled();
            if (all_reqs_fulfilled){
                // reset inputs_level
                inputs_level_ = level_; // set from the building level as a base
                // remove required items from inventory
                for (auto& input : GetActiveRecipe()->inputs_){
                    double reqd_amount = input.second;
                    // remove from inventory
                    AddToInventory(input.first, -reqd_amount);
                    // adjust inputs_level
                    inputs_level_.Update(inventory_.GetItem(input.first)->level_);
                }
                // set building status to operating
                building_status_ = BuildingStatus::OPERATING;
            }
            break;}
        case BuildingStatus::OPERATING:
            Operating();
            break;
        default:
            break;
    }
}
void Building::AddToInventory(ItemTypes item_type, double yield){
    // add to the storage
    // check if item_type in inventory_map_
    inventory_.AddTo(item_type, yield);
}
RecipePtr Building::GetActiveRecipe(){
    if (active_recipe_idx_ == -1){
        return nullptr;
    }
    return recipes_[active_recipe_idx_];
}
void Building::Draw(sf::RenderTarget& target, double pixelX, double pixelY, double pixelsPerMeterW, double pixelsPerMeterH){
    // draw the building

    auto sprite = GetSprite();
    sprite.setPosition(pixelX, pixelY - pixelsPerMeterH);
    auto rect = sprite.getTextureRect();
    sprite.setScale(sf::Vector2f(pixelsPerMeterW / (float)rect.width, pixelsPerMeterH / (float)rect.height));
    // sprite.setOrigin(0, pixelsPerMeterH); // set origin to bottom left
    // modulate sprite depending on building status
    switch (building_status_){
        case BuildingStatus::PRECONSTRUCTION:
            sprite.setColor(sf::Color::Blue); // it's a blueprint right now
            break;
        case BuildingStatus::CONSTRUCTION:
            sprite.setColor(sf::Color(255, 255, 255, 128)); // transparent
            break;
        case BuildingStatus::READY:
            // no change
            break;
        case BuildingStatus::OPERATING:
            sprite.setColor(sf::Color::Green);
            break;
        default:
            break;
    }

    // draw
    target.draw(sprite);
}

/// @brief For construction
/// @return 
bool Building::CheckItemReqs(){
    // local vars
    bool all_reqs_fulfilled = true; // default true in case item_reqs_map_ is empty
    
    // iterate through item_reqs_map_
    for (auto& item: item_reqs_map_){
        //
        double reqd_amount = item.second;

        // check if item is in my inventory
        if (inventory_.find(item.first)){
            // check if amount in inventory is greater than required
            if (inventory_.GetItem(item.first)->GetAmount() < reqd_amount){
                all_reqs_fulfilled = false;
            }
        } else {
            all_reqs_fulfilled = false;
        }
    }
    return all_reqs_fulfilled;
}
bool Building::IsRecipeFulfilled(){
    // check if all items of the active recipe are fulfilled
    if (active_recipe_idx_ == -1)
        return false;
    
    bool all_reqs_fulfilled = true;
    for (auto input: GetActiveRecipe()->inputs_){
        double reqd_amount = input.second;
        // check if item in inventory
        if (inventory_.find(input.first)){
            // check if amount in inventory is greater than required
            if (inventory_.GetItem(input.first)->GetAmount() < reqd_amount){
                all_reqs_fulfilled = false;
            }
        } else {
            all_reqs_fulfilled = false;
        }
    }
    return all_reqs_fulfilled;
}
bool Building::CanConstruct(){
    return building_status_ == BuildingStatus::PRECONSTRUCTION || building_status_ == BuildingStatus::CONSTRUCTION;
}
bool Building::CanCraft(){
    return (building_status_ == BuildingStatus::READY || building_status_ == BuildingStatus::OPERATING) && HasActiveRecipe();
}
std::vector<ItemPtr> Building::GetNonRecipeItems(){
    std::vector<ItemPtr> non_recipe_items;

    // iterate over inventory
    for (auto& item : GetItemMap()){
        // check if item is in the recipe inputs map or has a zero amount, then skip it
        if (GetActiveRecipe()->inputs_.find(item.first) != GetActiveRecipe()->inputs_.end() || item.second->GetAmount() <= 0.0){
            continue;
        }
        non_recipe_items.push_back(item.second);
    }
    return non_recipe_items;
}
/////////////////////////////////////// End Building ///////////////////////////////////////
// sf::Texture Building::texture_ = sf::Texture();
// sf::Sprite Building::sprite_ = sf::Sprite();
// sf::Texture Workspace::texture_;
// sf::Sprite Workspace::sprite_;
// sf::Texture Storagespace::texture_ = sf::Texture();
// sf::Sprite Storagespace::sprite_ = sf::Sprite();
// sf::Texture Farm::texture_ = sf::Texture();
// sf::Sprite Farm::sprite_ = sf::Sprite();



void Workspace::LoadArt(){
    texture_.loadFromFile("../../../assets/workspace.png");
    sprite_.setTexture(texture_);
}
void Farm::LoadArt(){
    texture_.loadFromFile("../../../assets/farm.png");
    sprite_.setTexture(texture_);
}
void Storagespace::LoadArt(){
    texture_.loadFromFile("../../../assets/storagespace.png");
    sprite_.setTexture(texture_);
}
