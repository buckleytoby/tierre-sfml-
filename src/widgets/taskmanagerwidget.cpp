#include "widgets.hpp"

TaskManagerWidget::TaskManagerWidget(double x, double y, double w, double h, TaskManagerPtr task_manager_ptr): Widget(x, y, w, h)
{
    task_manager_ptr_ = task_manager_ptr;

    // make the child widgets
    add_new_task = MakeButton(5, 5, "New Task");
    add_new_task->SetOnClickCallback([this](){
        task_manager_ptr_->NewTask();
        // must redraw
        reDraw();
    });
    remove_task = MakeButton(50, 5, "Remove Task");
    remove_task->SetOnClickCallback([this](){
        task_manager_ptr_->RemoveTask(list_of_tasks->GetClickedIdx());
        // must redraw
        reDraw();
    });
    edit_task = MakeButton(100, 5, "Edit Task");
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
    children_.clear();

    // dropdowns
    list_of_tasks = std::make_shared<Dropdown>(5, 50, task_manager_ptr_->GetTaskNames());

    current_task = std::make_shared<Dropdown>(75, 50, task_manager_ptr_->GetActiveTaskActionNames());

    // add all widgets
    AddChild(add_new_task);
    AddChild(remove_task);
    AddChild(edit_task);
    AddChild(list_of_tasks);
    AddChild(current_task);
}
// void TaskManagerWidget::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const {
//     // for now redraw
//     // TODO: only redraw when a change has occured
//     reDraw();
// }