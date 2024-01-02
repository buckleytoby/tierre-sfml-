#include "widgets.hpp"

TaskManagerWidget::TaskManagerWidget(double x, double y, TaskManagerPtr task_manager_ptr): Widget(x, y)
{
    double w = 1000;
    double h = 600;
    Widget::Widget(x, y, w, h);
    task_manager_ptr_ = task_manager_ptr;

    // border rectangle
    border = std::make_shared<RectWidget>(0, 0, w, h);

    // make the child widgets
    add_new_task = MakeButton(0, 0, "New Task");
    add_new_task->SetOnClickCallback([this](){
        task_manager_ptr_->NewTask();
        // must redraw
        reDraw();
    });
    remove_task = MakeButton(200, 0, "Remove Task");
    remove_task->SetOnClickCallback([this](){
        task_manager_ptr_->RemoveTask(list_of_tasks->GetClickedIdx());
        // must redraw
        reDraw();
    });
    edit_task = MakeButton(400, 0, "Edit Task");
    edit_task->SetOnClickCallback([this](){
        task_manager_ptr_->EditTask(list_of_tasks->GetClickedIdx());
        // must redraw
        reDraw();
    });

    // invoke redraw
    reDraw();
}
void TaskManagerWidget::reDraw(){
    // redraw the widget, based on the task manager

    // dropdowns
    list_of_tasks = std::make_shared<Dropdown>(0, 100, task_manager_ptr_->GetTaskNames());

    current_task = std::make_shared<Dropdown>(300, 100, task_manager_ptr_->GetActiveTaskActionNames());

    list_of_actions = std::make_shared<Dropdown>(600, 100, task_manager_ptr_->GetActionsAndTasks());

    // current_task modification buttons
    add_action = MakeButton(600, 0, "Add Action");
    add_action->SetOnClickCallback([this](){
        task_manager_ptr_->active_task_->NewAndAddAction(
            // get action type from list_of_actions dropdown,
            // then get action type from that
            (ActionTypes)list_of_actions->GetClickedIdx()
        );
        reDraw();
    });
    remove_action = MakeButton(800, 0, "Remove Action");
    remove_action->SetOnClickCallback([this](){
        task_manager_ptr_->active_task_->RemoveAction();
        reDraw();
    });

    // add all widgets
    children_.clear();
    AddChild(border);
    AddChild(add_new_task);
    AddChild(remove_task);
    AddChild(edit_task);
    AddChild(list_of_tasks);
    AddChild(current_task);
    AddChild(list_of_actions);
    AddChild(add_action);
    AddChild(remove_action);
}
bool TaskManagerWidget::onClick(){
    // return true to prevent click-through to background
    return true;
}
// void TaskManagerWidget::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const {
//     // for now redraw
//     // TODO: only redraw when a change has occured
//     reDraw();
// }