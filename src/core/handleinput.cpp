#include "gameplay.hpp"
#include "gui.hpp"


GameScreenInputs GamePlay::HandleInput(sf::Event& event){
    // the idea is to have the actions resultant from the sf::Event& event be context dependent, as in, depending on what the user has currently selected, and what keystrokes they make, the action that results is different.
    // this allows each class member to implement its own HandleInput function which updates its own actions, and then in the update step, it'll actually execute those actions

    // gameplay level actions, these are independent of what's going on in the game

    switch (event.type){
        case sf::Event::KeyReleased:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::Escape:
                    std::cout << "Change active gamescreen TitleScreen" << std::endl;
                    return GameScreenInputs::CHANGEACTIVEGAMESCREENTITLESCREEN;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    // update mouse position
    viewport_.UpdateMousePosition();
    map_.SetMousePosition(viewport_.map_mouse_x_, viewport_.map_mouse_y_);
    hud_.SetMousePosition(viewport_.mouse_x_, viewport_.mouse_y_);

    // viewport takes next precedence
    auto viewportinputs = viewport_.HandleInput(event);
    switch (viewportinputs){
        case ViewportInputs::NONE:
            break;
        case ViewportInputs::HANDLED:
            return GameScreenInputs::NONE;
            break;
        default:
            break;
    }
    
    // HUD takes next precedence
    auto hudinputs = hud_.HandleInput(event);
    switch (hudinputs){
        case GUIInputs::NONE:
            break;
        case GUIInputs::HANDLED:
            return GameScreenInputs::HANDLED;
            break;
        default:
            break;
    }

    // Map takes next precedence
    auto mapinputs = map_.HandleInput(event);
    switch (mapinputs){
        case MapInputs::NONE:
            break;
        case MapInputs::HANDLED:
            return GameScreenInputs::HANDLED;
            break;
        default:
            break;
    }


    // not handled by this or any children of this component
    return GameScreenInputs::NONE;

}

GUIInputs GUI::HandleInput(sf::Event& event){
    // GUI level input handling

    // iterate through widgets. Only 1 widget can handle an input so we can early exit
    for (auto& widget: widgets_){
        // handle input for each widget
        auto widget_input = widget->HandleInput(event);
        if (widget_input == WidgetInputs::HANDLED){
            return GUIInputs::HANDLED;
        }
    }
    return GUIInputs::NONE;
}


ViewportInputs Viewport::HandleInput(sf::Event& event){
    // handle input for the title screen

    switch (event.type){
        ///////////////// key pressed /////////////////
        case sf::Event::KeyPressed:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::A:
                    std::cout << "Move viewport left" << std::endl;
                    viewport_actions_.SetFlag((EFlagValue)ViewportActions::SCROLL_LEFT);
                    return ViewportInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::D:
                    std::cout << "Move viewport right" << std::endl;
                    viewport_actions_.SetFlag((EFlagValue)ViewportActions::SCROLL_RIGHT);
                    return ViewportInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::W:
                    std::cout << "Move viewport up" << std::endl;
                    viewport_actions_.SetFlag((EFlagValue)ViewportActions::SCROLL_UP);
                    return ViewportInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::S:
                    std::cout << "Move viewport down" << std::endl;
                    viewport_actions_.SetFlag((EFlagValue)ViewportActions::SCROLL_DOWN);
                    return ViewportInputs::HANDLED;
                    break;
                default:
                    break;
            }
            break;
        case sf::Event::KeyReleased:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::A:
                    std::cout << "Stop moving viewport left" << std::endl;
                    viewport_actions_.UnsetFlag((EFlagValue)ViewportActions::SCROLL_LEFT);
                    return ViewportInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::D:
                    std::cout << "Stop moving viewport right" << std::endl;
                    viewport_actions_.UnsetFlag((EFlagValue)ViewportActions::SCROLL_RIGHT);
                    return ViewportInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::W:
                    std::cout << "Stop moving viewport up" << std::endl;
                    viewport_actions_.UnsetFlag((EFlagValue)ViewportActions::SCROLL_UP);
                    return ViewportInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::S:
                    std::cout << "Stop moving viewport down" << std::endl;
                    viewport_actions_.UnsetFlag((EFlagValue)ViewportActions::SCROLL_DOWN);
                    return ViewportInputs::HANDLED;
                    break;
                default:
                    break;
            }
            break;
        case sf::Event::MouseWheelScrolled:
            std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
            std::cout << "mouse x: " << event.mouseWheelScroll.x << std::endl;
            std::cout << "mouse y: " << event.mouseWheelScroll.y << std::endl;
            if (event.mouseWheelScroll.delta > 0){
                std::cout << "Zoom in" << std::endl;
                viewport_actions_.SetFlag((EFlagValue)ViewportActions::ZOOM_IN_ONCE);
                    return ViewportInputs::HANDLED;
            } else {
                std::cout << "Zoom out" << std::endl;
                viewport_actions_.SetFlag((EFlagValue)ViewportActions::ZOOM_OUT_ONCE);
                    return ViewportInputs::HANDLED;
            }
            break;
        default:
            break;
    }

    return ViewportInputs::NONE;
}

