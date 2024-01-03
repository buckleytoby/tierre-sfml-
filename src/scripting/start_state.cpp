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
    auto cornstalk = std::make_shared<CornstalkResource>(100.0);
    std::shared_ptr<Resource> resource1(lumber);
    std::shared_ptr<Resource> resource2(stone);
    tile->AddResource(resource1);
    tile2->AddResource(resource2);
    tile3->AddResource(std::shared_ptr<Resource>(cornstalk));
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

    // Make the task manager
    task_manager_ptr_ = std::make_shared<TaskManager>();
    // make example tasks
    MakeTask1();

    ////////////////////////// GUI STUFF //////////////////////////
    auto esc = std::make_shared<TextBox>(0, 0, "Press ESC to go back to Title Screen");
    hud_.AddWidget(esc);

    // selected unit status
    auto selected = std::make_shared<SelectedStatus>(300, 200, 300, 900, hud_.map_ref_, task_manager_ptr_);
    hud_.AddWidget(selected);

    //// TASK MANAGER GUI
    // make task manager widget
    auto task_mgr_widget = std::make_shared<TaskManagerWidget>(300, 200, task_manager_ptr_);
    task_mgr_widget->MakeInvisible();
    hud_.AddWidget(task_mgr_widget);

    // task mgr open button
    auto task_mgr_open_button = std::make_shared<Button>(50, 800, "Task Manager");
    task_mgr_open_button->SetOnClickCallback([task_mgr_widget](){
        task_mgr_widget->ToggleVisibility();
    });
    hud_.AddWidget(task_mgr_open_button);

    //// end task manager GUI


    ////////////////////////// END GUI STUFF //////////////////////////
}
void GamePlay::MakeTask1(){
    // example task, goes to closest building and empties inventory
    std::vector<ActionTypes> actions = // {"SelectClosestBuilding", "SetGoalToSelectedBuilding", "MoveTowardsGoal", "TransferInventory"};
    {
        ActionTypes::SelectClosestBuilding,
        ActionTypes::SetGoalToSelectedBuilding,
        ActionTypes::MoveTowardsGoal,
        ActionTypes::TransferInventory
    };
    
    TaskPtr task = std::make_shared<Task>("Example Task 1");
    for (int i=0; i<actions.size(); i++){
        ActionPtr action1 = std::make_shared<Action>(actions[i]);
        task->AddAction(action1);
    }
    // add to task manager
    task_manager_ptr_->AddTask(task);
    task_manager_ptr_->EditTask(0);
}