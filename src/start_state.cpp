#include "gameplay.hpp"


GamePlay::GamePlay(){
    // constructor
    hud_.map_ref_ = MapPtr(&map_);
    hud_.viewport_ref_ = ViewportPtr(&viewport_);

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

    // make a pre-built task
    worker->MakeTask1();

    map_.dynamic_object_ptrs_.push_back(std::unique_ptr<DynamicObject>((DynamicObject*)worker));

    // test dropdown
    auto dropdown = std::make_shared<Dropdown>(20, 500, std::vector<std::string>{"test1", "test2", "test3"});
    hud_.AddWidget(dropdown);

    // finalize all widgets
    for (auto& widget : hud_.widgets_){
        widget->Finalize();
    }
}