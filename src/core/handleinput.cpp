#include "gameplay.hpp"
#include "gamescreen.hpp"
#include "gui.hpp"
#include "handleinput.hpp"

using namespace HandleInputNS;
HandleInput::HandleInput(){
    // default constructor
    auto fcn = [](sf::Event& event){return InputResult::NOTHANDLED;};
    SetRunCb(fcn);
}
HandleInput::HandleInput(std::function<InputResult(sf::Event&)> run_cb){
    SetRunCb(run_cb);
}
bool HandleInput::CanAccess(){
    // two states: either locked is false, in which case proceed, or if locked is true, then only proceed if I'm the locker
    if (can_override || !global_lock || (global_lock && local_lock))
        return true;
    else
        return false;
}
InputResult HandleInput::UpdateLock(InputResult result){
    // this only runs if this class instance is the one that locked it (or it's unlocked)
    if (!global_lock || (global_lock && local_lock)){
        // unlock the mutex
        if (result == InputResult::PERSISTENT){
            global_lock = true; // sets the static
            local_lock = true; // sets the local so that only this class can unlock it
        } else {
            global_lock = false;
            local_lock = false;
        }
    }
    return result;
}
InputResult HandleInput::Run(sf::Event& event){
    auto result = InputResult::NOTHANDLED;

    // try to handle it myself
    if (CanAccess()){
        // run the onRun function
        result = m_run_cb(event);
    }

    // if I didn't handle it (or can't handle it), then pass it to the children
    if (UpdateLock(result) == InputResult::NOTHANDLED){
        // iterate through children, order matters due to the early exit
        for (auto& child: children){
            result = child->Run(event); // recurse
            if (result >= InputResult::HANDLED)
                break;
        }
    }

    return result;
}
void HandleInput::AddChild(std::shared_ptr<HandleInput> child){
    if (child != nullptr)
        children.push_back(child);
}
bool HandleInput::Lock(){
    // lock the mutex
    if (CanAccess()){
        global_lock = true;
        local_lock = true;
        return true;
    } else {
        return false;
    }
}
bool HandleInput::Unlock(){
    // unlock the mutex
    if (CanAccess()){
        global_lock = false;
        local_lock = false;
        return true;
    } else {
        return false;
    }
}

