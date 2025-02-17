#include "widgets.hpp"

SelectWorkerAction::SelectWorkerAction(double x, double y, double w, double h): Widget(x, y, w, h)
{
    //// put together the UI
    // list of strings
    std::vector<std::string> strs;

    // iterate through worker states
    for (int i=(int)WorkerStates::begin+1; i<(int)WorkerStates::end-1; i++)
    {
        // cast to the enum
        auto state = static_cast<WorkerStates>(i);

        // get the string
        strs.push_back(to_string(state));
    }
    
    // make the container, we'll use a dropdown
    auto container = std::make_shared<Dropdown>(0, 50, strs);

    container->SetOnClickCallback(
        [this]()
        {
            MakeInvisible();

            return true;
        }
    );

    // add to children
    AddChild(container);
}