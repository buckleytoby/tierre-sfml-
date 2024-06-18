#include "gameplay.hpp"


GamePlay::GamePlay(){
    // constructor

    // init pointers
    viewport_ = std::make_shared<Viewport>();
    map_ = std::make_shared<Map>();
    hud_ = std::make_shared<HUD>();
    task_manager_ptr_ = std::make_shared<TaskManager>(map_, hud_);

    // handleinput stuff
    SetHandleInputCb(std::bind(&GamePlay::onHandleInput, this, std::placeholders::_1));
    HandleInputAddChild(task_manager_ptr_->m_handleinput); // Task Manager takes next precedence (should return if not in easy edit mode)
    HandleInputAddChild(viewport_->m_handleinput); // viewport takes next precedence
    HandleInputAddChild(hud_->m_handleinput); // HUD takes next precedence
    HandleInputAddChild(map_->m_handleinput); // Map takes next precedence

    // make the game
    hud_->map_ref_ = map_;
    hud_->viewport_ref_ = viewport_;

    // Make the map
    map_->SetTaskManager(task_manager_ptr_);
    map_->SetWidth(100);
    map_->SetHeight(100);
    map_->CreateTiles();

    // Set the viewport
    viewport_->SetX(30);
    viewport_->SetY(30);
    viewport_->SetWidth(19);
    viewport_->SetHeight(10);


    // print out the map
    for (int i=0; i<10; i++){
        for (int j=0; j<10; j++){
            // iterate through resources
            if(map_->GetTile(i, j)->HasResource()){
                std::cout << to_string(map_->GetTile(i, j)->GetResource()->GetResourceType()) << " total amount: " << map_->GetTile(i, j)->GetResource()->GetTotal();
            }
        }
        std::cout << std::endl;
    }

    // hard code add a super worker
    map_->MakeWorker(35, 35);
    auto dynaobj = map_->dynamic_object_ptrs_[0];
    auto worker = (Worker*)dynaobj.get();
    // worker->needs_map_[NeedsTypes::FOOD].val_ = 100.0;
    // give worker starting inventory of corn

    // Make the task manager
    // make example tasks
    MakeTask1();

    ////////////////////////// GUI STUFF //////////////////////////
    auto esc = std::make_shared<TextBox>(0, 0, "Press ESC to go back to Title Screen");
    hud_->AddWidget(esc);

    // click & drag border
    auto border = std::make_shared<Border>(0, 0, 1920, 1080);
    border->MakeInvisible();
    hud_->AddWidget(border);

    // stub widget
    auto stub = std::make_shared<Widget>(0, 0, SCREENWIDTH, SCREENHEIGHT);
    stub->SetOnMouseDownCallback([border](){
        border->MakeVisible();
        border->shape_.setPosition(border->mouse_x_, border->mouse_y_);
        border->shape_.setSize(sf::Vector2f(0, 0));
        border->bounds_.left = border->mouse_x_;
        border->bounds_.top = border->mouse_y_;
        return false;
    });
    stub->SetOnClickCallback([border](){
        border->MakeInvisible();
        return false;
    });
    hud_->AddWidget(stub);

    // selected unit status
    auto selected = std::make_shared<SelectedStatus>(0, 50, 300, 900, hud_->map_ref_, task_manager_ptr_);
    hud_->AddWidget(selected);

    //// TASK MANAGER GUI
    // make task manager widget
    auto task_mgr_widget = std::make_shared<TaskManagerWidget>(300, 200, task_manager_ptr_);
    task_mgr_widget->MakeInvisible();
    hud_->AddWidget(task_mgr_widget);

    // task mgr open button
    auto task_mgr_open_button = std::make_shared<Button>(50, 800, "Task Manager");
    task_mgr_open_button->SetOnClickCallback([task_mgr_widget](){
        task_mgr_widget->ToggleVisibility();
        return true;
    });
    hud_->AddWidget(task_mgr_open_button);

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