HandleInputNS::InputResult GamePlay::onHandleInput(sf::Event& event){
    // the idea is to have the actions resultant from the sf::Event& event be context dependent, as in, depending on what the user has currently selected, and what keystrokes they make, the action that results is different.
    // this allows each class member to implement its own HandleInput function which updates its own actions, and then in the update step, it'll actually execute those actions
    HandleInputNS::InputResult out = HandleInputNS::InputResult::NOTHANDLED;

    // self handle input: gameplay level actions, these are independent of what's going on in the game
    switch (event.type){
        case sf::Event::KeyReleased:
            switch (event.key.scancode){
                default:
                    break;
            }
            break;
        case sf::Event::KeyPressed:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::LShift:
                    // enter task manager edit mode
                    task_manager_ptr_->EnterEditMode(); // # TODO: move this into task manager and use the handle input mutex properly
                    return HandleInputNS::InputResult::HANDLED;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return out;
}

HandleInputNS::InputResult GUI::onHandleInput(sf::Event& event){
    // GUI level input handling

    // iterate through widgets. Only 1 widget can handle an input so we can early exit
    for (auto& widget: widgets_){
        // handle input for each widget
        auto widget_input = widget->HandleInput(event);
        if (widget_input == WidgetInputs::HANDLED){
            return HandleInputNS::InputResult::HANDLED;
        }
    }
    return HandleInputNS::InputResult::NOTHANDLED;
}


HandleInputNS::InputResult Viewport::onHandleInput(sf::Event& event){
    // handle input for the title screen

    switch (event.type){
        ///////////////// key pressed /////////////////
        case sf::Event::KeyPressed:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::A:
                    std::cout << "Move viewport left" << std::endl;
                    viewport_actions_.SetFlag((BitEnum)ViewportActions::SCROLL_LEFT);
                    return HandleInputNS::InputResult::HANDLED;
                    break;
                case sf::Keyboard::Scan::D:
                    std::cout << "Move viewport right" << std::endl;
                    viewport_actions_.SetFlag((BitEnum)ViewportActions::SCROLL_RIGHT);
                    return HandleInputNS::InputResult::HANDLED;
                    break;
                case sf::Keyboard::Scan::W:
                    std::cout << "Move viewport up" << std::endl;
                    viewport_actions_.SetFlag((BitEnum)ViewportActions::SCROLL_UP);
                    return HandleInputNS::InputResult::HANDLED;
                    break;
                case sf::Keyboard::Scan::S:
                    std::cout << "Move viewport down" << std::endl;
                    viewport_actions_.SetFlag((BitEnum)ViewportActions::SCROLL_DOWN);
                    return HandleInputNS::InputResult::HANDLED;
                    break;
                default:
                    break;
            }
            break;
        case sf::Event::KeyReleased:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::A:
                    std::cout << "Stop moving viewport left" << std::endl;
                    viewport_actions_.UnsetFlag((BitEnum)ViewportActions::SCROLL_LEFT);
                    return HandleInputNS::InputResult::HANDLED;
                    break;
                case sf::Keyboard::Scan::D:
                    std::cout << "Stop moving viewport right" << std::endl;
                    viewport_actions_.UnsetFlag((BitEnum)ViewportActions::SCROLL_RIGHT);
                    return HandleInputNS::InputResult::HANDLED;
                    break;
                case sf::Keyboard::Scan::W:
                    std::cout << "Stop moving viewport up" << std::endl;
                    viewport_actions_.UnsetFlag((BitEnum)ViewportActions::SCROLL_UP);
                    return HandleInputNS::InputResult::HANDLED;
                    break;
                case sf::Keyboard::Scan::S:
                    std::cout << "Stop moving viewport down" << std::endl;
                    viewport_actions_.UnsetFlag((BitEnum)ViewportActions::SCROLL_DOWN);
                    return HandleInputNS::InputResult::HANDLED;
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
                viewport_actions_.SetFlag((BitEnum)ViewportActions::ZOOM_IN_ONCE);
                    return HandleInputNS::InputResult::HANDLED;
            } else {
                std::cout << "Zoom out" << std::endl;
                viewport_actions_.SetFlag((BitEnum)ViewportActions::ZOOM_OUT_ONCE);
                    return HandleInputNS::InputResult::HANDLED;
            }
            break;
        default:
            break;
    }

    return HandleInputNS::InputResult::NOTHANDLED;
}