MapInputs Map::HandleInput(sf::Event& event){
    switch (event.type){
        case sf::Event::KeyReleased:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::Num1:{
                    std::cout << "Build workspace" << std::endl;
                    MakeBuilding(BuildingTypes::WORKSPACE, mouse_x_, mouse_y_);
                    break;}
                case sf::Keyboard::Scan::Num2:{
                    std::cout << "Build farm" << std::endl;
                    // create a farm
                    MakeBuilding(BuildingTypes::FARM, mouse_x_, mouse_y_);
                    break;}
                case sf::Keyboard::Scan::Num0:{
                    std::cout << "Make worker" << std::endl;
                    // create a worker
                    MakeWorker(mouse_x_, mouse_y_);
                    break;}
                default:
                    break;
            }
            break;
        case sf::Event::MouseButtonReleased:
            std::cout << "mouse button: " << event.mouseButton.button << std::endl;
            std::cout << "mouse x: " << event.mouseButton.x << std::endl;
            std::cout << "mouse y: " << event.mouseButton.y << std::endl;
            if (event.mouseButton.button == sf::Mouse::Left){
                std::cout << "Left mouse button released" << std::endl;
                // select a dynamic object
                SelectObject(mouse_x_, mouse_y_);
            }
            if (event.mouseButton.button == sf::Mouse::Right){
                std::cout << "Right mouse button released" << std::endl;
                SetAttentionAndMove(mouse_x_, mouse_y_);
            }
            break;
        default:
            break;
    }

    // selected workers take next precedence
    for (auto& dynamic_object_ptr : selected_dynamic_object_ptrs_){
        if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::WORKER){
            auto worker = std::dynamic_pointer_cast<Worker>(dynamic_object_ptr);
            auto workerinputs = worker->HandleInput(event);
            switch (workerinputs){
                case WorkerInputs::NONE:
                    break;
                case WorkerInputs::HANDLED:
                    return MapInputs::HANDLED;
                    break;
                default:
                    break;
            }
        }
    }
    return MapInputs::NONE;
}
WorkerInputs Worker::HandleInput(sf::Event& event){

    switch (event.type){
        case sf::Event::KeyReleased:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::G:
                    std::cout << "Toggle gathering" << std::endl;
                    SetState(WorkerStates::GATHERIDLE);
                    return WorkerInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::C:
                    std::cout << "Toggle constructing" << std::endl;
                    SetState(WorkerStates::CONSTRUCTINGIDLE);
                    return WorkerInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::R:
                    std::cout << "Toggle crafting" << std::endl;
                    SetState(WorkerStates::CRAFTINGIDLE);
                    return WorkerInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::T:
                    std::cout << "Transfer Inventory" << std::endl;
                    TransferInventory();
                    return WorkerInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::E:
                    std::cout << "Start executing task" << std::endl;
                    SetState(WorkerStates::EXECUTINGTASK);
                    return WorkerInputs::HANDLED;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return WorkerInputs::NONE;
}