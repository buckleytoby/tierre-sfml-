#include "widgets.hpp"



WorkerWidget::WorkerWidget(double x, double y, double w, double h, WorkerPtr worker_ptr): Widget(x, y, w, h)
{
    worker_ptr_ = worker_ptr;

    // iterate through action_primitive_map_ and make a button for each one
    double xi = 0;
    double yi = 0;
    for (auto& action_primitive : worker_ptr_->action_primitive_map_){
        // auto button = std::make_shared<Button>(x, y, 100, 50, to_string(action_primitive.first));
        // button->onClick = [this, action_primitive](){
        //     worker_ptr_->SetActionPrimitive(action_primitive.first);
        // };


        yi += 75;
    }
}

void WorkerWidget::onUpdate(double dt, double x, double y)
{
}

void WorkerWidget::reDraw()
{
}