HandleInputNS::InputResult Map::onHandleInput(sf::Event& event){
    switch (event.type){
        case sf::Event::KeyReleased:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::Num1:{
                    std::cout << "Build workspace" << std::endl;
                    MakeBuilding(BuildingTypes::WORKSPACE, mouse_x_, mouse_y_);
                    return HandleInputNS::InputResult::HANDLED;
                    break;}
                case sf::Keyboard::Scan::Num2:{
                    std::cout << "Build farm" << std::endl;
                    // create a farm
                    MakeBuilding(BuildingTypes::FARM, mouse_x_, mouse_y_);
                    return HandleInputNS::InputResult::HANDLED;
                    break;}
                case sf::Keyboard::Scan::Num3:{
                    MakeBuilding(BuildingTypes::STORAGESPACE, mouse_x_, mouse_y_);
                    return HandleInputNS::InputResult::HANDLED;
                    break;}
                case sf::Keyboard::Scan::Num0:{
                    std::cout << "Make worker" << std::endl;
                    // create a worker
                    MakeWorker(mouse_x_, mouse_y_);
                    return HandleInputNS::InputResult::HANDLED;
                    break;}
                default:
                    break;
            }
            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left){
                std::cout << "Left mouse button pressed" << std::endl;
                SetMouseDownPosition();
                return HandleInputNS::InputResult::HANDLED;
            }
            break;
        case sf::Event::MouseButtonReleased:
            std::cout << "mouse button: " << event.mouseButton.button << std::endl;
            std::cout << "mouse x: " << event.mouseButton.x << std::endl;
            std::cout << "mouse y: " << event.mouseButton.y << std::endl;
            if (event.mouseButton.button == sf::Mouse::Left){
                std::cout << "Left mouse button released" << std::endl;
                // select a dynamic object
                onSelect();
                return HandleInputNS::InputResult::HANDLED;
            }
            if (event.mouseButton.button == sf::Mouse::Right){
                std::cout << "Right mouse button released" << std::endl;
                InferAction();
                return HandleInputNS::InputResult::HANDLED;
            }
            break;
        default:
            break;
    }

    // selected workers take next precedence, apply the event to all selected objects
    // this is broken if any workers use persistent handleinputs
    auto done = HandleInputNS::InputResult::NOTHANDLED;
    for (auto& dynamic_object_ptr : selected_dynamic_object_ptrs_){
        if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::WORKER){
            auto worker = std::dynamic_pointer_cast<Worker>(dynamic_object_ptr);
            if (worker->HandleInput(event) >= HandleInputNS::InputResult::HANDLED)
                done = HandleInputNS::InputResult::HANDLED;
        }
    }
    return done;
}
HandleInputNS::InputResult Worker::onHandleInput(sf::Event& event){
    // TODO: move this one level higher to the map, and then iterate over selected objects within the switch-case (through a cb)

    switch (event.type){
        case sf::Event::KeyReleased:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::G:
                    std::cout << "Toggle gathering" << std::endl;
                    SetState(WorkerStates::GATHERIDLE);
                    return HandleInputNS::InputResult::HANDLED;
                    break;
                case sf::Keyboard::Scan::C:
                    std::cout << "Toggle constructing" << std::endl;
                    SetState(WorkerStates::CONSTRUCTINGIDLE);
                    return HandleInputNS::InputResult::HANDLED;
                    break;
                case sf::Keyboard::Scan::R:
                    std::cout << "Toggle crafting" << std::endl;
                    SetState(WorkerStates::CRAFTINGIDLE);
                    return HandleInputNS::InputResult::HANDLED;
                    break;
                case sf::Keyboard::Scan::T:
                    std::cout << "Transfer Inventory" << std::endl;
                    TransferInventory();
                    return HandleInputNS::InputResult::HANDLED;
                    break;
                case sf::Keyboard::Scan::E:
                    std::cout << "Start executing task" << std::endl;
                    SetState(WorkerStates::EXECUTINGTASK);
                    return HandleInputNS::InputResult::HANDLED;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return HandleInputNS::InputResult::NOTHANDLED;
}

HandleInputNS::InputResult TaskManager::onHandleInput(sf::Event event){
    // handle input for the task manager
    // if we're in easy edit mode, then the task manager claims the handle input by returning PERSISTENT
    if (!in_easy_edit_mode_){
        return HandleInputNS::InputResult::NOTHANDLED;
    }

    switch (event.type){
        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button){
                case sf::Mouse::Right:
                    InferAction();
                    return HandleInputNS::InputResult::PERSISTENT;
                    break;
            }
            break;
        case sf::Event::KeyReleased:
            switch (event.key.scancode){
                case sf::Keyboard::Scancode::LShift:
                    // exit easy edit mode
                    ExitEditMode();
                    return HandleInputNS::InputResult::HANDLED; // not persistent
                    break;
            }
            break;
    }

    // iterate through widgets. Only 1 widget can handle an input so we can early exit
    for (auto& widget: widgets_){
        // handle input for each widget
        auto widget_input = widget->HandleInput(event);
        if (widget_input == WidgetInputs::HANDLED){
            return HandleInputNS::InputResult::HANDLED;
        }
    }
    return HandleInputNS::InputResult::NOTHANDLED;
}